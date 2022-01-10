#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {	
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid != 0)		//부모가 아닌,
		exit(1);

	printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));
	setsid();			//자식 프로세스에서 setsid() 호출하여, 세션 리더 프로세스 생성
	printf("aftet pid = %d, sid = %d\n", getpid(), getsid(getpid()));
	exit(0);
}
