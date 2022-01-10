#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int ssu_daemon_init(void);
int main(void) 
{
	pid_t pid;

	pid = getpid();
	printf("parent process : %d\n", pid);
	printf("daemon process initialization\n");

	if (ssu_daemon_init() < 0) {
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}

	exit(0);	//부모 종료
}

int ssu_daemon_init(void) {
	pid_t pid;
	int fd, maxfd;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	
	else if (pid != 0)
		exit(0);

	pid = getpid();
	printf("process %d running as daemon\n", pid);
	setsid();		//자식프로세스를 새로운 세션의 리더로 만들어, 제어 터미널이 없게 한다

	//터미널 입출력 시그널을 무시하게 한다
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	
	maxfd = getdtablesize();

	for (fd = 0; fd < maxfd; fd++)
		close(fd);		//필요없는 파일디스크립터 모두 닫기

	umask(0);
	chdir("/");		//루트 디렉토리를 다음 작업 디렉토리로
	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	
	return 0;
}
