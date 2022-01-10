// 난수 생성 프로그램
// fputc, read, write 할 때, 정수는 아스키코드번호를 이용해서 문자로 바꿔야 한다

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {

	int i;
	int array[50];
	int d;
	FILE *fp;

	for (i = 0; i < 50; i++) {
		array[i] = rand() % 10;
		printf("%d", array[i]);
	}

	if ((fp = fopen("ssu_test1.txt", "w+")) == NULL) {
		fprintf(stderr, "fopen error\n");
		exit(1);
	}

	for (i = 0; i < 50; i++) {
		fputc(array[i], fp);		// 이렇게 실행하면, 이상한 값들만 생성.. array[i]+48 로 바꿔야 한다

	}

	fclose(fp);
	exit(0);
}