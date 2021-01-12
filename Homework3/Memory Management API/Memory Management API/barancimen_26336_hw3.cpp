#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>

#include <ctime> // included to generate random seeds every time
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size


bool server_on = true;


void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	node temp_node;
	temp_node.id = thread_id;
	temp_node.size = size;
	myqueue.push(temp_node);
}

void * server_function(void *)
{
	
	//This function should grant or decline a thread depending on memory size.
	int index = 0;
	while(server_on) {
		pthread_mutex_lock(&sharedLock);
		if(!myqueue.empty()) {
			node temp_node = myqueue.front();
			myqueue.pop();
			if(temp_node.size < MEMORY_SIZE - index) {
				thread_message[temp_node.id] = index;
				index += temp_node.size;
			}
			else {
				thread_message[temp_node.id] = -1;
			}
			sem_post(&semlist[temp_node.id]);
		}
		pthread_mutex_unlock(&sharedLock);
	}
}

void * thread_function(void * id) 
{
	//This function will create a random size, and call my_malloc
	int * id_ptr = (int *) id;

	srand(time(NULL) + *id_ptr);
	int size = (rand() % MEMORY_SIZE) / 6; // since it should be smaller than memory size / 6 [I didnt do % (MEMORY_SIZE / 6) since it wouldnt be equal to that value.]


	pthread_mutex_lock(&sharedLock);
	my_malloc(*id_ptr, size);
	pthread_mutex_unlock(&sharedLock);



	//Block
	sem_wait(&semlist[*id_ptr]);

	//Then fill the memory with id's or give an error prompt
	if(thread_message[*id_ptr] == -1) { // No available memory
		printf("Thread[%d]: No Available Memory\n", *id_ptr);
	}
	else { // memory is available
		for(size_t i = 0; i < size; i++) {
			memory[thread_message[*id_ptr] + i] = *id_ptr;
		}
	}

	return NULL;

}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
 // You need to print the whole memory array here.
	for(size_t i=0; i<MEMORY_SIZE; i++) {
		printf("[%d]",memory[i]);
	}
}

int main (int argc, char *argv[])
 {

 	//You need to create a thread ID array here
	int threadIDs[NUM_THREADS];
	pthread_t threads[NUM_THREADS];

	for(size_t i=0; i<NUM_THREADS; i++) {
		threadIDs[i] = i;
		pthread_t temp;
		threads[i] = temp;
	}


 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()
	for(size_t i=0; i<NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, thread_function, (void *) & threadIDs[i]); 
	}


 	//You need to join the threads
	for(size_t i=0; i<NUM_THREADS; i++) {
		pthread_join(threads[i], NULL); 
	}

	

 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}

 	printf("\nTerminating...\n");
	// To terminate we need to close the server
	server_on = false;
	// Delete the queue nodes (deallocation)
	while(!myqueue.empty()) {
		myqueue.pop();
	}
	// We need to destroy the thread semaphores (deallocation)
	for (size_t i = 0; i < NUM_THREADS; i++) {
		sem_destroy(&semlist[i]);
	}
 }