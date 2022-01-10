#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])	//argv :전달받은 문자열들을 가리키는 포인터
{
	int i;

	for (i = 0; i < argc; i++) 
		printf("argv[%d] : %s\n", i, argv[i]);

	exit(0);
}
