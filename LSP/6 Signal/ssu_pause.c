#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

void ssu_alarm(int signo);
int main(void) 
{
	printf("Alarm Setting\n"); 
	signal(SIGALRM, ssu_alarm); 	// ssu_alarm 을 SIGALRM 의 시그널핸들러로 등록
	alarm(2);
	
	while (1) { 
		printf("done\n"); 
		pause(); 	// SIGALRM 시그널이 발생할 때까지 프로세스 중지
		alarm(2); 
	}
	
	exit(0);
}
void ssu_alarm(intsigno) { 
	printf("alarm..!!!\n"); 
}

