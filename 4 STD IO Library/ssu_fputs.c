//인자로 받은 파일에 gets(), fputs() 으로 출력하고, fgets(), puts() 으로 터미널에 한번 더 출력한다
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	char buf[BUFFER_SIZE];
	FILE *fp;

	if (argc != 2) {
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "w+")) == NULL) {
		fprintf(stderr, "fopen error fir %s\n", argv[1]);
		exit(1);
	}

	fputs("Input String >> ", stdout);	// =printf("Input String>> ");

	//인자로 받는 파일에 출력
	gets(buf);				// =fgets(buf, BUFFER_SIZE, stdin);
	fputs(buf, fp);		
	
	//표준출력으로 출력
	rewind(fp);				//rewind() :스트림의 위치 지정자를 맨 처음으로 설정
	fgets(buf, sizeof(buf), fp);	//파일에서 버퍼에 입력
	puts(buf);		//버퍼 화면에 출력
	fclose(fp);

	exit(0);
}
