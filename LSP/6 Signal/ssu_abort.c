#include <stdio.h> 
#include <stdlib.h>

int main(void) { 
	printf("abort terminate this program\n"); 
	abort(); 	// SIGABRT 시그널 발생킨다. 디폴트 액션은 비정상적 종료(Aborted (core dumped))
	printf("this line is never reached\n"); 
	exit(0); 
}
