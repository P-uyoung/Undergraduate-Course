/* 파일 삭제 시점을 알아본다
unlink 후, write() 함수로 "How are you?" 입력 후, 표준출력으로 뿌린다
close() 후, 열기		*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char buf[64];
	char *fname = "ssu_tempfile";
	int fd;
	int length;

	if ((fd = open(fname, O_RDWR | O_CREAT |O_TRUNC, 0600)) < 0) {	//reference counter: 1, link counter :1
		fprintf(stderr, "first open error for %s\n", fname);
		exit(1);
	}

	if (unlink(fname) < 0) {
		fprintf(stderr, "unlink error for %s\n", fname);
		exit(1);
	}	//reference counter :1, link counter :0

	if (write(fd, "How are you?", 12) != 12) {
		fprintf(stderr, "write error\n");
		exit(1);
	}

	lseek(fd, 0, 0);	//read하기 위해 오프셋 파일 처음으로 이동
	if ((length = read(fd, buf, sizeof(buf))) < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}

	else
		buf[length] = 0;	//buf를 표준출력하기 위해 널문자 삽입

	printf("%s\n", buf);

	close(fd);	//reference counter :0 >> 파일 삭제

	if ((fd = open(fname, O_RDWR)) < 0 ) {		// open(fname, O_RD|O_CREAT|O_TRUNC) 하면, 오류X
		fprintf(stderr, "second open error for %s\n", fname);
		exit(1);
	}

	else
		close(fd);

	exit(0);
}
