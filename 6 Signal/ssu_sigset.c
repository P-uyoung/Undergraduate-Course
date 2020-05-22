#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>

int main(void) 
{
	sigset_t set;
	
	sigemptyset(&set); 	// 시그널 집합 초기화
	sigaddset(&set, SIGINT);	// 시그널 집합에 SIGINT 시그널 추가

	switch (sigismember(&set, SIGINT)) 	// 해당 시그널이 시그널 집합에 존재하는가
	{ 
		case 1: 
			printf("SIGINT is included. \n");	// 해당 문자열이 출력됨 
			break; 
		case 0: 
			printf("SIGINT is not included. \n"); 
			break; 
		default: 
			printf("failed to call sigismember() \n"); 
	}
	switch (sigismember(&set, SIGSYS)) 	
	{ 
	case 1: 
		printf("SIGSYS is included. \n"); 
		break; 
	case 0: 
		printf("SIGSYS is not included. \n"); 		// 해당 문자열이 출력됨
		break; 
	default: 
		printf("failed to call sigismember() \n"); 
	}
	
	exit(0);
}

