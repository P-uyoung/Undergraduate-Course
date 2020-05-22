//666 모드 파일 생성하고, Filename, Descriptor 출력하기
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char *fname = "ssu_test.txt";		
	int fd;

	if((fd = creat(fname, 06666)) < 0){	
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);			
	}
	else {
		printf("Success!\nFilename : %s\nDescriptor : %d\n", fname, fd);
		close(fd);		//파일 닫기
	}

	exit(0);


}
