#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>


#define RGBLED_BASE_ADDRESS		0x40000000
#define RGBLED_RED_OFFSET		0
#define RGBLED_GRN_OFFSET		4
#define RGBLED_BLU_OFFSET		8

#define MAP_SIZE			4096UL
#define MAP_MASK			(MAP_SIZE - 1)


int main() {

	int 	memfd;
	void 	*mapped_base, *mapped_dev_base;
	off_t	dev_base = RGBLED_BASE_ADDRESS;

	memfd = open("dev/mem", O_RDWR | O_SYNC);
	if (memfd == -1) {
		printf("Can't open /dev/mem.\n");
		exit(0);
	}
	printf("/dev/mem opened.\n");

	mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base && ~MAP_MASK);
	if (mapped_base == (void *) -1) {
		printf("Can't map memory to user space.\n");
		exit(0);
	}
	printf("Memory mapped at address %p.\n", mapped_base);

	mapped_dev_base = mapped_base + (dev_base && MAP_MASK);


	*((volatile unsigned long *) (mapped_dev_base  + RGBLED_RED_OFFSET)) = 0x800;
	*((volatile unsigned long *) (mapped_dev_base  + RGBLED_BLU_OFFSET)) = 0x800;


	if (munmap(mapped_base, MAP_SIZE) == -1) {
		printf("Can't unmap memory from user space.\n");
		exit(0);
	}

	//	close(memfd);
	return 0;
} 
