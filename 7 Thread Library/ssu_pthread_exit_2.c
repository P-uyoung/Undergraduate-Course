#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <pthread.h>

void *ssu_thread(void *arg);

int main(void) {
	pthread_t tid;
	
	if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) { 
		fprintf(stderr, "pthread_createerror\n"); 
		exit(1); 
	}
	
	sleep(1); 
	
	printf("쓰레드가완료되기전main 함수가먼저종료되면실행중쓰레드 소멸\n"); 
	printf("메인종료\n"); 
	
	exit(0);	// main() 쓰레드 종료.. 생성된 쓰레드도 종료
}

void *ssu_thread(void *arg) 
{ 
	printf("쓰레드시작\n"); 
	sleep(5);	// main() 쓰레드가 생성된 쓰레드보다 먼저 종료
	
	printf("쓰레드수행완료\n"); 
	pthread_exit(NULL); 
	return NULL; 
}
