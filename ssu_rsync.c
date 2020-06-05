#include "ssu_rsync.h"

char add_list[BUFFER_SIZE][BUFFER_SIZE];
char rm_list[BUFFER_SIZE][BUFFER_SIZE];
char cwd[BUFFER_SIZE], dst_path[BUFFER_SIZE];
time_t rsync_time; // 동기화 완료 시간
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
			printf("/* USAGE */\n");
			printf("ssu_rsync <src> <dst>\n");
			printf("<src> is file or directory.\n");
			printf("<dst> is directory.\n");
			exit(1);
		}
		
		rsync(argv); // 동기화 시작

		remove_swp(); // swp 파일 삭제

		write_log(argv); // 로그파일 작성
		
		exit(0);
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
		

			if(line[i] == '\n')
				break;

			argv[argc][j] = line[i];
		}
	}
}

// 입력 포맷 검사하는 함수
int check_argv(char (*argv)[BUFFER_SIZE])
{
	struct stat statbuf, stbuf;

	// argv[0] 입력 검사
	if(strcmp(argv[0],"ssu_rsync"))
		return -1;
	
	// argv[1] 입력 검사
	if (access(argv[1], F_OK) < 0) 
		return -1;

	if (access(argv[1], R_OK) < 0)
		return -1;
	stat(argv[1], &statbuf);

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
	char *ptr = argv[1];
	char *src_rltv = 0;
	struct dirent *dentry;
	struct stat stbuf, statbuf; // stbuf: src stat구조체, statbuf: dir stat구조체
	char fname[DIRECTORY_SIZE+1];
	DIR *dirp;
	int is_exist = 0;

	getcwd(cwd, PATHMAX);

	while(ptr != NULL)
	{
		ptr = strstr(ptr+1, "/");
		if (ptr != NULL)
			src_rltv = ptr;
	}
	printf("rsc 파일 상대경로명 : %s\n", src_rltv);
	stat(src_rltv, &stbuf);

	if ((dirp = opendir(argv[2])) == NULL) { 
		fprintf(stderr, "opendir error for %s\n", argv[2]);
		exit(1);
	}

	if ((chdir(argv[2])) == -1) {
		fprintf(stderr, "chdir error for %s\n", argv[2]);
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
	
	chdir(cwd); // 원래 cwd 이동하기
}

// rsc 디렉토리일 때 동기화할 파일 찾는 함수
int rsync_dir(char (*argv)[BUFFER_SIZE])
{	
	struct dirent *src_dent, *dst_dent;
	struct stat src_st, dst_st; 
	char src_fname[DIRECTORY_SIZE+1], dst_fname[DIRECTORY_SIZE+1];
	char src_file_path[PATHMAX], dst_file_path[PATHMAX];
	DIR *src_dirp, *dst_dirp;
	int is_exist = 0;

	getcwd(cwd, PATHMAX);

	if ((src_dirp = opendir(argv[1])) == NULL) {
		fprintf(stderr, "opendir error for %s\n", argv[1]);
		exit(1);
	}
	
	while ((src_dent = readdir(src_dirp))!= NULL) {
		if (src_dent->d_ino == 0)
			continue;
		
		memset(src_fname, 0, DIRECTORY_SIZE+1);
		memset(src_file_path, 0, PATHMAX);
		memcpy(src_fname, src_dent->d_name, DIRECTORY_SIZE);

		realpath(argv[1], src_file_path);
		strcat(src_file_path, "/");
		strcat(src_file_path, src_dent->d_name);

		if(stat(src_file_path, &src_st) < 0) {
			fprintf(stderr, "stat error for %s in %s\n", src_file_path, argv[1]);
			exit(1);
		};

		if (!S_ISDIR(src_st.st_mode)) { // src 디렉토리 내 임의의 파일이

			if ((dst_dirp = opendir(argv[2])) == NULL ) {
				fprintf(stderr, "opendir error for %s\n", argv[2]);
				exit(1);
			}

			while ((dst_dent = readdir(dst_dirp))!= NULL) { // dst 디렉토리 내 임의의 파일과 비교
				if (dst_dent->d_ino == 0)
					continue;
				
				memset(dst_fname, 0, DIRECTORY_SIZE+1);
				memset(dst_file_path, 0, PATHMAX);
				memcpy(dst_fname, dst_dent->d_name, DIRECTORY_SIZE);

				realpath(argv[2], dst_file_path);
			     	strcat(dst_file_path, "/");
				strcat(dst_file_path, dst_dent->d_name);	

				if(stat(dst_file_path, &dst_st) < 0) {
					fprintf(stderr, "stat error for %s in %s\n", dst_file_path, argv[2]);
					exit(1);
				};

				if (!S_ISDIR(dst_st.st_mode)) { 
					
					// src 디렉토리의 파일과 dst 디렉토리의 파일 비교
					if (!strcmp(src_fname, dst_fname)) { // 파일명이 같을 때
						printf("%s\n", src_fname);
						printf("src_size : dir_size = %ld : %ld\n",src_st.st_size, dst_st.st_size);
						printf("src_mtime : src_mtime = %ld : %ld\n", src_st.st_mtime, dst_st.st_mtime);
						
						if (src_st.st_size == dst_st.st_size &&
								src_st.st_mtime == dst_st.st_mtime)
							is_exist = 1;
						else {			    // 파일명만 같은 다른 파일일 때
							strcpy(rm_list[rm_count], src_fname);
							rm_count++;
							strcpy(add_list[add_count], src_fname);
							add_count++;
							is_exist = 1;
						}

					}
				}

				if (is_exist)
					break;
			} // dst 디렉토리 탐색 끝
			
			if (!is_exist) { // dst 디렉토리 내 동일한 파일이 없는 경우
				strcpy(add_list[add_count], src_fname);
				add_count++;
			}

			is_exist = 0;
			
			closedir(dst_dirp);
		}

	} // src 디렉토리 탐색 끝
	printf("rm_count:%d add_count:%d\n", rm_count, add_count);
	for (int i = 0; i <rm_count; i++)
		printf("rm_count[%d]:%s\n", i, rm_list[i]);	
	for (int i = 0; i <add_count; i++)
		printf("add_count[%d]:%s\n", i, add_list[i]);	
}

// dst 디렉토리를 동기화하는 함수
void rsync(char (*argv)[BUFFER_SIZE])
{
	char str[BUFFER_SIZE];
	char buf[BUFFER_SIZE];
	struct stat statbuf, swp_stat, copy_stat;
	FILE *fp, *swp_fp, *copy_fp;
	
	chdir(argv[2]);
	getcwd(dst_path, BUFFER_SIZE);

	// 동기화 :삭제
	for (int i = 0; i < rm_count; i++ ) {
		memset(str, 0, BUFFER_SIZE);
		
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

		if(stat(rm_list[i], &statbuf) < 0){
			fprintf(stderr, "stat error for %s in rm_list\n", rm_list[i]);
			exit(1);
		}
		if(stat(str, &swp_stat) < 0) {
			fprintf(stderr, "stat error for %s\n", str);
			exit(1);
		}
		
		swp_stat.st_size = statbuf.st_size;
		swp_stat.st_mtime = swp_stat.st_mtime;
	
		remove(rm_list[i]); // 파일 삭제

		fclose(fp);
		fclose(swp_fp);
		printf("파일 삭제 완료\n");
	}
	
	
	printf("add_count : %d\n", add_count);
	// 동기화 :추가
	for (int i = 0; i < add_count; i++) {
		chdir(cwd);
		chdir(argv[1]);

		if ((fp = fopen(add_list[i], "r")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", add_list[i]);
			exit(1);
		}
		
		if(stat(add_list[i], &statbuf) < 0) {
			fprintf(stderr, "stat error for %s in add_list\n", add_list[i]);
			exit(1);
		}
		
		chdir(cwd);
		chdir(argv[2]);

		if ((copy_fp = fopen(add_list[i], "w+")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", add_list[i]);
			exit(1);
		}

		while(fgets(buf, BUFFER_SIZE, fp) != NULL)
			fputs(buf, copy_fp);

		if(stat(add_list[i], &copy_stat) < 0) {
			fprintf(stderr, "stat error for %s in %s\n", add_list[i], argv[2]);
			exit(1);
		}

		printf("copy mtime : %ld\n", copy_stat.st_mtime);
		printf("addlist mtime : %ld\n", statbuf.st_mtime);
		copy_stat.st_size = statbuf.st_size;
		copy_stat.st_mtime = statbuf.st_mtime;
		printf("copy mtime : %ld\n", copy_stat.st_mtime);

		fclose(fp);
		fclose(copy_fp);
	}

	chdir(cwd);
	
	time(&rsync_time);

	signal(SIGINT, ssu_signal_handler); // 핸들러 등록

	printf(" 동기화는 완료\n");
}

// SIGINT 발생 시 동기화 중단하고 dst 디렉토리 원래대로 되돌리는 함수
void ssu_signal_handler(int signo)
{
	chdir(dst_path);
	
	// 삭제된 파일 되돌리기
	for (int i = 0; i < rm_count; i++) {
		if (access(rm_list[i], F_OK) < 0) { // 삭제된 경우
			char swp[BUFFER_SIZE]; // .swp 파일명
			strcpy(swp, rm_list[i]);
			strcat(swp, ".swp");
			if (access(swp, F_OK) == 0) 
				rename(swp, rm_list[i]);									
		}
	}

	// 복사된 파일 삭제하기
	for (int i = 0; i < add_count; i++) {
		if (access(add_list[i], F_OK) > 0)  // 복사한 파일이 있다면
			remove(add_list[i]);		
	}
	
	exit(EXIT_SUCCESS);
}	

// swp 파일을 삭제하는 함수
void remove_swp(void)
{
	chdir(dst_path);
	
	for (int i = 0; i <rm_count; i++) {
		char swp[BUFFER_SIZE]; // .swp 파일명
		strcpy(swp, rm_list[i]);
		strcat(swp, ".swp");
		if (access(swp, F_OK) == 0)
			remove(swp);
	}	
	
	chdir(cwd);
}

// 동기화 완료시 로그를 쓰는 함수
void write_log(char (*argv)[BUFFER_SIZE])
{
	char buf[TIME_LEN], tmp[BUFFER_SIZE];
	FILE *fp;
	
	if ((fp = fopen("ssu_rsync_log", "a")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", "ssu_rsync_log");
		exit(1);
	}
	
	strncpy(buf, ctime(&rsync_time), TIME_LEN);
	buf[TIME_LEN-2]=0;

	fprintf(fp,"[%s] ssu_rsync %s %s\n", buf, argv[1], argv[2]);
	
	for (int i = 0; i < rm_count; i++) 
		fprintf(fp, "\t%s delete\n", rm_list[i]); 

	chdir(argv[2]);
	for (int i = 0; i < add_count; i++) {
		struct stat statbuf;

		stat(add_list[i], &statbuf);
		fprintf(fp, "\t%s %ldbytes\n", add_list[i], statbuf.st_size);
	}

	printf(" 로그 완료\n");
}
