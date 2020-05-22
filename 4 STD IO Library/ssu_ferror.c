/* feof(), ferror()로 에러발생인지 EOF 상황인지 구분하여, 에러발생시 clearerr()로 에러와 EOF 플래그 초기화
feof(), ferror()로 파일의 마지막까지 읽으면서, 에러 발생시 "Error detected" 출력한다	*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	int character;

	if (argc != 2) {
		fprintf(stderr, "usage : %s <filename>\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}

	character = fgetc(fp);			//c(19), c(29) :파일 끝 읽을 때 ,EOF 플래그 설정되도록

	while (!feof(fp)) {		
		fputc(character, stdout);

		if (ferror(fp)) {
			fprintf(stderr, "Error detected!\n");
			clearerr(fp);
		}

		character = fgetc(fp);

	}
	
	fclose(fp);
	exit(0);
}
