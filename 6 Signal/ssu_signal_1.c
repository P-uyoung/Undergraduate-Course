#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

void ssu_signal_handler(int signo); 
void (*ssu_func)(int);

int main (void) 
{
	ssu_func = signal(SIGINT, ssu_signal_handler);	// 리턴값: 이전 시그널 핸들러
	while (1) { 
		printf("process running...\n"); 
		sleep(1); 
	}
	exit(0);
}
void ssu_signal_handler(int signo) { 
	printf("SIGINT 시그널발생.\n"); 
	printf("SIGINT를SIG_DFL로재설정함.\n"); 
	signal(SIGINT, ssu_func);	// 이전 핸들러를 이용해서 다시 시그널 핸들러로 등록 
}

