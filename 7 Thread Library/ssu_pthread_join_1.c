#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h>

void *ssu_thread(void *arg);

int main(void) 
{
	pthread_t tid1, tid2; 
	int thread1 = 1; 
	int thread2 = 2; 
	int status;
	
	if (pthread_create(&tid1, NULL, ssu_thread, (void *)&thread1) != 0) { 
		fprintf(stderr, "pthread_createerror\n"); 
		exit(1);
	}
	
	if (pthread_create(&tid2, NULL, ssu_thread, (void *)&thread2) != 0) { 
		fprintf(stderr, "pthread_createerror\n"); 
		exit(1);
	} 
	
	pthread_join(tid1, (void *)&status);	// main() 쓰레드가 생성한 쓰레드가 종료될 때까지 기다린다
	pthread_join(tid2, (void *)&status);	
	
	exit(0);
}

void *ssu_thread(void *arg) {
	int thread_index; 
	int i;
	
	thread_index = *((int*)arg);	// arg casting
	
	for (i = 0; i < 5; i++) { 
		printf("%d : %d\n", thread_index, i);	// thread1,2 뒤엉켜 실행된다
		sleep(1); 
	}
	
	return NULL;
}

