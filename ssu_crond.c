#include "ssu_crontab.h"

pid_t d_pid;
exec_period period;
char exec_cycle[BUFFER_SIZE];
char command[BUFFER_SIZE];
int log_fd, crontab_fd;

// 디몬 프로세스
int ssu_daemon_init(void)
{
	pid_t pid;
	int fd, maxfd;
	char *log_filename = "ssu_crontab_log";
	char *crontab_filename = "ssu_crontab_file";

	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if (pid != 0)		// fork() 두번.. 부모가 죽어 자식프로세스를 백그라운드로 실행하게 한다
	{
		exit(0);
	}

	d_pid = getpid();
	setsid();
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	maxfd = getdtablesize();

	for (fd = 0; fd < maxfd; fd++)
		close(fd);

	umask(0);
	//chdir("/");

	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	//return 0;

	while (1) {
		system_command();
		sleep(10);
	}

}

// "ssu_crontab_file" 에 저장된 명령어 수행하는 함수
void system_command(void)
{
	if ((crontab_fd = open(crontab_filename, O_RDONLY | O_CREAT, 0666)) < 0) {
			fprintf(stderr, "open error for %s\n", crontab_filename);
			exit(1);
		}
	
	
	for (주기)
		if (조건)
	
			if(system(command) != 0)
				write_log();
			else {
				fprintf(stderr, "system error\n");
				exit(1);
			}






	close(crontab_fd);

}

// 명령어 정상수행시 로그 작성하는 함수
void write_log(void) 
{
	if ((log_fd = open(log_filename, O_WRONLY | O_CREAT, 0666)) < 0)
	{
		fprintf(stderr, "open error for %s\n", log_filename);
		exit(1);
	}


	close(log_fd);
}

// 실행주기 MIN(분)의 범위 검사하는 함수
int min_scope_check(char (*argv)[BUFFER_SIZE], int i)	// period.min[] 의 i번째로 들어갈 범위 검사
{
	if (argv[1][i] == 48) {	// min:0
		period.min[period.min_count] = 0;
		period.min_count++;
		if (argv[1][i+1] >= 48 && argv[1][i+1] <= 57)
			return -1;
	}		
	
	if (argv[1][i] > 48 && argv[1][i] <= 53) {	// min 범위:1-5, 10-59 
		if (argv[1][i+1] >= 48 && argv[1][i+1] <= 57) {	
			period.min[period.min_count] = ((argv[1][i] -48)*10 + (argv[1][i+1] - 48));
			period.min_count++;
		}
		else {
			period.min[period.min_count] = argv[1][i] -48;		
			period.min_count++;
		}
	}

	if (argv[1][i] > 53 && argv[1][i] <= 57) {	// min 범위:6-9
		period.min[period.min_count] = argv[1][i] - 48;
		period.min_count++;
		if (argv[1][i+1] >= 48 && argv[1][i+1] <= 57)
			return -1;
	}
}

// 실행주기 HOUR(시간)의 범위를 검사하는 함수
int hour_scope_check(char (*argv)[BUFFER_SIZE], int i)	// period.hour[] 의 i번째로 들어갈 범위 검사
{
	if (argv[2][i] == 48) {	// hour:0
		period.hour[period.hour_count] = 0;
		period.hour_count++;
		if (argv[2][i+1] >= 48 && argv[2][i+1] <= 57)
			return -1;
	}		
	
	if (argv[2][i] == 49) {	// hour 범위:1, 10-19 
		if (argv[2][i+1] >= 48 && argv[2][i+1] <= 57) {	
			period.hour[period.hour_count] = (10 + (argv[2][i+1] - 48));
			period.hour_count++;
		}
		else {
			period.hour[period.hour_count] = 1;		
			period.hour_count++;
		}
	}
	
	if (argv[2][i] == 50) {	// hour 범위:2, 20-23
		if (argv[2][i+1] >= 48 && argv[2][i+1] <= 51) {	
			period.hour[period.hour_count] = (20 + (argv[2][i+1] - 48));
			period.hour_count++;
		}
		else if (argv[2][i+1] > 51 && argv[2][i+1] <= 57) 
			return -1;
		
		else  {
			period.hour[period.hour_count] = 2;		
			period.hour_count++;
		}
	}
	
	if (argv[2][i] >= 51 && argv[2][i] <= 57) { // hour 범위:3-9
		if (argv[2][i+1] >= 48 && argv[2][i+1] <= 57) 
			return -1;
		else {
			period.hour[period.hour_count] = argv[2][i] - 48;
			period.hour_count++;	
		}
		
	}
}

// 실행주기 DAY(일)의 범위를 검사하는 함수
int day_scope_check(char (*argv)[BUFFER_SIZE], int i)	// period.day[] 의 i번째로 들어갈 범위 검사
{
	if (argv[3][i] == 48)
		return -1;

	if (argv[3][i] == 49 || argv[3][i] == 50) {	// day 범위:1-2, 10-29 
		if (argv[3][i+1] >= 48 && argv[3][i+1] <= 57) {	
			period.day[period.day_count] = ((argv[3][i] - 48)*10 + (argv[3][i+1] - 48));
			period.day_count++;
		}
		else {
			period.day[period.day_count] = argv[3][i] - 48;		
			period.day_count++;
		}
	}
	
	if (argv[3][i] == 51) {	// day 범위:3, 30-31
		if (argv[3][i+1] == 48 || argv[3][i+1] == 49) {	
			period.day[period.day_count] = (30 + (argv[3][i+1] - 48));
			period.day_count++;
		}
		else if (argv[3][i+1] >= 50 && argv[3][i+1] <= 57) 
			return -1;
		
		else  {
			period.day[period.day_count] = 3;		
			period.day_count++;
		}
	}
	
	if (argv[3][i] >= 52 && argv[3][i] <= 57) { // day 범위:4-9
		if (argv[3][i+1] >= 48 && argv[3][i+1] <= 57) 
			return -1;
		else {
			period.day[period.day_count] = argv[3][i] - 48;
			period.day_count++;	
		}	
	}
}

// 실행주기 MONTH(월)의 범위를 검사하는 함수
int month_scope_check(char (*argv)[BUFFER_SIZE], int i)	// period.month[] 의 i번째로 들어갈 범위 검사
{
	if (argv[4][i] == 48)
		return -1;

	if (argv[4][i] == 49) {	// month 범위:1, 10-12
		if (argv[4][i+1] >= 48 && argv[4][i+1] <= 50) {	
			period.month[period.month_count] = (10 + (argv[4][i+1] - 48));
			period.month_count++;
		}
		else if (argv[4][i+1] >= 51 && argv[4][i+1] <= 57) 
			return -1;
		
		else  {
			period.month[period.month_count] = 1;		
			period.month_count++;
		}
	}
	
	if (argv[4][i] >= 50 && argv[4][i] <= 57) { // month 범위:2-9
		if (argv[4][i+1] >= 48 && argv[4][i+1] <= 57) 
			return -1;
		else {
			period.month[period.month_count] = argv[4][i] - 48;
			period.month_count++;	
		}	
	}
}

// 실행주기 WEEK(요일)의 범위를 검사하는 함수
int week_scope_check(char (*argv)[BUFFER_SIZE], int i)	// period.week[] 의 i번째로 들어갈 범위 검사
{
	if (argv[5][i] >= 55 && argv[5][i] <= 57)
		return -1;
	
	if (argv[5][i] >= 48 && argv[5][i] <= 54) {	// week 범위:0-6
		if (argv[5][i+1] >= 48 && argv[5][i+1] <= 57) 	
			return -1;
		period.week[period.week_count] = argv[5][i] - 48;
		period.week_count++;
	}
}

// 실행주기 기호 검사하고 분리하는 함수
int opt_check(char (*argv)[BUFFER_SIZE])
{
	int cycle_count = 0;
	int period.min_count = 0, period.hour_count = 0, period.day_count = 0,
		period.month_count = 0; period_week_count =0;

	min_scope_check(*argv,0);
	hour_scope_check(*argv, 0);
	day_scope_check(*argv, 0);
	month_scope_check(*argv, 0);
	week_scope_check(argv,0);
	
	for (int j = 1; j < 6; j++) { // min, hour, day, month, week 차례대로 검사 및 분리
		for (int i = 0; i < strlen(argv[1]); i++) {
			if (argv[j][i] == '/') {
				if (argv[j][i+1] >= 48 && argv[j][i+1] <= 57) {
					if (argv[j][i+2] >= 48 && argv[j][i+2] <= 57) {
						exec_cycle[j-1] = argv[j][i+1]*600;
						exec_cycle[j-1] += argv[j][i+2]*60;
						cycle_count++;
					}	
					else if (argv[1][i+2] == '\0') {
						exec_cycle[0] = argv[1][i+1]*60;
						cycle_count++;
					}
					
					else	// 주기 뒤 숫자 포맷이 올바르지 않을 때
						return -1;
				}
				else  // 주기 뒤에 숫자가 오지 않을 때
					return -1;
			}

			if else (argv[1][i] == ',') {
				if (
				period.min[period.min_count] = argv[1][i+1] - 48;
				period.min_count++;
		
			}
			
			(기호 이상한거 예외처리...)	
		
		}	
	
		if (cycle_count == 0)	// 주기가 없으면 -1로 저장
			exec_cycle[0] = -1;
	}
}

