#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

//structure for shared memory
struct pid_data{
pthread_mutex_t pid_mutex;
pid_t pid;
};

//buffer for message passing
char buffer[1500];

int message_server();

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

	//Perform communication
	int channelID = message_server();

	return EXIT_SUCCESS;
}

//Creates msg server
//Performs communication with client
int message_server(){
	int channelID, clientID;
	channelID = ChannelCreate(0);

	clientID = MsgReceive(channelID, &buffer, sizeof buffer, NULL);

	//Perform some task for client
	printf("client sent: %c \n", buffer[0]);

	//reply to client
	int someKindOfReturn;
	someKindOfReturn = MsgReply(clientID, EOK, &buffer, sizeof buffer);

	//Unnecessary return
	return channelID;
}
