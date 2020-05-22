/* opendir()로 DIR 구조체를 얻고, 
이 구조체를 readdir()의 인자로 넣어 파일이름을 얻는다
파일 이름은 경로를 포함하지 않으므로 chdir() 호출
stat()을 통해, 일반파일인지 확인하고, 이름과 크기를 출력한다 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define DIRECTORY_SIZE MAXNAMLEN

int main(int argc, char *argv[])
{
	struct dirent *dentry;
	struct stat statbuf;
	char filename[DIRECTORY_SIZE+1];
	DIR *dirp;

	if (argv < 2) {
		fprintf(stderr, "usage : %s <directory>\n", argv[0]);
		exit(1);
	}

	if ((dirp = opendir(argv[1])) == NULL || chdir(argv[1]) == -1) {			//opendir(), chdir() : DIR 구조체를 얻음, chdir()로 작업디렉토리를 변경
		fprintf(stderr, "opendir, chdir error for %s\n", argv[1]);
		exit(1);
	}

	while ((dentry = readdir(dirp)) != NULL) {			//readdir() : dirent 구조체 얻음
		if (dentry -> d_ino == 0)	//삭제된 파일은 d_ino = 0 이 된다
			continue;

		memcpy(filename, dentry->d_name, DIRECTORY_SIZE);

		if (stat(filename, &statbuf) == -1) {
			fprintf(stderr, "stat error for %s\n", filename);
			break;
		}

		if ((statbuf.st_mode & S_IFMT) == S_IFREG)		//stat구조체의 파일 정보로, 일반파일인지 확인
				printf("%-14s %ld\n", filename, statbuf.st_size);

		else 
			printf("%-14s\n", filename);
	}


	exit(0);

}

