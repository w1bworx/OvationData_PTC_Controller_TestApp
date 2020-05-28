#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 

#define RGBLED_BASE_ADDRESS 	0x40000000
#define RGBLED_BLU 				0
#define RGBLED_GRN				4
#define RGBLED_RED				8
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)


int main() {

	int 	memfd;
	void 	*mapped_base, *mapped_dev_base; 
	off_t 	dev_base = RGBLED_BASE_ADDRESS; 

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
    	if (memfd == -1) {
		printf("Can't open /dev/mem.\n");
		exit(0);
	}
	printf("/dev/mem opened.\n");

	// Map one page of memory into user space such that the device is in that page, but it may not
	// be at the start of the page

	mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
    	if (mapped_base == (void *) -1) {
		printf("Can't map the memory to user space.\n");
		exit(0);
	}
 	printf("Memory mapped at address %p.\n", mapped_base); 

	// get the address of the device in user space which will be an offset from the base 
	// that was mapped as memory is mapped at the start of a page 

	mapped_dev_base = mapped_base + (dev_base & MAP_MASK);

	*((volatile unsigned long *) (mapped_dev_base  + RGBLED_RED)) = 5000;
	*((volatile unsigned long *) (mapped_dev_base  + RGBLED_BLU)) = 5000;


	if (munmap(mapped_base, MAP_SIZE) == -1) {
		printf("Can't unmap memory from user space.\n");
		exit(0);
	}

	close(memfd);
	return 0;
} 
