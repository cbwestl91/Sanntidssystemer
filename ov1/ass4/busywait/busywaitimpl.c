#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/times.h>
#include <unistd.h>

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}

void *printer(){

	printf("This is a print\n");
	busy_wait_delay(5);

}

int main(){
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, printer, NULL);
	pthread_create(&thread2, NULL, printer, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}
