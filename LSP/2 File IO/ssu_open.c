//읽기전용 파일로 파일열기
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char *fname = "ssu_text.txt";
	int fd;

	if((fd = open(fname,O_RDONLY)) < 0) {			
		fprintf(stderr, "open error for %s\n",fname);	//fprintf :file printf
		exit(1);
	}
	else
		printf("Success!\nFilenave : %d\nDescriptor : %d\n", fname, fd);
	exit(0);
}

