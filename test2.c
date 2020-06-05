#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	struct stat statbuf;

	if (argc != 3) {
		fprintf(stderr, "usage : %s <oldname><newname>\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	stat(argv[1], &statbuf);

	printf("%d\n", statbuf.st_mtime);
	close(fd);

	if (rename(argv[1], argv[2]) < 0) {	//파일명 ssu_test1.txt에서 ssu_test2.txt로 바꾼다
		fprintf(stderr, "rename error\n");
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0)		//존재하지 않는 파일명이다
		printf("second open error for %s\n", argv[1]);

	else {
		fprintf(stderr, "it's very strange!\n");
		exit(1);
	}

	if ((fd = open(argv[2], O_RDONLY)) < 0) {
		fprintf(stderr, "third open error for %s\n", argv[2]);
		exit(1);
	}
	struct stat stbuf;

	stat(argv[2], &stbuf);

	printf("%d\n", stbuf.st_mtime);

	printf("Everything is good!\n");
	exit(0);
}


