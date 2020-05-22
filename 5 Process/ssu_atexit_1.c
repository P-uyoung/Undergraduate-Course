#include <stdio.h>
#include <stdlib.h>

void ssu_out(void);

int main(void)
{
	if (atexit(ssu_out)) {		//atexit ;함수포인터 (성공 시, 0 리턴)
		fprintf(stderr, "atexit error\n");
		exit(1);
	}

	exit(0);
}

void ssu_out(void) {
	printf("atexit succeeded!\n");
}

