/* ssu_file1 비트연산자로 setuid 설정하고, 그룹의 실행 권한을 해제한다
ssu_file2 645 모드로 파일모드 변경  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	struct stat statbuf;
	char *fname1 = "ssu_file1";
	char *fname2 = "ssu_file2";

	if (stat(fname1, &statbuf) < 0)
	       	fprintf(stderr, "stat error %s\n", fname1);

	//set-user-ID를 설정하고, 그룹의 실행 권한을 해제한다
	if (chmod(fname1, (statbuf.st_mode & ~S_IXGRP) | S_ISUID) < 0)	/* % chmod 4644 fname1
																	   % chmod g-w fname1	
																	   & 연산: st_mode 값은 바뀌지 않고, 뽑아낸다 */
		
		fprintf(stderr, "chmoderror %s\n", fname1);
		

	//645 모드로 파일모드 변경
	if (chmod(fname2, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH) < 0)		//% chmod 654 fname2
		fprintf(stderr, "chmoderror %s\n", fname2);

	exit(0);
}