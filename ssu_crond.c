#include "ssu_crond.h"

exec_period period;
char exec_cycle[BUFFER_SIZE];
char command[BUFFER_SIZE];
int log_fd, crontab_fd;

int main(void)
{
	char *log_filename = "ssu_crontab_log";
	char *crontab_filename = "ssu_crontab_file";

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


