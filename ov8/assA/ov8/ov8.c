#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

struct pid_data{
pthread_mutex_t pid_mutex;
pid_t pid;
};

int main(int argc, char *argv[]) {
	printf("Welcome to the QNX Momentics IDE\n");


	//Create shared memory location
	int f_desc = shm_open("/dev/shmem/sharedpid", O_RDWR | O_CREAT, S_IRWXU);

	ftruncate(f_desc, (sizeof(struct pid_data)));

	struct pid_data* voidy = mmap(0, (sizeof(struct pid_data)), (PROT_READ | PROT_WRITE), MAP_SHARED, f_desc, 0);

	//Create shared memory mutex
	pthread_mutexattr_t mutey;

	pthread_mutexattr_init(&mutey);
	pthread_mutexattr_setpshared(&mutey, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&voidy->pid_mutex, &mutey );

	//Put something inside the struct
	pthread_mutex_lock(&voidy->pid_mutex);
	voidy->pid = getpid();
	printf("My pid is %d \n", voidy->pid);

	pthread_mutex_unlock(&voidy->pid_mutex);

	return EXIT_SUCCESS;
}
