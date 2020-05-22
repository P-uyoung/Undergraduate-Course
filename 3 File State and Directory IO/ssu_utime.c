/* 인자로 지정한 파일들(ssu_test1.txt, ssu_test2.txt)의 atime과 mtime이 변경되지 않게 파일 오픈 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char *argv[])
{
	struct utimbuf time_buf;
	struct stat statbuf;
	int fd;
	int i;

	for (i = 1; i <  argc; i++) {
		if (stat(argv[i], &statbuf) < 0) {			//open 하기 전, 파일의 actime, modtime 정보를
			fprintf(stderr, "stat error for %s\n", argv[i]);
			continue;
		}

		if ((fd = open(argv[i],O_RDWR | O_TRUNC)) < 0) {
			fprintf(stderr, "open error for %s\n", argv[i]);
			continue;
		}

		close(fd);
		time_buf.actime = statbuf.st_atime;		
		time_buf.modtime = statbuf.st_mtime;	
		
		if (utime(argv[i], &time_buf) < 0) {			//utime 을 통해, stat()을 통해 리턴된 시간값으로 지정
			fprintf(stderr, "utime error for &s\n", argv[i]);
			continue;
		}

	}


	exit(0);
}
