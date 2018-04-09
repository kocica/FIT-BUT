#include <signal.h>
#include <getopt.h>
#include <rte_eal.h>
#include <rte_common.h>
#include <rte_errno.h>
#include <rte_ethdev.h>
#include <rte_lcore.h>
#include <rte_malloc.h>
#include <rte_mbuf.h>
#include <rte_mempool.h>
#include <rte_ring.h>
#include <rte_reorder.h>
#include <rte_ip.h>
#include <pcap/pcap.h>
#include <sys/time.h>
#include <limits.h>

#define RX_DESC_PER_QUEUE 	1024
#define TX_DESC_PER_QUEUE	1024
#define MAX_PKTS_BURST 		32
#define REORDER_BUFFER_SIZE 	1024
#define MBUF_PER_POOL 		65535
#define MBUF_POOL_CACHE_SIZE 	250
#define RING_SIZE 		16384
#define NXP_MIN_PORTS 		1  	/* TODO: 4 */
#define NXP_MAX_PORTS 		4
#define NXP_MIN_LCORES 		2 	/* TODO: 5 */
#define PCAP_MAX_PCKTS 		50 	/* TODO: MAX_BYTES ? */

volatile uint8_t quit_signal, free_mem, transport;

struct timeval start;

static struct rte_mempool *mbuf_pool;

static struct rte_eth_conf port_conf_default = {
        .rxmode = {
                .ignore_offload_bitfield = 1,
        },
};

struct app_stats_rx {
	uint64_t rx_pkts;			/**< Amount of received pckts */
	uint64_t enqueue_pkts;			/**< Amount of pckts enqueued to ring buffer */
	uint64_t enqueue_failed_pkts;		/**< Amount of pckts failed to enqueue to ring buffer */
	uint64_t rx_bytes;			/**< Amount of received bytes */
};

struct app_stats_transport {
        uint64_t dequeue_pkts;			/**< Amount of pckts drained from ring buffer */
        uint64_t saved_pckts;			/**< Amount of pckts written to pcap file */
	uint64_t saved_bytes;			/**< Amount of bytes written to pcap file */
};

struct rx_thread_args {
	struct rte_ring *ring_out; 		/**< Ring where rx thread stores mbufs */
	uint8_t port;				/**< Which port to read from */
	struct app_stats_rx *stats;             /**< RX stats */
};

struct transport_thread_args {
        struct rte_ring *ring_in;		/**< Ring from which transport thread drains mbufs */
        struct rte_reorder_buffer *buffer;	/**< Buffer where mbufs are reordered */
	struct app_stats_transport *stats;      /**< Transport stats */
};


struct cir_buffer {
	unsigned int size;   			/**< Number of entries that can be stored */
	unsigned int mask;   			/**< [buffer_size - 1]: used for wrap-around */
	unsigned int head;   			/**< insertion point in buffer */
	unsigned int tail;   			/**< extraction point in buffer */
	struct rte_mbuf **entries;
} __rte_cache_aligned;


struct rte_reorder_buffer {
	char name[32];
	uint32_t min_seqn;  			/**< Lowest seq. number that can be in the buffer */
	unsigned int memsize; 			/**< memory area size of reorder buffer */
	struct cir_buffer ready_buf; 		/**< temp buffer for dequeued entries */
	struct cir_buffer order_buf; 		/**< buffer used to reorder entries */
	int is_initialized;
} __rte_cache_aligned;

static void
reorder_to_ready(struct rte_reorder_buffer *b) {
	struct cir_buffer *order_buf = &b->order_buf,
			*ready_buf = &b->ready_buf;

	while (((order_buf->head + 1) & order_buf->mask) != order_buf->tail &&
			((ready_buf->head + 1) & ready_buf->mask) != ready_buf->tail) {

		/* if we are blocked waiting on a packet, skip it */
		if (order_buf->entries[order_buf->head] == NULL) {
			order_buf->head = (order_buf->head + 1) & order_buf->mask;
		}

		/* Move all ready entries that fit to the ready_buf */
		while (order_buf->entries[order_buf->head] != NULL) {
			ready_buf->entries[ready_buf->head] =
					order_buf->entries[order_buf->head];

			order_buf->entries[order_buf->head] = NULL;

			order_buf->head = (order_buf->head + 1) & order_buf->mask;

			if (((ready_buf->head + 1) & ready_buf->mask) == ready_buf->tail)
				break;

			ready_buf->head = (ready_buf->head + 1) & ready_buf->mask;
		}
	}

	order_buf->head = order_buf->tail = 0;

	//b->min_seqn = ???;
}

static inline void
pktmbuf_free_bulk(struct rte_mbuf *mbuf_table[], unsigned n)
{
        unsigned int 		i;

        for (i = 0; i < n; i++)
                rte_pktmbuf_free(mbuf_table[i]);
}

static inline int
configure_eth_port(uint16_t port_id)
{
        struct 			ether_addr addr;
        const uint16_t 		rxRings = 1, txRings = 1;
        const uint8_t 		nb_ports = rte_eth_dev_count();
        int 			ret;
        uint16_t 		q, nb_rxd = RX_DESC_PER_QUEUE, nb_txd = TX_DESC_PER_QUEUE;
        struct rte_eth_dev_info dev_info;
        struct rte_eth_txconf 	txconf;
        struct rte_eth_conf 	port_conf = port_conf_default;

        if (port_id > nb_ports)
                return -1;
        rte_eth_dev_info_get(port_id, &dev_info);
        if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_MBUF_FAST_FREE)
                port_conf.txmode.offloads |=
                        DEV_TX_OFFLOAD_MBUF_FAST_FREE;
        ret = rte_eth_dev_configure(port_id, rxRings, txRings, &port_conf_default);
        if (ret != 0)
                return ret;
        ret = rte_eth_dev_adjust_nb_rx_tx_desc(port_id, &nb_rxd, &nb_txd);
        if (ret != 0)
                return ret;
        for (q = 0; q < rxRings; q++) {
                ret = rte_eth_rx_queue_setup(port_id, q, nb_rxd,
                                rte_eth_dev_socket_id(port_id), NULL,
                                mbuf_pool);
                if (ret < 0)
                        return ret;
        }
        txconf = dev_info.default_txconf;
        txconf.txq_flags = ETH_TXQ_FLAGS_IGNORE;
        txconf.offloads = port_conf.txmode.offloads;
        for (q = 0; q < txRings; q++) {
                ret = rte_eth_tx_queue_setup(port_id, q, nb_txd,
                                rte_eth_dev_socket_id(port_id), &txconf);
                if (ret < 0)
                        return ret;
        }
        ret = rte_eth_dev_start(port_id);
        if (ret < 0)
                return ret;
        rte_eth_macaddr_get(port_id, &addr);
        printf("Port %u MAC: %02"PRIx8" %02"PRIx8" %02"PRIx8
                        " %02"PRIx8" %02"PRIx8" %02"PRIx8"\n",
                        port_id,
                        addr.addr_bytes[0], addr.addr_bytes[1],
                        addr.addr_bytes[2], addr.addr_bytes[3],
                        addr.addr_bytes[4], addr.addr_bytes[5]);
        rte_eth_promiscuous_enable(port_id);
        return 0;
}

static void
print_stats(struct app_stats_rx *rx, struct app_stats_transport *transport)
{
        const uint8_t nb_ports = rte_eth_dev_count();
        unsigned i;
        struct rte_eth_stats eth_stats;

	printf("\n====================== Stats ======================\n");
        printf("\nRX threads stats:\n");
        printf(" - Pkts received:               %"PRIu64"\n",
                                                rx->rx_pkts);
        printf(" - Pkts parsed and enqd:        %"PRIu64"\n",
                                                rx->enqueue_pkts);
        printf(" - Pkts failed to enq:          %"PRIu64"\n",
                                                rx->enqueue_failed_pkts);
	printf(" - Bytes received:              %"PRIu64"\n",
                                                rx->rx_bytes);

        printf("\nTransport thread stats:\n");
        printf(" - Pkts deqd from rx ring:      %"PRIu64"\n",
                                                transport->dequeue_pkts);
        printf(" - Pkts saved to pcap:          %"PRIu64"\n",
                                                transport->saved_pckts);
        printf(" - Bytes saved to pcap:         %"PRIu64"\n",
                                                transport->saved_bytes);

	printf("\nTX threads stats:\n");
        printf(" - Not implemented\n");

        for (i = 0; i < nb_ports; i++) {
                rte_eth_stats_get(i, &eth_stats);
                printf("\nPort %u stats:\n", i);
                printf(" - Pkts in:                     %"PRIu64"\n", eth_stats.ipackets);
                printf(" - Pkts out:                    %"PRIu64"\n", eth_stats.opackets);
                printf(" - In Errs:                     %"PRIu64"\n", eth_stats.ierrors);
                printf(" - Out Errs:                    %"PRIu64"\n", eth_stats.oerrors);
                printf(" - Mbuf Errs:                   %"PRIu64"\n", eth_stats.rx_nombuf);
        }

	struct timeval end;
	gettimeofday(&end, NULL);
	double t = (end.tv_sec - start.tv_sec) * 1000.0;
	t += (end.tv_usec - start.tv_usec) / 1000.0;

	printf("\nTX threads stats:\n");
	printf(" - Time:                        %f sec\n", t / 1000.0);
	printf(" - Speed:                          Mbps\n");

	printf("\n===================================================\n\n");
}

static void
int_handler(int sig_num)
{
        printf("Recieved signal %d.\n", sig_num);
        quit_signal = 1;
}

static int
rx_thread(void *args)
{
        const uint8_t 		nb_ports = rte_eth_dev_count();
	uint32_t 		out_pckt_counter = 0, pkt_len, pkts_len = 0,
				ipdata_offset, data_len, pad_len = 0, *seqn, *ts;
        uint16_t 		i, ret = 0, nb_rx_pkts, port_id;
        struct rte_mbuf 	*pkts[MAX_PKTS_BURST], *m, *out_pkts[MAX_PKTS_BURST * 128];
	struct ipv4_hdr 	*ip_hdr;
	struct ether_hdr 	*eth_hdr;
	struct rx_thread_args 	*rx_args = (struct rx_thread_args *)args;

	// TODO: Remove
	uint32_t 		spirent_seqn = 0;

        printf("%s() started on lcore %u\n", __func__, rte_lcore_id());

        while (!quit_signal && !free_mem) {
		/* receive packets */
		nb_rx_pkts = rte_eth_rx_burst(rx_args->port, 0, pkts, MAX_PKTS_BURST);
		/* no pckts, skip */
		if (nb_rx_pkts == 0) {
				continue;
		}
		/* add recvd pckts to counter */
		rx_args->stats->rx_pkts += nb_rx_pkts;

		/* for each pckt */
		for(int i = 0; i < nb_rx_pkts; ++i)
		{
			m = pkts[i];

			rx_args->stats->rx_bytes += rte_pktmbuf_data_len(m);

			/* Package packet */
			if (rte_pktmbuf_data_len(m) > 0) {
				/* get eth hdr */
				eth_hdr = rte_pktmbuf_mtod(m, struct ether_hdr *);
				ipdata_offset = sizeof(struct ether_hdr);

				/* get ipv4 hdr */
				ip_hdr = (struct ipv4_hdr *)(rte_pktmbuf_mtod(m, char *) + ipdata_offset);
				ipdata_offset += (ip_hdr->version_ihl & IPV4_HDR_IHL_MASK) * IPV4_IHL_MULTIPLIER;

				/* len of data in pckt (without header) */
				data_len = rte_pktmbuf_data_len(m) - ipdata_offset;

				/* iterate over pckts in package pckt */
				while (pkts_len < data_len)
				{
					/* first 32 bits are seqn number */
					seqn = (uint32_t *)(rte_pktmbuf_mtod(m, char *) + ipdata_offset + pkts_len);

					/* next 32 bits are timestamp */
					ts = (uint32_t *)(rte_pktmbuf_mtod(m, char *) + ipdata_offset + pkts_len + sizeof(uint32_t));
					/* behind these is pckt which seqn and timestamp belongs to */
					ip_hdr = (struct ipv4_hdr *)(rte_pktmbuf_mtod(m, char *) + ipdata_offset + pkts_len + sizeof(struct ether_hdr) + (2*sizeof(uint32_t)));

					/* allocate new mbuf in mbuf pool */
					out_pkts[out_pckt_counter] = rte_pktmbuf_alloc(mbuf_pool);
					if (out_pkts[out_pckt_counter] == NULL || *seqn == 452) {
						/* ran out of memory, tell other threads to stop */
						free_mem = 1;
						/* tell transport thread to transport mbufs */
						transport = 1;
						/* enqueue already allocated mbufs */
						ret = rte_ring_enqueue_burst(rx_args->ring_out,
							(void *)out_pkts, out_pckt_counter, NULL);
						rx_args->stats->enqueue_pkts += ret;
						if (unlikely(ret < out_pckt_counter)) {
								rx_args->stats->enqueue_failed_pkts += (out_pckt_counter-ret);
								pktmbuf_free_bulk(&out_pkts[ret], out_pckt_counter - ret);
						}
						/* finish */
						return 0;
					}

					pkt_len = rte_bswap16(ip_hdr->total_length) + sizeof(struct ether_hdr);

					/* TODO: Remove (outgoing pckts) */
					if (pkt_len < 60)
						pkt_len = 60;

					/* copy pckt data to mbuf data segments(s) */
					rte_memcpy((void*)(rte_pktmbuf_mtod(out_pkts[out_pckt_counter], char*)),
							(void *)(rte_pktmbuf_mtod(m, char *) + ipdata_offset + pkts_len + (2*sizeof(uint32_t))),
							pkt_len);

					/* add aditional info to mbuf eg. seqn number, ts */
					out_pkts[out_pckt_counter]->data_off = 128;
					out_pkts[out_pckt_counter]->seqn = spirent_seqn++; //*seqn;
					out_pkts[out_pckt_counter]->data_len = pkt_len;
					out_pkts[out_pckt_counter]->timestamp = *ts;

					/* move to the next packet OR end of package packet */
					pkts_len += pkt_len + (2*sizeof(uint32_t));

					out_pckt_counter++;
				}

				pkts_len = 0;
				/* Free package pckt mbuf */
				rte_pktmbuf_free(m);
			}
			/* otherwise its regular packet */
			else
			{
				// Move mbuf to the mbuf buffer and assign him seqn + ts TODO */
				out_pkts[out_pckt_counter] = m;
				out_pkts[out_pckt_counter]->seqn = spirent_seqn++; //++(*seqn);
				out_pkts[out_pckt_counter++]->timestamp = time(NULL);
			}
		}


		/* enqueue mbuf buffer to rx_to_transport ring */
		ret = rte_ring_enqueue_burst(rx_args->ring_out,
				(void *)out_pkts, out_pckt_counter, NULL);

		/* did any mbufs fail to enqueue ? */
		rx_args->stats->enqueue_pkts += ret;
		if (unlikely(ret < out_pckt_counter)) {
				rx_args->stats->enqueue_failed_pkts += (out_pckt_counter-ret);
				pktmbuf_free_bulk(&out_pkts[ret], out_pckt_counter - ret);
		}

		out_pckt_counter = 0;
        }
        return 0;
}

static int
transport_thread(struct transport_thread_args *args)
{
	pcap_dumper_t 		*pcap_file_p;
	pcap_t 			*pd;
	struct pcap_pkthdr 	pcap_hdr;
	u_char 			*packet;
	char 			*pcap_file_name = "nxpOut.pcap";

        uint8_t 		outp;
        unsigned 		sent, i, dret;
	uint64_t 		nb_dq_mbufs;
        int 			ret;
	struct 			rte_mbuf *mbuf, *mbufs[MAX_PKTS_BURST],
				*rombufs[MAX_PKTS_BURST] = {NULL};

        printf("%s() started on lcore %u\n", __func__, rte_lcore_id());

        while (!quit_signal) {
                /* deque the mbufs from rx_to_transport ring */
                nb_dq_mbufs = rte_ring_dequeue_burst(args->ring_in,
                                (void *)mbufs, MAX_PKTS_BURST, NULL);

		/* didnt enqueue single packet -> skip */
                if (unlikely(nb_dq_mbufs == 0))
                        continue;

		/* add amount of dequeued pckts to counter */
                args->stats->dequeue_pkts += nb_dq_mbufs;

		/* for each dequed mbuf */
                for (i = 0; i < nb_dq_mbufs; i++) {

			/* ran out of the space for mbufs - transport them to pcap */
			if (transport) {
				for (int k = 0; k < args->stats->dequeue_pkts; k++) {
					/* store mbuf to reorder insert */
					ret = rte_reorder_insert(args->buffer, mbufs[k]);
					if (ret == -1 && ENOSPC)
						printf("Reorder: ENOSPC\n");
					else if (ret == -1 && ERANGE)
						printf("Reorder: ERANGE\n");
				}

				/* dequeue mbufs which stucked in rx_to_transmit ring */
				do {
                			nb_dq_mbufs = rte_ring_dequeue_burst(args->ring_in,
                                		(void *)mbufs, MAX_PKTS_BURST, NULL);
					/* add amount of dequeued pckts to counter */
                			args->stats->dequeue_pkts += nb_dq_mbufs;
					for (int k = 0; k < nb_dq_mbufs; k++) {
						/* store mbuf to reorder insert */
						ret = rte_reorder_insert(args->buffer, mbufs[k]);
						if (ret == -1 && ENOSPC)
							printf("Reorder: ENOSPC\n");
						else if (ret == -1 && ERANGE)
							printf("Reorder: ERANGE\n");
					}
				} while (nb_dq_mbufs > 0);

				/* move mbufs from reorder buffer to ready buffer */
				reorder_to_ready(args->buffer);

				/* Open pcap file */
				pd = pcap_open_dead(DLT_EN10MB, 65535);
				pcap_file_p = pcap_dump_open(pd, pcap_file_name);
				if (pcap_file_p == NULL) {
					rte_exit(EXIT_FAILURE, "PCAP -- pcap_dump_open failed\n");
				}
				printf("Opened pcap file %s\n", pcap_file_name);

				/*
				 * drain MAX_PKTS_BURST of reordered
				 * mbufs for writing to PCAP
				 */
				dret = rte_reorder_drain(args->buffer, rombufs, MAX_PKTS_BURST);
				while (dret != 0) {
					for (int j = 0; j < dret; j++) {
						mbuf = rombufs[j];
						args->stats->saved_bytes += rte_pktmbuf_data_len(mbuf);
						/* add additional info about pckt & save to pcap */
						pcap_hdr.caplen = rte_pktmbuf_data_len(mbuf);
						pcap_hdr.len = rte_pktmbuf_data_len(mbuf);
						pcap_hdr.ts.tv_sec = mbuf->timestamp;
						pcap_hdr.ts.tv_usec = 0;
						packet = rte_pktmbuf_mtod(mbuf, u_char*);
						pcap_dump((u_char *)pcap_file_p, &pcap_hdr, packet);
						args->stats->saved_pckts++;
						/* free mbuf */
						rte_pktmbuf_free(mbuf);
					}
					dret = rte_reorder_drain(args->buffer, rombufs, MAX_PKTS_BURST);
				}

				/* Close pcap file */
				pcap_close(pd);
				pcap_dump_close(pcap_file_p);

				/* finish thread */
				return 0;
                        }
			else  {
				/* store mbuf to reorder insert */
				ret = rte_reorder_insert(args->buffer, mbufs[i]);
				if (ret == -1 && ENOSPC)
					printf("Reorder: ENOSPC\n");
				else if (ret == -1 && ERANGE)
					printf("Reorder: ERANGE\n");
			}
                }
        }

	/* move last mbufs from reorder buffer to ready buffer */
	reorder_to_ready(args->buffer);
	
	/* open pcap file and save pckts into */
	pd = pcap_open_dead(DLT_EN10MB, 65535);
	pcap_file_p = pcap_dump_open(pd, pcap_file_name);
	if (pcap_file_p == NULL) {
		rte_exit(EXIT_FAILURE, "PCAP -- pcap_dump_open failed\n");

	}

	printf("Opened pcap file %s\n", pcap_file_name);

	dret = rte_reorder_drain(args->buffer, rombufs, MAX_PKTS_BURST);
	while (dret != 0) {
		for (i = 0; i < dret; i++) {
			mbuf = rombufs[i];
			args->stats->saved_bytes += rte_pktmbuf_data_len(mbuf);
			/* add additional info about pckt & save to pcap */
			pcap_hdr.caplen = rte_pktmbuf_data_len(mbuf);
			pcap_hdr.len = rte_pktmbuf_data_len(mbuf);
			pcap_hdr.ts.tv_sec = mbuf->timestamp;
			pcap_hdr.ts.tv_usec = 0;
			packet = rte_pktmbuf_mtod(mbuf, u_char*);
			pcap_dump((u_char *)pcap_file_p, &pcap_hdr, packet);
			args->stats->saved_pckts++;
			/* free mbuf */
			rte_pktmbuf_free(mbuf);
		}
		dret = rte_reorder_drain(args->buffer, rombufs, MAX_PKTS_BURST);
	}

	/* close pcap */
	pcap_close(pd);
	pcap_dump_close(pcap_file_p);

        return 0;
}

int
main(int argc, char **argv)
{
        int 			ret;
        unsigned 		nb_ports, lcore_id, master_lcore_id;
        uint16_t 		port_id, nb_ports_available;
        struct 			transport_thread_args transport_args = {NULL, NULL};
	struct 			rx_thread_args rx_args[NXP_MAX_PORTS];
        struct 			rte_ring *rx_to_transport;

        /* catch ctrl-c so we can print stats on exit */
        signal(SIGINT, int_handler);

        /* Initialize EAL */
        ret = rte_eal_init(argc, argv);
        if (ret < 0)
                return -1;

        /* Check if we have enought cores */
        if (rte_lcore_count() < NXP_MIN_LCORES)
                rte_exit(EXIT_FAILURE, "Error, This application needs at "
                                "least %d logical cores to run:\n"
                                "%d lcore for packet RX\n"
                                "1 lcore for packet Transport(TX)\n",
				NXP_MIN_LCORES, NXP_MIN_LCORES-1);

	/* Check if weve got correct number of ports */
        nb_ports = rte_eth_dev_count();
        if (nb_ports < NXP_MIN_PORTS)
                rte_exit(EXIT_FAILURE, "Error: %d ethernet ports expected\n",
			NXP_MIN_PORTS);

	/* Create mbuf pool */
        mbuf_pool = rte_pktmbuf_pool_create("mbuf_pool", MBUF_PER_POOL,
                        MBUF_POOL_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
                        rte_socket_id());
        if (mbuf_pool == NULL)
                rte_exit(EXIT_FAILURE, "%s\n", rte_strerror(rte_errno));

        nb_ports_available = nb_ports;
        /* initialize all ports */
        for (port_id = 0; port_id < nb_ports; port_id++) {
                /* init port */
                printf("Initializing port %u... done\n", port_id);
                if (configure_eth_port(port_id) != 0)
                        rte_exit(EXIT_FAILURE, "Cannot initialize port %"PRIu8"\n",
                                        port_id);
        }
	/* Is any port unavaible ? */
        if (nb_ports_available != nb_ports) {
                rte_exit(EXIT_FAILURE,
                        "%d available ports expected.\n", NXP_MIN_PORTS);
        }

        /* Create ring for inter core communication */
        rx_to_transport = rte_ring_create("rx_to_transport", RING_SIZE, rte_socket_id(),
                        RING_F_SP_ENQ);
        if (rx_to_transport == NULL)
                rte_exit(EXIT_FAILURE, "%s\n", rte_strerror(rte_errno));

	/* Alloc stat structs for both rx & transport threads */
	struct app_stats_rx *rx_stats = rte_zmalloc(NULL, sizeof *rx_stats, 0);
	struct app_stats_transport *transport_stats = rte_zmalloc(NULL, sizeof *transport_stats, 0);

	/* Create reorder buffer */
	transport_args.buffer = rte_reorder_create("PKT_RO", rte_socket_id(),
						REORDER_BUFFER_SIZE);
	if (transport_args.buffer == NULL)
				rte_exit(EXIT_FAILURE, "%s\n", rte_strerror(rte_errno));
	transport_args.buffer->min_seqn = 0;
	transport_args.buffer->is_initialized = 1;

	/* Get master lcore id and leave it to transport_thread */
        master_lcore_id = rte_get_master_lcore();
	
	/* Start timer */
	gettimeofday(&start, NULL);

	/* Start rx_thread() for each port (4 for NXP) */
	for (lcore_id = 0, port_id = 0; lcore_id < rte_lcore_count() && port_id < nb_ports; ++lcore_id) {
		if (lcore_id != master_lcore_id) {
			rx_args[port_id].ring_out = rx_to_transport;
			rx_args[port_id].port = port_id;
			rx_args[port_id].stats = rx_stats;
			rte_eal_remote_launch((lcore_function_t *)rx_thread,
					(void *)&rx_args[port_id++], lcore_id);
		}
	}

        /* Start transport_thread() on master lcore */
	transport_args.ring_in = rx_to_transport;
	transport_args.stats = transport_stats;
        transport_thread(&transport_args);

	/* Wait for lcores to finish */
        RTE_LCORE_FOREACH_SLAVE(lcore_id) {
                if (rte_eal_wait_lcore(lcore_id) < 0)
                        return -1;
        }

        print_stats(rx_stats, transport_stats);
        return 0;
}
