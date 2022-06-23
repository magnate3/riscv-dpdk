#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#define MAP_SIZE 1024*1024
#define rte_bswap32(x) __builtin_bswap32(x)
int main(void){
        int val = 0xffff1111;
        printf("val %x and sizeof %d \n", val , sizeof(val));
        printf("val %x and sizeof %d \n",  rte_bswap32(val) , sizeof(val));
	return 0;
}
