#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>

void ssu_signal_handler1(int signo); 
void ssu_signal_handler2(int signo);

int main(void) {
	if (signal(SIGINT, ssu_signal_handler1) == SIG_ERR) { 	// SIGINT 시그널 핸들러 등록
		fprintf(stderr, "cannot handle SIGINT\n"); 
		exit(EXIT_FAILURE); 
	}
	if (signal(SIGUSR1, ssu_signal_handler2) == SIG_ERR) { 	// SIGUSR1 시그널 핸들러 등록
		fprintf(stderr, "cannot handle SIGUSR1\n"); 
		exit(EXIT_FAILURE); 
	}
	
	raise(SIGINT);	 // 시그널에 대응되는 시그널 핸들러 호출
	raise(SIGUSR1); 
	printf("main return\n"); 
	exit(0);
}

void ssu_signal_handler1(intsigno) { 
	printf("SIGINT 시그널발생\n"); 
}
void ssu_signal_handler2(intsigno) { 
	printf("SIGUSR1 시그널발생\n"); 
}
