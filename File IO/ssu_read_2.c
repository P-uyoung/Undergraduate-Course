/* 하나의 "ssu_test.txt"을 서로 다른 fd로 open한다(읽기전용, 644모드) 
(에러처리 or 연산으로 같이 처리) 
fd1, fd2 로 두번씩 읽는다 -두 줄 짜리 (개행문자 유의) 
printf로 뿌려준다 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int count;		//버퍼의 인덱스, read로 읽은 buf를 printf으로 출력해야하므로 필요 (개행문자)
	int fd1, fd2;

	fd1 = open(fname, O_RDONLY, 0644);
	fd2 = open(fname, O_RDONLY, 0644);

	if (fd1 < 0 || fd2 < 0) {	//파일디스크립터1,2에 대한 에러처리
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	//fd1로 출력
	count = read(fd1, buf, 25);
	buf[count] = 0;		
	printf("fd1's first printf : %s\n", buf);
	lseek(fd1, 1, SEEK_CUR);	//개행문자 띄기 위해 커서 한 칸 옮김
	count = read(fd1, buf, 24);
	buf[count] = 0;
	printf("fd1' second printf : %s\n", buf);
	//fd2로 출력
	count = read(fd2, buf, 25);
	buf[count] = 0;
	printf("fd2's first printf : %s\n", buf);
	lseek(fd2, 1, SEEK_CUR);
	count = read(fd2, buf, 24);
	buf[count] = 0;
	printf("fd2's second printf : %s\n", buf);	

	exit(0);
}