/* ssu_hole.txt (인자로 받을) 파일의 정보를 읽어 파일의 변경 여부를 확인한다.
st_mtime 필드 이용 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

struct stat statbuf;

void ssu_checkfile(char *fname, time_t *time);	//파일이 변경되면, 경고문을 뿌린다

int main(int argc, char *argv[])
{
	time_t intertime;	//최종수정시간 변수

	if (argc < 2) {
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	if (stat(argv[1], &statbuf) < 0) {
		fprintf(stderr, "stat error for %s\n", argv[1]);
		exit(1);
	}

	intertime = statbuf.st_mtime;	//stat구조체의 st_mtime 필드 할당
	
	while (1) {
		ssu_checkfile(argv[1], &intertime);
		sleep(10);
	}
}

//ssu_checkfile 구현부
void ssu_checkfile (char *fname, time_t *time) {	
	if (stat(fname, &statbuf) < 0) {
		fprintf(stderr, "Warning : ssu_checkfile() error\n");	//파일이 없을때
		exit(1);
	}

	else
		if(statbuf.st_mtime != *time) {		//파일 정보가 변경되었을 때
		       	printf("Warning : %s was modified!.\n", fname);
			*time = statbuf.st_mtime;
		}
}

