#include "ssu_crond.h"

FILE *log_fp, *crontab_fp;
int min[60][1], hour[24][1], mday[32][1], month[13][1], wday[7][1];

int main(void)
{
	char *crontab_filename = "ssu_crontab_file";
	char buf[BUFFER_SIZE];
	char token[6][BUFFER_SIZE] = {0};

	if ((crontab_fp = fopen(crontab_filename, "a+")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", crontab_filename);
		exit(1);
	}

	while(fgets(buf, BUFFER_SIZE, crontab_fp) != NULL) {
		memset(token, 0, sizeof(token));
		command_separation(buf, token);

		make_timetable(token);

	}
	
	
	
	fclose(crontab_fp);

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
	printf("argc:%d\n", argc);
	for (int k = 0; k < argc+1; k++) {
		printf("argv[%d] : %s\n", k, token[k]);
	}

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
