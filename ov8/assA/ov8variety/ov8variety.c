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

	//This program reads from a shared memory location
	//and prints content

	//Access shared memory
	int f_desc = shm_open("/dev/shmem/sharedpid", O_RDWR, S_IRWXU);

	struct pid_data* voidy = mmap(0, (sizeof(struct pid_data)), (PROT_READ | PROT_WRITE), MAP_SHARED, f_desc, 0);

	//Extract info from shared memory
	pid_t result;

	pthread_mutex_lock(&voidy->pid_mutex);
	result = voidy->pid;
	pthread_mutex_unlock(&voidy->pid_mutex);


	printf("The pid of the other program is %d \n", result);

	return EXIT_SUCCESS;
}
