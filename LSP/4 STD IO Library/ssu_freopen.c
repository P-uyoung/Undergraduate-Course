#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	int fd;

	printf("First printf : Hello, OSLAB\n");

	if((fd = open(fname, O_RDONLY)) < 0) {		//파일 읽기전용으로 오픈
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	if(freopen(fname, "w", stdout) != NULL)		//오픈된 파일 fnmae을 읽기전용 표준출력파일로 연다
		printf("Second printf : Hell, OSLAB!!\n");

	exit(0);

}


