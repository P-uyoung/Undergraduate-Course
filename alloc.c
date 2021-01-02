#include "alloc.h"


char *p = NULL;
char buf[4096] ={0x00, };
mem m;

int init_alloc()
{
	if((p = mmap(0, PAGESIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
		perror("mmap error\n");
		exit(1);
	}
	
	// 필요한 데이터 초기화
	for (int i=0; i < PAGESIZE/8; i++) {
		m.mem_alloc[i] = 0;
		m.mem_offset[i] = 0;
	}
		
	return 0;
}

int cleanup()
{
	if(munmap(p, PAGESIZE) == -1) {
		perror("munmap error\n");
		exit(1);
	}

	return 0;
}

char *alloc(int size)
{
	if(size%8 != 0 || size > PAGESIZE)
		return NULL;
			
	int count = size/8;

	for (int i = 0; i < PAGESIZE; i++)
	{
		if(m.mem_alloc[i] == 0)	// size만큼 공간이 있는지 확인
			count--;

		else
			count = size/8;

		if(count == 0) {
			for (int j = 0; j < size/8; j++)
				m.mem_alloc[i-size/8+1+j] = 1;
			m.mem_offset[i-size/8+1] = size/8;
			return p+8*(i-size/8+1);
		}
	
	}
	
	return NULL;
}

void dealloc(char *alloc_ptr)
{
	int dsize = (alloc_ptr - p) / 8;
	
	for (int i = 0; i < m.mem_offset[dsize]; i++)
		m.mem_alloc[dsize+i] = 0;

}
