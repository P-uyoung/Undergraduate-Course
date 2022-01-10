//fseek(), ftell()을 이용하여 스트림의 파일크기 구하기
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	FILE *fp;
	long fsize;
	
	if ((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);	//fsize: 표준 입출력 스트림의 파일크기
	printf("The size of <%s> is %ld bytes\n", fname, fsize);
	exit(0);
}
