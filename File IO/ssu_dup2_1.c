{/* printf 중간에 dup(fd,1) 한다면? 
 creat 666모드로 "ssu_test.txt"파일 생성
 결과 : printf 하면, 표준출력(fd=1)로 나가, 따라가면 기존의 파일을 덮어쓰게 된다 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	int fd;

	if ((fd = creat(fname, 0666)) < 0) {
		printf("creat error for %s\n", fname);
		exit(1);
	}

	printf("First printf is on the screen.\n");
	dup2(fd,1);	//fd(3)을 fd(1)에 복사
	printf("Second printf is in this file.\n");	//출력안됨
	exit(0);
}

