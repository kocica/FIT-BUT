~/Desktop/dpdk/usertools$

	python dpdk-devbind.py --status


	sudo modprobe uio_pci_generic
		      igb


	python dpdk-devbind.py --bind=uio_pci_generic enp0s3
				      igb_uio


	
	
	./dpdk-setup.sh


	17


	sudo insmod ../build/kmod/igb_uio.ko





	# Hugepages
	sudo sysctl -w vm.nr_hugepages=1024

	export RTE_SDK=/home/$USER/Desktop/dpdk/





	cd /media/nic30/BOOT
	cp ~/yocto-sdk/build_ls2088ardb/tmp/deploy/images/ls2088ardb/itbImage-fsl-image-networking-ls2088ardb-4.9-r0-20180410132714.bin .
	mv itbImage-fsl-image-networking-ls2088ardb-4.9-r0-20180410132714.bin itbImage-fsl-image-networking-ls2088ardb-4.9-r0-20180130171828.bin
	sync




	ip link set eth0 up
	ip addr add 192.168.0.201/24 dev eth0


	mkdir /mnt/huge
	mount -t hugetlbfs nodev /mnt/huge

	export DPRC=dprc.2

	source /usr/odp/scripts/dynamic_dpl.sh dpmac.1 dpmac.2 dpmac.3 dpmac.4




	U spirentu je konfigurace xkocic01PkgPkt v IP podslozce
	Je potreba dat velikost paketu 156
	!!!! Odskrtnout v general aby spirent pridaval nejake blbosti !!!!



