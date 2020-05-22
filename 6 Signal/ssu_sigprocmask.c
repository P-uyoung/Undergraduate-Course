#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>

int main(void) 
{
	sigset_t sig_set; 
	int count;
	
	sigemptyset(&sig_set); 	// 인자 초기화
	sigaddset(&sig_set, SIGINT);	// 시그널 sig_set(집합) 인자에 추가 
	sigprocmask(SIG_BLOCK, &sig_set, NULL);		// 시그널을 블록시킨다
	
	for (count = 3; 0 < count; count--) { 
		printf("count %d\n", count); 
		sleep(1); 
	}
	
	printf("Ctrl-C에대한블록을해제\n"); 
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL); 	// 시그널 블록을 해제한다
	printf("count중Ctrl-C입력하면이문장은출력되지않음.\n");
	
	while (1);
	
	exit(0);
}

