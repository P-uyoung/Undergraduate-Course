//uid, gid 바꾸기
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
	struct stat statbuf;
	char *fname = "ssu_myfile";
	int fd;

	if ((fd = open(fname, O_RDWR | O_CREAT, 0600)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	close(fd);
	stat(fname, &statbuf);
	printf("# 1st stat call # UID:%d  GID:%d\n", statbuf.st_uid, statbuf.st_gid);	//UID :0  GID:0
	if (chown(fname, 501, 300) < 0) {		/* 소유자ID 501,  그룹사용자ID 300 으로  변경
											   cf) UserName 은 UserId로 관리되고 있는데, 
											       %id 치면 유저이름과 유저 아이디를 볼 수 있다 */
		fprintf(stderr, "chown error for %s\n", fname);
		exit(1);
	}

	stat(fname, &statbuf);		//변경된 파일의 정보를 statbuf에 담기
	printf("# 2nd stat call # UID:%d  GID:%d\n", statbuf.st_uid, statbuf.st_gid);

	if (unlink(fname) < 0) {		//해당 inode 삭제 >> 파일 삭제
		fprintf(stderr, "unlink error for %s\n", fname);
		exit(1);
	}

	exit(0);
}

			
