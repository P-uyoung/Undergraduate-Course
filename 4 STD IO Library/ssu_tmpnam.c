#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 4096

int main(void) 
{
	char buf[MAX_LINE];
	char name[L_tmpnam];
	FILE *fp;

	printf("temp file 1 : %s\n", tmpnam(NULL));	//임시파일 생성
	tmpnam(name);		//저장된 임시파일 이름을 name에 저장한다
	printf("temp file 2 : %s\n", name);

	if ((fp =tmpfile()) == NULL) {		//임시파일 생성
		fprintf(stderr, "tmpfile error\n");
		exit(1);
	}

	fputs("tmpfile created temporary file.\n", fp);		//해당파일에 문자열 쓰기
	fseek(fp, 0, SEEK_SET);

	if (fgets(buf, sizeof(buf), fp) == NULL ) {		//해당파일에서, 문자열 출력하기
		fprintf(stderr, "fgets error\n");
		exit(1);
	}

	fputs(buf, stdout);
	exit(0);
}
