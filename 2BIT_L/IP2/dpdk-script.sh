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
