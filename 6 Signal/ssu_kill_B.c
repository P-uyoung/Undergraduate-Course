#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>

int main(void)	// 백그라운드 프로세스로 실행될 예정이며, ssu_kill_A 프로그램에 의해 종료될 것 
{
	while (1) { 
		printf("\n[OSLAB]"); 
		sleep(5); 
	}
	
	exit(0);
}

