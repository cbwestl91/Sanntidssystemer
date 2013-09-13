#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int a = 0;

void *parent(){
	int b = 0;
	while(b < 15){
	b++;
	a++;
	}
	printf("variable b: %d, variable a: %d\n", b, a);

}

int main(){
	pthread_t t1, t2;

	pthread_create(&t1, NULL, parent, NULL);
	pthread_create(&t2, NULL, parent, NULL);

	
    	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}



// Everything is as expected. a is incremented up to 15 twice, once by each thread, so it ends up at 30
