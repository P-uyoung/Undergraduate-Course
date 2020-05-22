/* umask 값에 따라, 파일모드가 다르게 creat 된다
umask 000 일 때, ssu_file1 파일 creat >> 666 모드
umask 066 일 때, ssu_file2 파일 creat >> 600 모드가 된다 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define RW_MODE 
(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
//파일모드 666 mode 상수 선언

int main(void)
{
	char *fname1 = "ssu_file1";
	char *fname2 = "ssu_file2";

	umask(0);	//%umask 000 (디폴트값 022)

	if (creat(fname1, RW_MODE) < 0) {
		fprintf(stderr, "creat error for %s\n", fname1);
		exit(1);	
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);	//%umask 066

	if (creat(fname2, RW_MODE) < 0) {
		fprintf(stderr, "creat error for %s\n", fname2);
		exit(1);
	}

	exit(0);
}


