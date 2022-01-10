/* fgets() 함수로 표준 입력으로부터 입력을 받고, fputs() 함수로 표준 출력을 받는다 
standard output, input error 출력하기	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	while (fgets(buf, BUFFER_SIZE, stdin) != NULL)
		if (fputs(buf, stdout) == EOF) {
			fprintf(stderr, "standard output error\n");
			exit(1);
		}

	if (ferror(stdin)) {	
		fprintf(stderr, "standard input error\n");
		exit(1);
	}

	exit(0);

}
