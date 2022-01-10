/* /etc/passwd 파일을 password 파일에 복사한다 
main함수에 파일 2개를 인자로 받는다  
/etc/passwd (RDONLY), passwd(WRONLY, 644모드)로 열기  
(버퍼사이즈 1G 바이트)								*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define S_MODE 0644
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	char buf[BUFFER_SIZE];
	int fd1, fd2;	//입력용, 출력용
	int length;

 	 if (argc < 2) {
                fprintf(stderr, "usage : %s <file>\n", argv[0]);
                exit(1);
        }

        if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
                fprintf(stderr,"open error for %s\n", argv[1]);
                exit(1);
        }

	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_MODE)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

	while ((length = read(fd1, buf, BUFFER_SIZE)) > 0)
		write(fd2,buf, length);

	exit(0);
}
