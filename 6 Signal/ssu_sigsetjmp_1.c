/* sigsetjmp(), siglongjmp() 을 사용하지 않고 
 * 여러함수로 호출하여 setjmp(), longjmp() 을 사용한 경우
 * 사용자가 SIGINT 시그널을 발생시켜도 SIGINT 블록상태이므로 
 * (setjmp(), longjmp(): 시그널 마스크 복원기능이 없음)
 * 핸들러 함수가 실행되지 않는 문제점 발생 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h> 
#include <setjmp.h>

void ssu_signal_handler(int signo);

jmp_buf jump_buffer;

int main(void) {
	signal(SIGINT, ssu_signal_handler);
	while (1) { 
		if (setjmp(jump_buffer) == 0) { 
			printf("Hit Ctrl-c at anytime ... \n"); 
			pause(); 
		} 
	}

	exit(0);
}

void ssu_signal_handler(int signo) {
	char character;
	
	signal(signo, SIG_IGN); 
	printf("Did you hit Ctrl-c?\n" "Do you really want to quit? [y/n] "); 
	character = getchar(); 
	
	if (character == 'y' || character == 'Y') 
		exit(0); 
	else { 	// 사용자 입력 'n' 인 경우
		signal(SIGINT, ssu_signal_handler);	// SIGINT 시그널에 대한 핸들러 함수 재설정 
		longjmp(jump_buffer, 1); 	// 이전에 main() 에서 설정한 setjmp()로 점프
		
	}
}

