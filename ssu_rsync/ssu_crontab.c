#include "ssu_crontab.h"

FILE *crontab_fp;	// "ssu_crontab_file" 구조체 포인터
FILE *log_fp;		// "ssu_crontab_log" 구조체 포인터
int line_counter;

int main(void)
{
//	pid_t pid;
//	pid_t cpid;

	struct timeval begin_t, end_t;
	char buf[BUFFER_SIZE];
	int status;

	gettimeofday(&begin_t, NULL);

/*	if ((pid = fork()) < 0)
	{
		printf("fork error\n");
		exit(1);
	}
	
	else if (pid == 0)  // 자식 프로세스
	{
		if (cpid = ssu_daemon_init() < 0) {
			fprintf(stderr, "ssu_daemon_init failed\n");
			exit(1);
		}
		printf("cpid = %d\n", cpid);
		
	}
	wait(&status); 	// 자식프로세스의 종료상태 회수
*/	

	if ((crontab_fp = fopen("ssu_crontab_file", "a+")) == NULL) {
			fprintf(stderr, "open error for ssu_crontab_file\n");
			exit(1);
		}
	
	while(fgets(buf, BUFFER_SIZE, crontab_fp) != NULL)
		line_counter++;
	
	fclose(crontab_fp);
	
	while (1)
	{
		char c, character;
		char line[BUFFER_SIZE];	// command
		char buf[BUFFER_SIZE];
		char argv[7][BUFFER_SIZE] = { 0 };	// memset 을 위해, 6+1
		int argc = 0;

		argc = 0;
		memset(argv, 0, sizeof(argv));

		if ((crontab_fp = fopen("ssu_crontab_file", "a+")) == NULL) {
			fprintf(stderr, "open error for ssu_crontab_file\n");
			exit(1);
		}
		
		if ((log_fp = fopen("ssu_crontab_log", "a+")) == NULL) {
			fprintf(stderr, "open error for ssu_crontab_log\n");
			exit(1);
		}
		
		// 저장된 모든 명령어 출력
		while (fgets(buf, BUFFER_SIZE, crontab_fp) != NULL) 
			fputs(buf, stdout);
		
		printf("\n");
		printf("20162365> ");
		if((fgets(line, sizeof(line), stdin)) == NULL) {
			printf("/* USAGE */\n");
			printf("*ADD [실행주기] [명령어]\n");
			printf("*REMOVE [COMMAND_NUMBER]\n");
			printf("*EXIT\n");
			printf("\n");
			
			continue;
		}
		
		argc = command_separation(line, argc, argv);    // 명령어 분리 (argc 0부터 카운트)
		
/*		printf("argc = %d\n", argc);
		for (int j = 0; j < argc+1; j++)
			printf("argv[%d] = %s\n",j, argv[j]);
*/
		if (!strcmp(argv[0], "add") || !strcmp(argv[0], "ADD"))
		{
			if (argc != 6) {  // 실행주기 항목이 5가지가 아닌 경우, 예외처리
				printf("/* ADD 명령어 인자 부족합니다 */\n");
				printf("\n");
				continue;
			}

			if (opt_check(argv) == -1) { // ADD 항목 입력 범위 오류
				printf("/* ADD 명령어 항목 범위 오류 */\n");
				printf("*범위: min(0-59), hour(0-23), 일(1-31), 월(1-12), 요일(0-6)\n");
				printf("\n");
				
				continue;	
			}

			else if (opt_check(argv) == -2) { // ADD 항목 입력 기호 오류
				printf("/* ADD 명령어 항목 기호 오류 */\n");
				printf("*기호: '*', '숫자-숫자', '숫자,숫자', '범위/주기'\n");
				printf("\n");
				
				continue;	
			
			}

			add_command(argv);	// 명령어 "ssu_crontab_file" 파일에 추가

		}
	
		else if (!strcmp(argv[0], "remove") || !strcmp(argv[0], "REMOVE"))
		{
			if (argc != 1) {  // COMMAND_NUMBER 인자가 없는 경우
				printf("/* REMOVE 명령어 인자 하나입니다 */\n");
				printf("\n");

				continue;
			}

			if (number_check(argv) == -1) {
				printf("/* REMOVE 명령어 항목 입력 오류 */\n");
				printf("\n");

				continue;
			}

			remove_command(argv);	// 명령어 "ssu_crontab_file" 파일에서 삭제
		}

		else if (!strcmp(argv[0], "exit") || !strcmp(argv[0], "EXIT"))
		{
			break;
		}

		else {
			printf("/* USAGE */\n");
			printf("*ADD [실행주기] [명령어]\n");
			printf("*REMOVE [COMMAND_NUMBER]\n");
			printf("*EXIT\n");
			printf("\n");
		}
		
		fclose(crontab_fp);
		fclose(log_fp);
	}
	
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);

	exit(0);

}

// 명령어를 분리하는 함수
int command_separation(char *line, int argc, char (*argv)[BUFFER_SIZE])  // 2차원배열의 포인터
{
	int i, j;
	int count  = (int)strlen(line);
	
	for (i = 0; i < count; i++) {
		for(j = 0;;j++, i++)
		{
			if (argc != 6) {
				if (line[i] == ' ')
				{
					argv[argc][j] = 0;
					argc++;
					break;
				}
			}
			
			if(line[i] == '\n')
				break;

			argv[argc][j] = line[i];
		}
	}

//	for (int k = 0; k < argc+1; k++)
//		printf("argv[%d]:%s\n",argc, argv[k]);	
	return argc;
}	

// 실행주기 MIN(분)의 범위 검사하는 함수
int min_scope_check(char (*argv)[BUFFER_SIZE], int i)	// argv[1][i] 의 범위 검사
{
//	for (int i= 0; i < strlen(argv[1]); i++)
//		printf("argv[1][%d]:%d\n", i, argv[1][i]);

	if (argv[1][i] == 48) {	// min:0
		if (argv[1][i+1] >= 48 && argv[1][i+1] <= 57)
			return -1;
	}		
	
	// min 범위:1-5, 10-59
		
	if (argv[1][i] > 53 && argv[1][i] <= 57) {	// min 범위:6-9
		if (argv[1][i+1] >= 48 && argv[1][i+1] <= 57)
			return -1;
	}
}

// 실행주기 HOUR(시간)의 범위를 검사하는 함수
int hour_scope_check(char (*argv)[BUFFER_SIZE], int i)	// argv[2][i]의 범위 검사
{
	if (argv[2][i] == 48) {	// hour:0
		if (argv[2][i+1] >= 48 && argv[2][i+1] <= 57)
			return -1;
	}		
	
	// hour 범위:1, 10-19 
		
	if (argv[2][i] == 50) {	// hour 범위:2, 20-23
		if (argv[2][i+1] > 51 && argv[2][i+1] <= 57) 
			return -1;
	}
	
	if (argv[2][i] >= 51 && argv[2][i] <= 57) { // hour 범위:3-9
		if (argv[2][i+1] >= 48 && argv[2][i+1] <= 57)
			return -1;
	}
}

// 실행주기 DAY(일)의 범위를 검사하는 함수
int day_scope_check(char (*argv)[BUFFER_SIZE], int i)	// argv[3][i]의 범위 검사
{
	if (argv[3][i] == 48)
		return -1;

	// day 범위:1-2, 10-29 
	
	if (argv[3][i] == 51) {	// day 범위:3, 30-31
		if (argv[3][i+1] >= 50 && argv[3][i+1] <= 57) 
			return -1;
	}
	
	if (argv[3][i] >= 52 && argv[3][i] <= 57) { // day 범위:4-9
		if (argv[3][i+1] >= 48 && argv[3][i+1] <= 57) 
			return -1;
	}
}

// 실행주기 MONTH(월)의 범위를 검사하는 함수
int month_scope_check(char (*argv)[BUFFER_SIZE], int i)	// argv[4][i]의 범위 검사
{
	if (argv[4][i] == 48)
		return -1;

	if (argv[4][i] == 49) {	// month 범위:1, 10-12
		if (argv[4][i+1] >= 51 && argv[4][i+1] <= 57) 
			return -1;
	}	
	if (argv[4][i] >= 50 && argv[4][i] <= 57) { // month 범위:2-9
		if (argv[4][i+1] >= 48 && argv[4][i+1] <= 57) 
			return -1;
	}
}

// 실행주기 WEEK(요일)의 범위를 검사하는 함수
int week_scope_check(char (*argv)[BUFFER_SIZE], int i)	// argv[5][i]의 범위 검사
{
	if (argv[5][i] >= 55 && argv[5][i] <= 57)
		return -1;
	
	if (argv[5][i] >= 48 && argv[5][i] <= 54) {	// week 범위:0-6
		if (argv[5][i+1] >= 48 && argv[5][i+1] <= 57) 	
			return -1;
	}
}

// 실행주기 기호 검사하고 분리하는 함수
int opt_check(char (*argv)[BUFFER_SIZE])
{
	if(min_scope_check(argv,0) < 0) return -1;
	if(hour_scope_check(argv, 0) <0) return -1;
	if(day_scope_check(argv, 0) < 0) return -1;
	if(month_scope_check(argv, 0) <0) return -1;
	if(week_scope_check(argv,0) <0) return -1;
	
	for (int j = 1; j < 6; j++) { // min, hour, day, month, week 차례대로 검사 및 분리
		for (int i = 0; i < strlen(argv[j]); i++) {
			//printf("argv[%d][%d] = %d\n", j,i, argv[j][i]);
			if (argv[j][i] == '/' || argv[j][i] == ',' || argv[j][i] == '-') {
				if (!(argv[j][i+1] >= 48 && argv[j][i+1] <= 57))  // 기호 뒤에 숫자가 아닐 때
					return -2;
				else {
					switch (j) {
						case 1 :
							if(min_scope_check(argv, i+1) < 0) return -1;
							break;
						case 2 :
							if(hour_scope_check(argv, i+1) <0) return -1;
							break;
						case 3 :
							if(day_scope_check(argv, i+1) <0) return -1;
							break;
						case 4 :
							if(month_scope_check(argv, i+1) <0) return -1;
							break;
						case 5 :
							if(week_scope_check(argv, i+1) <0) return -1;
							break;
						default :
							exit(1);
					}
				}
			}
				
			else if (argv[j][i] != '*' && (argv[j][i] < 48 || argv[j][i] > 57)) // '*','-',',','/' 이외의 기호
				return -2;
			
		}	
	}
}

// REMOVE 명령어의 인자를 검사하는 함수
int number_check(char (*argv)[BUFFER_SIZE])
{
	if (!(argv[1][0] >= 48 && argv[1][0] <= 57)) // 숫자입력이 아닌 경우
		return -1;

	if ((atoi(argv[1]) - 1) > line_counter) // 없는 번호일 경우
		return -1;
}

// "ssu_crontab_file" 파일에 명령어 ADD 하는 함수
void add_command(char (*argv)[BUFFER_SIZE])
{
	char buf[BUFFER_SIZE];
	
	memset(buf, 0, BUFFER_SIZE);

	sprintf(buf, "%d. ", line_counter);
	fputs(buf,crontab_fp);
	
	for (int i = 1; i < 6; i++) {
		fputs(argv[i],crontab_fp);
		fputc(' ', crontab_fp);
	}
	
	fputs(argv[6],crontab_fp);
	fputc('\n',crontab_fp);
	line_counter++;
	
	write_add_log(argv);	
}

// "ssu_crontab_file" 파일에 명령어 REMOVE 하는 함수
void remove_command(char (*argv)[BUFFER_SIZE])
{
	char copy[BUFFER_SIZE][BUFFER_SIZE];	// 새로 오픈한 crontab_fp 에 복사할 버퍼
	char buf[BUFFER_SIZE];
	char remove_command[BUFFER_SIZE];
	char temp[BUFFER_SIZE] = {0};	
	char *str;	// 인덱스를 변경하기 위한 버퍼
	int size;
	int count = 0;
	int number = atoi(argv[1]); 	// 삭제할 명령어 인덱스 (0 부터 시작)

//	printf("삭제인덱스 : %d\n", number);
	memset(copy, 0, sizeof(copy));
	memset(temp, 0, BUFFER_SIZE);

	fseek(crontab_fp, 0, SEEK_SET);
	while (fgets(buf, BUFFER_SIZE, crontab_fp) != NULL) {
		if (count < number){	// 입력숫자 이전 명령어
			strcpy(copy[count],buf);	// 그대로 복사
//			printf("count < number 인 경우\n");
//			printf("%s", copy[count]);
			
		}
		else if (count == number) {
			for (int i = 3; i < strlen(buf); i++)
				temp[i-3] = buf[i];
			strcpy(remove_command, "remove ");
			strcat(remove_command, temp);
		}
		
		else if (count > number) {  // 입력숫자 이후 명령어
			str = buf;
			*str = count + 47; // 인덱스 수 count -1 + 48
			strcpy(copy[count-1], buf);
//			printf("count > number 인 경우\n");
//			printf("%s", copy[count-1]);
		}
		count++;	
	}
	
	fclose(crontab_fp);

	line_counter--;

	if((crontab_fp = fopen("ssu_crontab_file", "w+")) == NULL) {
		fprintf(stderr, "fopen error for ssu_crontab_file\n");
		exit(1);
	}

	for (int i = 0; i < line_counter+1; i++)
		fputs(copy[i],crontab_fp);

	write_remove_log(remove_command);
}

// 명령어 REMOVE 내용 "ssu_crontab_log" 파일에 기록하는 함수
void write_remove_log(char *command)
{
	write_log_date();	// 삭제 시간 기록
	fputs(command, log_fp);
}		

// 명령어 ADD 내용 "ssu_crontab_log" 파일에 기록하는 함수
void write_add_log(char (*argv)[BUFFER_SIZE])
{
	write_log_date();	// 저장 시간 기록

	for(int i = 0; i < 6; i++) {
		fputs(argv[i], log_fp);
		fputc(' ', log_fp);
	}
	fputs(argv[6],log_fp);
	fputc('\n', log_fp);

}

// 명령어 저장 또는 삭제 될 때 시간 기록하는 함수
void write_log_date() 
{
	time_t rawtime;
	char tmp[TIME_LEN];

   	time(&rawtime);
	strncpy(tmp, ctime(&rawtime), TIME_LEN);
	tmp[TIME_LEN-2] = 0;

	char buf[BUFFER_SIZE];
	sprintf(buf,"[%s] ", tmp);	
	fputs(buf, log_fp);

}

// 프로그램 실행 시간을 기록하는 함수
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

