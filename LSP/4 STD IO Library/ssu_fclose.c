#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	FILE *fp;

	if ((fp = fopen(fname, "r")) == NULL) {		//파일 스트림 읽기전용으로 오픈
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	else {
		printf("Succeess!\n");		//성공 시 출력
		printf("Opening \"%s\" in \"r\" mode!\n", fname);
	}

	fclose(fp);	//오픈된 파일 스트림 닫기
	exit(0);
}
