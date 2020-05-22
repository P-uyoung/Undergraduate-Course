#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

int main(void) {
	sigset_t pendingset; 
	sigset_t sig_set; 
	int count = 0;
	
	sigfillset(&sig_set); 
	sigprocmask(SIG_SETMASK, &sig_set, NULL);
	
	while (1) {
		printf("count: %d\n", count++); 
		sleep(1);
		
		if (sigpending(&pendingset) == 0) { 	// 팬딩 중인 시그널 집합을 찾아,
			if (sigismember(&pendingset, SIGINT)) { 	// SIGINT 시그널이 포함되어 있는지 검사
				printf("SIGINT가블록되어대기중. 무한루프를종료.\n"); 
				break; 
			} 
		}
	}
	exit(0);
}

