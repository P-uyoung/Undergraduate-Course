#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	if (chdir("/etc") < 0) {		//etc 로 작업디렉토리 변경
		fprintf(stderr, "child error\n");
		exit(1);
	}
	printf("chdir to /etc succeeded.\n");
	exit(0);
}
