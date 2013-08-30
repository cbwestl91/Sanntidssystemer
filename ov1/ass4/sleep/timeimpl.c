#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *printer(){

	printf("This is a print\n");
	sleep(5);

}

int main(){
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, printer, NULL);
	pthread_create(&thread2, NULL, printer, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}
