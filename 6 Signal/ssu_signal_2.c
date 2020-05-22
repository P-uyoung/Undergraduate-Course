#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

static void ssu_signal_handler(int signo);

int main(void) {
	if (signal(SIGINT, ssu_signal_handler) == SIG_ERR) { 
		fprintf(stderr, "cannot handle SIGINT\n"); 
		exit(EXIT_FAILURE); 
	}
	
	if (signal(SIGTERM, ssu_signal_handler) == SIG_ERR) { 
		fprintf(stderr, "cannot handle SIGTERM\n"); 
		exit(EXIT_FAILURE); 
	}
	
	if (signal(SIGPROF, SIG_DFL) == SIG_ERR) {	// SIGPROF 시그널-> SIG_DFL 로 등록, 시그널의 디폴트 액션을 실행
		fprintf(stderr, "cannot reset SIGPROF\n"); 
		exit(EXIT_FAILURE); 
	}
	
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) { 	// SIGHUP 시그널-> SIG_IGN 로 등록, 시그널이 발생하면 지정된 시그널 무시
		fprintf(stderr, "cannot ignore SIGHUP\n"); 
		exit(EXIT_FAILURE); 
	}
	while (1) 
		pause();
	
	exit(0);
}

static void ssu_signal_handler(int signo) {
	if (signo == SIGINT) 
		printf("caught SIGINT\n"); 
	
	else if (signo == SIGTERM) 
		printf("caught SIGTERM\n"); 
	
	else { 
		fprintf(stderr, "unexpected signal\n"); 
		exit(EXIT_FAILURE); 
	}
	
	exit(EXIT_SUCCESS);
}
