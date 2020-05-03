#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char name[L_tmpnam];
	FILE *fp;

	printf("Temporary filename <<%s>>\n", tmpnam(name));

	if ((fp = tmpfile()) == NULL) {		//임시 파일 생성
		fprintf(stderr, "tmpfile create error!!\n");
		exit(1);
	}

	fputs("create tmpfile success!!\n", fp);	//fp 파일에 해당 문자열 쓰기
	rewind(fp);		//커서 맨 처음으로 이동
	fgets(buf, sizeof(buf), fp);		//fp 파일에서 buf에 문자열 저장
	puts(buf);		//화면에 출력
	exit(0);
}
