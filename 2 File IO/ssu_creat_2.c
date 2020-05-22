//666 모드 파일을 만든 후, close 한 후 다시 읽고 쓸 수 있는 모드로 파일을 연다
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	int fd;

	if ((fd = creat(fname, 0666)) < 0 ) {		
		fprintf(stderr, "creat error for %s\n ", fname);
		exit(1);
	}
	else {
		close(fd);
		fd = open(fname, O_RDWR);		
		printf("Succeeded!\n<%s> is new readable and writable\n", fname);
	}

	exit(0);
}

