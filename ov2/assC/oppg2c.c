#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t lock;

void *resource_access(void *argument){
	sem_wait(&lock);
	int proc = (int) argument;
	printf("Thread number: %d\n", proc);
	usleep(1000000);
	sem_post(&lock);

	return 0;
}

int main(){
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	sem_init(&lock, 0, 3);

	pthread_t t1, t2, t3, t4, t5;

	pthread_create(&t1, NULL, resource_access, (void *) a);
	pthread_create(&t2, NULL, resource_access, (void *) b);
	pthread_create(&t3, NULL, resource_access, (void *) c);
	pthread_create(&t4, NULL, resource_access, (void *) d);
	pthread_create(&t5, NULL, resource_access, (void *) e);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_join(t5, NULL);

	return 0;
}
