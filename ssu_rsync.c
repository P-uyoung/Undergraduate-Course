#include "ssu_rsync.h"

char add_list[BUFFER_SIZE][BUFFER_SIZE];
char rm_list[BUFFER_SIZE][BUFFER_SIZE];
int add_count, rm_count; // rsync 추가, 삭제해야 할 개수

int main(void)
{
	while(1)
	{
		char line[BUFFER_SIZE];  //command
		char argv[3][BUFFER_SIZE] = {0};
		
		memset(argv, 0, sizeof(argv));

		fgets(line, sizeof(line), stdin);

		command_separation(line, argv); // 명령어 분리

		if(check_argv(argv) < 0) { // src dst 입력 포맷 검사
			print("/* USAGE */\n");
			printf("rsync <src> <dst>\n");
			printf("<src> is file or directory.\n");
			printf("<dst> is directory.\n");
			exit(1);
		}
		
		rsync();

	
	}


}

// 명령어를 분리하는 함수
void command_separation(char *line, char (*argv)[BUFFER_SIZE])
{
	int argc = 0;
	
	for(int i = 0; i < (int)strlen(line) ; i++)
	{
		for(int j = 0;;j++, i++)
		{

			if (line[i] == ' ')
			{
				argv[argc][j] = '\0';
				argc++;
				break;
			}
		}

		if(line[i] == '\n')
			break;

		argv[argc][j] = line[i];
	}
}

// 입력 포맷 검사하는 함수
int check_argv(char (*argv)[BUFFER_SIZE])
{
	struct stat statbuf, stbuf;

	// argv[0] 입력 검사
	if(argv[0] != "ssu_rysnc")
		return -1;
	
	// argv[1] 입력 검사
	if (access(argv[1], F_OK) < 0) 
		return -1;

	if (access(argv[1], R_OK) < 0)
		return -1;
	lstat(argv[1], &statbuf);

	if (S_ISDIR(statbuf.st_mode))
		rsync_dir(argv);
	else 
		rsync_file(argv);
	
	// argv[2] 입력 검사
	if (access(argv[2], F_OK) < 0)
		return -1;

	if (access(argv[2], R_OK) < 0)
		return -1;

	if (access(argv[2], W_OK) < 0)
		return -1;

	stat(argv[2], &stbuf);

	if (!S_ISDIR(stbuf.st_mode))
		return -1;
}

// rsc 파일일 때 동기화하는 함수
int rsync_file(char (*argv)[BUFFER_SIZE])
{
	char *tmp; // cwd pathname
	char str = argv[1];
	char *src_rltv = 0;
	struct dirent *dentry;
	struct stat stbuf, statbuf; //stbuf: src stat구조체, statbuf: dir stat구조체
	char fname[DIRECTORY_SIZE+1];
	DIR *dirp;
	int is_exist = 0;

	tmp = malloc(sizeof(char) *PATHMAX);
	getcwd(tmp, PATHMAX);

	while(ptr != NULL)
	{
		ptr = strstr(ptr+1, "/");
		if (ptr != NULL)
			src_rltv = ptr;
	}
	printf("rsc 파일 상대경로명 : %s\n", src_rltv);
	stat(src_rltv, &stbuf);

	if ((dirp = opendir(argv[2])) == NULL || chdir(argv[2]) == -1) {
		fprintf(stderr, "opendir, chdir error for %s\n", argv[2]);
		exit(1);
	}

	while ((dentry = readdir(dirp))!= NULL) {
		if (dentry->d_ino == 0)
			continue;
		
		memcpy(fname, dentry->d_name, DIRECTORY_SIZE);

		stat(fname, &statbuf);

		if (!S_ISDIR(statbuf.st_mode)) { // dst 디렉토리 내 임의의 파일이
			if(!strcmp(fname, src_rltv)) { // 파일명이 같을 때
				if(statbuf.st_size == stbuf.st_size &&
					       statbuf.st_mtime == stbuf.st_mtime)
					is_exist = 1;	// 동일한 파일이 있을 때
				else {			// 이름만 같은 파일이 있을 때
					strcpy(rm_list[rm_count], fname);
					rm_count++;
					strcpy(add_list[add_count], fname);	
					add_count++;
				}	 
			}	
		}

		if (is_exist)
			break;
	}

	if (!is_exist) { // dst 디렉토리 내 동일한 파일이 없는 경우
		strcpy(add_list[add_count], fname);
		add_count++;
	}

}

// rsc 디렉토리일 때 동기화하는 함수
int rsync_dir(char (*argv)[BUFFER_SIZE])
{
	


}


void rsync(void)
{
// 동기화 시작시간 구하기
// 동기화 
// 핸들러 등록
// 핸들러... 동기화 시작시간 후에 mtime 인거 지우기
// 핸들러... 삭제된 거 되돌리기..

}


