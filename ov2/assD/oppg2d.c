#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef int bool;
#define true 1
#define false 0

//Exposed variables
bool running = 1;
int var1 = 0;
int var2 = 0;

//Mutex for saving the day
pthread_mutex_t lock;

void *thread1(){
	while(running){
		pthread_mutex_lock(&lock);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(&lock);
	}

	return 0;
}

void *thread2(){
	int i;
	for(i = 0; i < 20; i++){
		pthread_mutex_lock(&lock);
		printf("Number 1 is %d\nNumber 2 is %d\n\n", var1, var2);
		usleep(100000);
		pthread_mutex_unlock(&lock);
		usleep(1000000);
	}
	running = 0;

	return 0;
}

int main(){
	pthread_t t1, t2;

	pthread_mutex_init(&lock, NULL);
	
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_mutex_destroy(&lock);

	return 0;
}
