/* ssu_test.txt(인자로 받을) 파일의 크기를 출력한다
st_size 필드 이용 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	struct stat statbuf;	//파일의 정보를 담을 구조체

	if (argc != 2) {
		fprintf (stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	if ((stat(argv[1],&statbuf)) < 0) {	//파일의 정보를 statbuf에 담는다
		fprintf(stderr, "stat error\n");
		exit(1);
	}

	printf("%s is %ld bytes\n", argv[1], statbuf.st_size);	//off_t형이므로 서식지정자 ld
	exit(0);

}

