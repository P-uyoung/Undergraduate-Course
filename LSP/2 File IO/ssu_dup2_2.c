/* 파일을 읽고, 숫자 4인 표준출력 파일디스크립터로 출력해준다 
open   읽기전용, 644모드로 열기 
while문으로 read한 값이 0이 되기 전까지 read-write하기
 
cf) printf/ write로 출력하기 
	-printf  (ssu_read_2.c 예제)
		count = read(fd, buf, BUFFER_SIZE);  
		buf[count] = 0;  
		printf("%s", buf);
	
	-write  (ssu_write_1.c 예제)
		while ((length = read(fd, buf, BUFFER_SIZE)) > 0)    
				write(1, buf, length);                        */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname =  "ssu_test.txt";
	int fd;
	int length;

	if ((fd = open(fname, O_RDONLY, 0644)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	if (dup2(1,4) != 4) {	// =dup(1);
		fprintf(stderr, "dup2 call failed\n");
		exit(1);
	}

	while (1) {
		length = read(fd, buf, BUFFER_SIZE);

		if (length <= 0)
			break;

		write(4, buf, length);

	}

	exit(0);
}

