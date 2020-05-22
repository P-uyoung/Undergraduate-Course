#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	char *mode = "r";

	if (fopen(fname, mode) == NULL) {		//r모드로 파일 스트림을 열기
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	else 
		printf("Success!\nFilename:<%s>,mode:<%s>\n", fname, mode);	//fopen 성공시, 출력

	exit(0);
}
