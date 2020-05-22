//chdir()로 CWD 변경 후, getcwd() 변경 확인
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX 1024

int main(void)
{
	char *pathname;

	if (chdir("/home/ubuntu") < 0) {	
		fprintf(stderr, "chdir error\n");
		exit(1);
	}

	pathname = malloc(PATH_MAX);
	
	if (getcwd(pathname, PATH_MAX) == NULL) {	 
		fprintf(stderr, "getcwd error\n");
		exit(1);
	}

	printf("currnet directory = %s\n", pathname);
	exit(0);
}
