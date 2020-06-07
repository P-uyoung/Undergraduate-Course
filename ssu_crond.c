#include "ssu_crond.h"

FILE *log_fp, *crontab_fp;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int min[60][1], hour[24][1], mday[32][1], month[13][1], wday[7][1]; // 명령어 TIME TABLE
char exe_period[BUFFER_SIZE], command[BUFFER_SIZE]; // log 파일에 기록할 실행주기와 명령어

int main(void)
{
	pthread_t tid;
	struct timeval begin_t, end_t;
	char *crontab_filename = "ssu_crontab_file";
	char buf[BUFFER_SIZE];
	char token[6][BUFFER_SIZE] = {0};

	gettimeofday(&begin_t, NULL);

	if ((crontab_fp = fopen(crontab_filename, "a+")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", crontab_filename);
		exit(1);
	}

	while (1) {
		while(fgets(buf, BUFFER_SIZE, crontab_fp) != NULL) {
			memset(token, 0, sizeof(token));
			command_separation(buf, token);	// 실행주기와 명령어를 분리

			make_timetable(token); // 타임테이블 생성

			if(pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
				fprintf(stderr, "pthread_create error\n");
				exit(1);
			}

			pthread_mutex_destroy(&mutex);

		}
		fclose(crontab_fp);
		
		sleep(60); // 1 분 간격 반복검사
	}

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);

	exit(0);
}

// 반복 명령어를 수행하는 쓰레드 함수
void *ssu_thread(void *arg)
{
	time_t rawtime;
	struct tm *tm;
	char tmp[TIME_LEN];

	time(&rawtime);
	tm = localtime(&rawtime);

	if (min[tm->tm_min][0]) 
		if (hour[tm->tm_hour][0])
			if (mday[tm->tm_mday][0])
				if (month[tm->tm_mon][0])
					if (wday[tm->tm_wday][0]) { // 실행주기가 현재시간과 맞다면
						pthread_mutex_lock(&mutex);

						system(command);

						if ((log_fp = fopen("ssu_crontab_log", "a+")) == NULL) {
							fprintf(stderr, "fopen error for ssu_crontab_log\n");
							exit(1);
						}

						strncpy(tmp, ctime(&rawtime), TIME_LEN);
						tmp[TIME_LEN-2] = 0;

						char buf[BUFFER_SIZE];
						sprintf(buf, "[%s] run %s ", tmp, exe_period);
						fputs(buf, log_fp);
						fputs(command, log_fp);

						pthread_mutex_unlock(&mutex);
					}
	
	return NULL;
}

// 명령어 분리하는 함수
void command_separation(char *line, char (*token)[BUFFER_SIZE]) 
{
        int i, j;
	int argc = 0;
        int count  = (int)strlen(line);

        for (i = 0; i < count; i++) {
		if (i > 2) {
			for (j = 0;;j++,i++) {
				if (argc != 5) {
					if(line[i] == ' ') {
						token[argc][j] = '\0';
						argc++;
						break;
					}
				}

				if (line[i] == '\n')
					break;

				token[argc][j] = line[i];
			}
		}
	}
	
	strcpy(exe_period, token[0]); // log 파일에 기록할 실행주기
	for (i = 1; i < 5; i++)
		strcat(exe_period, token[i]);
	strcpy(command, token[5]); // log 파일에 기록할 명령어
/*	printf("%s\n", exec_period);
 	printf("%s\n", command);
	
	printf("argc:%d\n", argc);
	for (int k = 0; k < argc+1; k++) {
		printf("token[%d] : %s\n", k, token[k]);
	}
*/

}

// 실행주기 테이블 만드는 함수
void make_timetable(char (*token)[BUFFER_SIZE])
{
	char *ptr;
	char *start, *end;
	char front[10], back[10];
	char buf[100][30] = {0};
	int i = 0;

	memset(min, 0, sizeof(min));
	memset(hour, 0, sizeof(hour));
	memset(mday, 0, sizeof(mday));
	memset(month, 0, sizeof(month));
	memset(wday, 0, sizeof(wday));
	
	/* MIN TIME TABLE 생성 */
	memset(buf, 0, sizeof(buf));

	// 쉼표로 토큰화하기
	ptr = strtok(token[0], ",");

	while (ptr != NULL) {
		strcpy(buf[i], ptr);
		i++;
		ptr = strtok(NULL, ",");
	}

	// 주기로 토큰화하기
	for (int j = 0; j < i; j++) {
		ptr = strtok(buf[j], "/");
		strcpy(front, ptr);
		if((ptr = strtok(NULL, "/")) == NULL) { 	// 주기가 없을 때
			start = strtok(buf[j], "-");	
			if((end = strtok(NULL, "-")) == NULL) { // _범위가 없을 때
				if(!strcmp(buf[j],"*")){	// _ _'*' 일 때
					for (int k = 0; k < 60; k++)
						min[k][0] = 1;
				}
				else{				// _ _숫자일 때
					min[atoi(buf[j])][0] = 1;
				}
			}

			else {					// _범위가 있을 때
				for (int k = atoi(start); k <= atoi(end); k++) 
					min[k][0] = 1;
			}	
		}

		else {
			strcpy(back, ptr);  			// 주기가 있을 때

			if (!strcmp(front, "*")) {  		// _범위가 없을 때('*')
				for (int k = 0; k < 60; k++)
					if (k%atoi(back) == 0)
						min[k][0] = 1;
			}

			else {
				start = strtok(front, "-"); 	// _범위가 있을 때
				end = strtok(NULL,"-");
				for (int k = atoi(start); k <= atoi(end); k++)
					if ((k-atoi(start))%atoi(back) == 0)
						min[k][0] = 1;
			}	
		}
	}
	
 	/* HOUR TIME TABLE 생성 */
	i = 0;
	memset(buf, 0, sizeof(buf));

	// 쉼표로 토큰화하기
	ptr = strtok(token[1], ",");

	while (ptr != NULL) {
		strcpy(buf[i], ptr);
		i++;
		ptr = strtok(NULL, ",");
	}


	// 주기로 토큰화하기
	for (int j = 0; j < i; j++) {
		ptr = strtok(buf[j], "/");
		strcpy(front, ptr);
		if((ptr = strtok(NULL, "/")) == NULL) { 	// 주기가 없을 때
			start = strtok(buf[j], "-");	
			if((end = strtok(NULL, "-")) == NULL) { // _범위가 없을 때
				if(!strcmp(buf[j],"*")){	// _ _'*' 일 때
					for (int k = 0; k < 24; k++)
						hour[k][0] = 1;
				}
				else{				// _ _숫자일 때
					hour[atoi(buf[j])][0] = 1;
				}
			}

			else {					// _범위가 있을 때
				for (int k = atoi(start); k <= atoi(end); k++) 
					hour[k][0] = 1;
			}	
		}

		else {
			strcpy(back, ptr);  			// 주기가 있을 때

			if (!strcmp(front, "*")) {  		// _범위가 없을 때('*')
				for (int k = 0; k < 24; k++)
					if (k%atoi(back) == 0)
						hour[k][0] = 1;
			}

			else {
				start = strtok(front, "-"); 	// _범위가 있을 때
				end = strtok(NULL,"-");
				for (int k = atoi(start); k <= atoi(end); k++)
					if ((k-atoi(start))%atoi(back) == 0)
						hour[k][0] = 1;
			}	
		}
	}
	
	
	/* MDAY TIME TABLE 생성 */
	i = 0;
	memset(buf, 0, sizeof(buf));

	// 쉼표로 토큰화하기
	ptr = strtok(token[2], ",");

	while (ptr != NULL) {
		strcpy(buf[i], ptr);
		i++;
		ptr = strtok(NULL, ",");
	}

	// 주기로 토큰화하기
	for (int j = 0; j < i; j++) {
		ptr = strtok(buf[j], "/");
		strcpy(front, ptr);
		if((ptr = strtok(NULL, "/")) == NULL) { 	// 주기가 없을 때
			start = strtok(buf[j], "-");	
			if((end = strtok(NULL, "-")) == NULL) { // _범위가 없을 때
				if(!strcmp(buf[j],"*")){	// _ _'*' 일 때
					for (int k = 1; k < 32; k++)
						mday[k][0] = 1;
				}
				else{				// _ _숫자일 때
					mday[atoi(buf[j])][0] = 1;
				}
			}

			else {					// _범위가 있을 때
				for (int k = atoi(start); k <= atoi(end); k++) 
					mday[k][0] = 1;
			}	
		}

		else {
			strcpy(back, ptr);  			// 주기가 있을 때

			if (!strcmp(front, "*")) {  		// _범위가 없을 때('*')
				for (int k = 1; k < 32; k++)
					if (k%atoi(back) == 0)
						mday[k][0] = 1;
			}

			else {
				start = strtok(front, "-"); 	// _범위가 있을 때
				end = strtok(NULL,"-");
				for (int k = atoi(start); k <= atoi(end); k++)
					if ((k-atoi(start))%atoi(back) == 0)
						mday[k][0] = 1;
			}	
		}
	}
	
	/* MONTH TIME TABLE 생성 */
	i = 0;
	memset(buf, 0, sizeof(buf));

	// 쉼표로 토큰화하기
	ptr = strtok(token[3], ",");

	while (ptr != NULL) {
		strcpy(buf[i], ptr);
		i++;
		ptr = strtok(NULL, ",");
	}

	// 주기로 토큰화하기
	for (int j = 0; j < i; j++) {
		ptr = strtok(buf[j], "/");
		strcpy(front, ptr);
		if((ptr = strtok(NULL, "/")) == NULL) { 	// 주기가 없을 때
			start = strtok(buf[j], "-");	
			if((end = strtok(NULL, "-")) == NULL) { // _범위가 없을 때
				if(!strcmp(buf[j],"*")){	// _ _'*' 일 때
					for (int k = 1; k < 13; k++)
						month[k][0] = 1;
				}
				else{				// _ _숫자일 때
					month[atoi(buf[j])][0] = 1;
				}
			}

			else {					// _범위가 있을 때
				for (int k = atoi(start); k <= atoi(end); k++) 
					month[k][0] = 1;
			}	
		}

		else {
			strcpy(back, ptr);  			// 주기가 있을 때

			if (!strcmp(front, "*")) {  		// _범위가 없을 때('*')
				for (int k = 1; k < 13; k++)
					if (k%atoi(back) == 0)
						month[k][0] = 1;
			}

			else {
				start = strtok(front, "-"); 	// _범위가 있을 때
				end = strtok(NULL,"-");
				for (int k = atoi(start); k <= atoi(end); k++)
					if ((k-atoi(start))%atoi(back) == 0)
						month[k][0] = 1;
			}	
		}
	}
	
	/* WDAY TIME TABLE 생성 */
	i = 0;
	memset(buf, 0, sizeof(buf));

	// 쉼표로 토큰화하기
	ptr = strtok(token[4], ",");

	while (ptr != NULL) {
		strcpy(buf[i], ptr);
		i++;
		ptr = strtok(NULL, ",");
	}

	// 주기로 토큰화하기
	for (int j = 0; j < i; j++) {
		ptr = strtok(buf[j], "/");
		strcpy(front, ptr);
		if((ptr = strtok(NULL, "/")) == NULL) { 	// 주기가 없을 때
			start = strtok(buf[j], "-");	
			if((end = strtok(NULL, "-")) == NULL) { // _범위가 없을 때
				if(!strcmp(buf[j],"*")){	// _ _'*' 일 때
					for (int k = 0; k < 7; k++)
						wday[k][0] = 1;
				}
				else{				// _ _숫자일 때
					wday[atoi(buf[j])][0] = 1;
				}
			}

			else {					// _범위가 있을 때
				for (int k = atoi(start); k <= atoi(end); k++) 
					wday[k][0] = 1;
			}	
		}

		else {
			strcpy(back, ptr);  			// 주기가 있을 때

			if (!strcmp(front, "*")) {  		// _범위가 없을 때('*')
				for (int k = 0; k < 7; k++)
					if (k%atoi(back) == 0)
						wday[k][0] = 1;
			}

			else {
				start = strtok(front, "-"); 	// _범위가 있을 때
				end = strtok(NULL,"-");
				for (int k = atoi(start); k <= atoi(end); k++)
					if ((k-atoi(start))%atoi(back) == 0)
						wday[k][0] = 1;
			}	
		}
	}
	
	/*
	for (int j = 0; j <60; j++)
		printf("%d 분: %d\n",j, min[j][0]);	
	for (int j = 0; j <24; j++)
		printf("%d 시: %d\n",j, hour[j][0]);	
	for (int j = 1; j <32; j++)
		printf("%d 일: %d\n",j, mday[j][0]);	
	for (int j = 1; j <13; j++)
		printf("%d 월: %d\n",j, month[j][0]);	
	for (int j = 0; j <7; j++)
		printf("%d 요일: %d\n",j, wday[j][0]);	
	*/
}

// 실행시간을 출력하는 함수
void ssu_runtime(struct timeval *begin_t, struct timeval *end_t)
{
	end_t->tv_sec -= begin_t->tv_sec;

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t->tv_sec--;
		end_t->tv_usec += SECOND_TO_MICRO;
	}

	end_t->tv_usec -= begin_t->tv_usec;
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);
}

