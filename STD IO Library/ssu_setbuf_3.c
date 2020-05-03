#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	int a, b;
	int i;

	//표준입력에서 입력받은 문자를 buf 에 넣기
	setbuf(stdin, buf);
	scanf("%d %d", &a, &b);

	for (i = 0; buf[i] != '\n'; i++)
		putchar(buf[i]);	

	putchar('\n');
	exit(0);

}

