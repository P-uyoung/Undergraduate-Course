#include "ssu_rsync.h"

char add_list[BUFFER_SIZE][BUFFER_SIZE];
char rm_list[BUFFER_SIZE][BUFFER_SIZE];
char cwd[BUFFER_SIZE];
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
		
		rsync(argv);

	
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

// rsc 파일일 때 동기화할 파일 찾는 함수
int rsync_file(char (*argv)[BUFFER_SIZE])
{
	char *tmp; // cwd pathname
	char str = argv[1];
	char *src_rltv = 0;
	struct dirent *dentry;
	struct stat stbuf, statbuf; // stbuf: src stat구조체, statbuf: dir stat구조체
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
	
	chdir(tmp); // 원래 cwd 이동하기
}

// rsc 디렉토리일 때 동기화할 파일 찾는 함수
int rsync_dir(char (*argv)[BUFFER_SIZE])
{	
	char *tmp; // cwd pathname
	struct dirent *src_dent, dst_dent;
	struct stat src_st, dst_st; 
	char src_fname[DIRECTORY_SIZE+1], dst_fname[DIRECTORY_SIZE+1];
	DIR *src_dirp, dst_dirp;
	int is_exist = 0;

	tmp = malloc(sizeof(char) *PATHMAX);
	getcwd(tmp, PATHMAX);

	if ((src_dirp = opendir(argv[1])) == NULL || chdir(argv[1]) == -1) {
		fprintf(stderr, "opendir, chdir error for %s\n", argv[1]);
		exit(1);
	}

	while ((src_dent = readdir(src_dirp))!= NULL) {
		if (dentry->d_ino == 0)
			continue;
		
		memcpy(src_fname, src_dent->d_name, DIRECTORY_SIZE);

		stat(src_fname, &src_st);

		if (!S_ISDIR(src_st.st_mode)) { // src 디렉토리 내 임의의 파일이
			
			if ((dst_dirp = opendir(argv[2])) == NULL || chdir(argv[2]) == -1) {
				fprintf(stderr, "opendir, chdir error for %s\n", argv[2]);
				exit(1);
			}

			while ((dst_dent = readdir(dst_dirp))!= NULL) { // dst 디렉토리 내 임의의 파일과 비교
				if (dentry->d_ino == 0)
					continue;

				memcpy(dst_fname, dst_dent->d_name, DIRECTORY_SIZE);

				stat(dst_fname, &dst_st);

				if (!S_ISDIR(dst_st.st_mode)) { 
					
					// src 디렉토리의 파일과 dst 디렉토리의 파일 비교
					if (!strcmp(src_fname, dst_fname)) { // 파일명이 같을 때
						if (src_st.st_size == dst_st.st_size &&
								src_st.st_mtime == dst_st.st_mtime)
							is_exist = 1;
						else {			    // 파일명만 같은 다른 파일일 때
							strcpy(rm_list[rm_count], src_fname);
							rm_count++;
							strcpy(add_list[add_count], src_fname);
							add_count++;
						}

					}
				}

				if (is_exist)
					break;
			}

			if (!is_exist) { // dst 디렉토리 내 동일한 파일이 없는 경우
				strcpy(add_list[add_count], src_fname);
				add_count++;
			}
		}

		is_exist = 0;
	
	} // src 디렉토리 탐색 끝
	
	chdir(tmp); // 원래 cwd 이동하기
}

// dst 디렉토리를 동기화하는 함수
void rsync(char (*argv)[BUFFER_SIZE])
{
	char *str;
	char buf;
	struct stat statbuf, swp_stat, copy_stat;
	FILE *fp, *swp_fp, *copy_fp;
	
	getcwd(cwd, BUFFER_SIZE);
	chdir(argv[2]);

	// 동기화 :삭제
	for (int i = 0; i < rm_count; i++ ) {
		
		strcpy(str, rm_list[i]);
		strcat(str, ".swp");
		
		if ((fp = fopen(rm_list[i], "r")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", rm_list[i]);
			exit(1);
		}

		if ((swp_fp = fopen(str, "w+")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", str);
			exit(1);
		}
		
		while(fgets(buf, BUFFER_SIZE, fp) != NULL)  // 파일 내용 .swp 파일에 복사
			fputs(buf, swp_fp);

		stat(rm_list[i], &statbuf);
		stat(str, &swp_stat);
		
		swp_stat.st_size = statbuf.st_size;
		swp_stat.st_mtime = swp_stat.st_mtime;
	
		remove(rm_list[i]); // 파일 삭제

		fclose(fp);
		fclose(swp_fp);
	}

	chdir(argv[1]);
	// 동기화 :추가
	for (int i = 0; i < add_count; i++) {

		if ((fp = fopen(add_list[i], "r")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", add_list[i]);
			exit(1);
		}
		char path; // add_list[i] 절대경로
		char *tmp; // add_list[i] 파일명
		char *temp; // dst 디렉토리내에 추가할 파일명(절대경로)
		char buf;

		realpath(add_list[i], path);
		tmp = path;
		tmp += strlen(path);

		while (*tmp != '/')
			tmp--;
		strcpy(temp, argv[2]);
		strcat(temp, tmp);
		printf("dir 동기화 추가할 파일명 절대경로.. %s\n", temp);

		if ((copy_fp = fopen(temp, "w+")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", temp);
			exit(1);
		}

		while(fgets(buf, BUFFER_SIZE, fp) != NULL)
			fputs(buf, copy_fp);

		stat(add_list[i], &statbuf);
		stat(str, &copy_stat);

		copy_stat.st_size = statbuf.st_size;
		copy_stat.st_mtime = statbuf.st_mtime;

		fclose(fp);
		fclose(copy_fp);
	}

	chdir(cwd);
	
	signal(SIGINT, ssu_signal_handler); // 핸들러 등록
}

// SIGINT 발생 시 동기화 중단하고 dst 디렉토리 원래대로 되돌리는 함수
void ssu_signal_handler(int signo)
{
	char buf;

	chdir(argv[2]);
	
	// 삭제된 파일 확인하기
	for (int i = 0; i < ; i++) {
		if (access(rm_list[i], F_OK) < 0) // 삭제된 경우
			
	}
//
// .swp 가 access 존재한다면,, 삭제!
// SIGINT 가 발생하지 않았다면, swp 파일 삭제
// 발생했다면, .swp에서 파일 새로 만들기 



// 추가된건 add_list[] 존재하는지 확인 후, 존재하면 삭제..





}
