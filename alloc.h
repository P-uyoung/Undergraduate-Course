#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGESIZE 4096 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

typedef struct mem {
	int mem_alloc[PAGESIZE/8];	// 8바이트 단위 메모리 할당 여부 배열
	int mem_offset[PAGESIZE/8];	// 할당한 size 기록 배열
} mem;

// function declarations
int init_alloc();
int cleanup();
char *alloc(int);
void dealloc(char *);
