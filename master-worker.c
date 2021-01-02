#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

int item_to_produce, curr_buf_size, item_to_consume;
int total_items, max_buf_size, num_workers, num_masters; //arguments

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex_init
pthread_cond_t cond_master = PTHREAD_COND_INITIALIZER; //cond_init
pthread_cond_t cond_worker = PTHREAD_COND_INITIALIZER; //cond_init

int *buffer;

void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data) //Master-thread execute
{
	int thread_id = *((int *)data);

	while(1)
	{
		pthread_mutex_lock(&mutex); //lock among master-thread


		if(item_to_produce >= total_items) { //If all item is produced, terminate
			pthread_mutex_unlock(&mutex);
			pthread_cond_broadcast(&cond_worker);
			break;
		}
		
		if(curr_buf_size == max_buf_size){ //If buf is full, wait
			pthread_cond_broadcast(&cond_worker);
			pthread_cond_wait(&cond_master, &mutex);

			if(item_to_produce >= total_items) { //If all item is produced, terminate
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&cond_worker);
				break;
			}
		}

		if (curr_buf_size < 0)
			curr_buf_size = 0;

		else {
			buffer[curr_buf_size++] = item_to_produce;
			print_produced(item_to_produce, thread_id);
			item_to_produce++;
		}

		pthread_mutex_unlock(&mutex); //unlock among master-thread
	}
	return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item
void *consume_requests_loop(void *data) //Worker-thread execute
{
	int thread_id = *((int*)data);

	while(1)
	{
		pthread_mutex_lock(&mutex);

		if(item_to_consume < 1){ //If all item is consumed, terminate
			pthread_mutex_unlock(&mutex);
			break;
		}

		if(curr_buf_size <= 0){	//If buf is empty, wait
			if(item_to_consume <= 0){
				pthread_mutex_unlock(&mutex);
				break;
			}

			pthread_cond_broadcast(&cond_master);
			pthread_cond_wait(&cond_worker, &mutex);
		}


		if(curr_buf_size < 0)
			curr_buf_size = 0;

		else if(curr_buf_size==0)
			;

		else if(buffer[--curr_buf_size] != -1) { //&(curr_buf_size > 0)
			print_consumed(buffer[curr_buf_size], thread_id);
			buffer[curr_buf_size] = -1;
			item_to_consume--;
		}
		
		pthread_mutex_unlock(&mutex);
	
	}

	return 0;

}


int main(int argc, char *argv[])
{
	int *master_thread_id, *worker_thread_id;
	pthread_t *master_thread, *worker_thread;

	item_to_produce = 0;
	curr_buf_size = 0;

	int i;

	if (argc < 5) {
		printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
		exit(1);
	}
	else {
		num_masters = atoi(argv[4]);
		num_workers = atoi(argv[3]);
		total_items = atoi(argv[1]);
		max_buf_size = atoi(argv[2]);
		item_to_consume = total_items;
	}


		buffer = (int *)malloc (sizeof(int) * max_buf_size);

		//create master producer threads
		master_thread_id = (int *)malloc(sizeof(int) * num_masters);
		master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
		for (i = 0; i < num_masters; i++)
			master_thread_id[i] = i;

		for (i = 0; i < num_masters; i++)
			pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);

		//create worker consumer threads
		worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
		worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
		for (i=0; i<num_workers; i++)
			worker_thread_id[i] = i;
		for(i = 0; i <num_workers; i++)
			pthread_create(&worker_thread[i], NULL, consume_requests_loop, (void *)&worker_thread_id[i]);

		//wait for all master-threads to complete
		for (i = 0; i < num_masters; i++)
		{
			pthread_join(master_thread[i], NULL);
			printf("master %d joined\n", i);
		}

		//wait for all worker-thread to complete
		for (i = 0; i < num_workers; i++)
		{
			pthread_join(worker_thread[i], NULL);
			printf("worker %d joined\n", i);
		}

		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond_master);
		pthread_cond_destroy(&cond_worker);

		return 0;
}
