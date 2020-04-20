//새파일 open 할 필요없이 fd값을 이용하여, 표준입력으로 입력받아 표준출력으로 뿌려준다
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	int length;

	length = read(0, buf, BUFFER_SIZE);	//0인 fd는 표준입력
	write(1, buf, length);			//1인 fd는 표준출력			
	exit(0);

}