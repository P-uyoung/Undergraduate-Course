#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>

void *ssu_thread(void *arg);

int main(void)
{
	pthread_t tid; 
	pid_t pid;

	if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0) { 	// 새로운 thread 생성, ssu_thread 실행 후 종료
		fprintf(stderr, "pthread_createerror\n");
		exit(1); 
	}
	
	pid = getpid(); 
	tid = pthread_self(); 
	printf("Main Thread: pid%u tid%u \n", (unsigned int)pid, (unsigned int)tid); 
	sleep(1); 
	
	exit(0);
}

void *ssu_thread(void *arg) {
	pthread_t tid; 
	pid_t pid;
	
	pid = getpid(); 	// 새로 생성한 thread는 동일한 pid 를 갖지만,
	tid = pthread_self(); 	// main thread 의 tid 와는 다르다
	
	printf("New Thread: pid%d tid%u \n", (int)pid, (unsigned int)tid); 
	
	return NULL;
}
