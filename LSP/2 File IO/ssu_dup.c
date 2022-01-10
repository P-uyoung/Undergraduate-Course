/* fd1, fd2를 이용하여 파일 이어 읽기 
open 으로 fd1으로 파일 열기 
dup으로 fd2는 fd1과 동일한 항목 공유 
fd1으로 12바이트 read 하기 > printf 버퍼로 출력하기 
fd2으로 12바이트 이어 read하기 > printf 버퍼로 출력하기 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int count;
	int fd1, fd2;

	if ((fd1 = open(fname, O_RDONLY, 0644)) < 0) {
                fprintf(stderr,"open error for %s\n", fname);
                exit(1);
        }
	
	//fd1,fd2로 파일 이어서 출력
	fd2 = dup(fd1);		//fd1과 FT상에서 동일한 항 공유
	count = read(fd1, buf, 12);
	buf[count] = 0;		//개행문자 넣기
	printf("fd1's printf : %s\n", buf);
	lseek(fd1, 1, SEEK_CUR);	//동일한 항목 공유이므로, 13이 아니다
	count = read(fd2, buf, 12);
	buf[count] = 0;
	printf("fd2's printf : %s\n", buf);
	exit(0);
}
