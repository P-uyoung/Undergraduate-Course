#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

int main(void) {
	sigset_t old_set; 
	sigset_t sig_set;
	
	sigemptyset(&sig_set); 
	sigaddset(&sig_set, SIGINT); 
	sigprocmask(SIG_BLOCK, &sig_set, &old_set); 
	sigsuspend(&old_set); 		// 시그널이 발생할 때까지 잠시 팬딩   
	
	exit(0);
}

