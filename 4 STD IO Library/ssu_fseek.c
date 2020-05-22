//사용자입력으로 받은 position 만큼 fseek() 한 후, getc() 이용하여 출력하기

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	FILE *fp;
	long position;		//사용자입력 값(원하는 문자 위치)
	int character;

	if ((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	printf("Input number >>");
	scanf("%ld", &position);
	fseek(fp, position-1, SEEK_SET);	
	character =  getc(fp);
	printf("%ldth character => %c\n", position, character);
	exit(0);
}
