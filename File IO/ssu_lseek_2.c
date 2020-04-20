/* creat로 새파일 열기 (파일모드는 전역변수로 선언. 사용자는 R,W 가능 나머지는 R만 가능)  
buf1 이용해서 파일에 write 하기  
lseek 으로 파일의 시작에서부터 15000으로 오프셋 변경하기  
buf2 이용해서 파일에 write 하기  
(od -c //octal값을 character 포맷으로 보여달라.  파일의 hole이 생긴 것 확인가능 ) */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define CREAT_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)	//creat의 mode인자 상수화

char buf1[] = "1234567890";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
	char *fname = "ssu_hole.txt";
	int fd;

	if((fd = creat(fname, CREAT_MODE)) < 0) {
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);
	}

	if(write(fd, buf1, 12) != 12) {
		fprintf(stderr, "buf1 write error\n");
		exit(1);
	}

	if(lseek(fd, 15000, SEEK_SET) < 0) {
		fprintf(stderr, "lseek error\n");
		exit(1);
	}

	if(write(fd, buf2, 12) != 12) {
		fprintf(stderr, "buf2 write error\n");
		exit(1);
	}

	exit(0);
}


