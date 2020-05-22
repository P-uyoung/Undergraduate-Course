#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

void ssu_signal_handler(int signo) { 
	printf("ssu_signal_handlercontrol\n"); 
}

int main(void) {
	struct sigaction sig_act; 
	sigset_t sig_set;
	
	sigemptyset(&sig_act.sa_mask); 	// 초기화
	sig_act.sa_flags = 0; 
	sig_act.sa_handler = ssu_signal_handler; 	// sa_handler 는 핸들러함수의 주소를 갖는다
	sigaction(SIGUSR1, &sig_act, NULL); 	// SIGCUSR1 시그널에 관한 액션을 변경
	printf("before first kill()\n"); 
	kill(getpid(), SIGUSR1); 	// for. 액션이 변경되었는지 확인
	
	sigemptyset(&sig_set); 	// 초기화
	sigaddset(&sig_set, SIGUSR1); 	// 시그널 추가
	sigprocmask(SIG_SETMASK, &sig_set, NULL); 	// SIGUSR1 블록
	printf("before second kill()\n"); 
	kill(getpid(), SIGUSR1); 	// for. 블록되었는지 확인
	printf("after second kill()\n"); 
	exit(0);
}
