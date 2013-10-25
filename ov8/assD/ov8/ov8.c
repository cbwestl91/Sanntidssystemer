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

//PRIORITY FUNCTIONS
int set_priority(int priority);
int get_priority();

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

	//Transform into message server! EVOLUTION!!!
	int channelID = message_server();

	return EXIT_SUCCESS;
}

//Creates msg server
//Performs communication with client
int message_server(){
	set_priority(20);

	printf("I am now a server \n\n");


	int channelID, clientID;
	channelID = ChannelCreate(0);

	struct _msg_info client_info;

	while(1){
		printf("PRIORITY BEFORE: %i\n", get_priority());

		clientID = MsgReceive(channelID, &buffer, sizeof buffer, &client_info);
		printf("PRIORITY AFTER: %i\n\n", get_priority());

		//Print client info
		printf("CLIENT INFO: \n");
		printf("from client with ID: %d\n", client_info.pid);
		printf("from process with ID: %d\n", client_info.pid);
		printf("Received msg of length: %d\n", client_info.msglen);

		//Perform some task for client
		printf("client sent: %c \n\n", buffer[0]);

		buffer[0] = 'S';

		//reply to client
		int someKindOfReturn;
		someKindOfReturn = MsgReply(clientID, EOK, buffer, sizeof buffer);
	}
	//Unnecessary return
	return channelID;
}

//**************************************
//Below are functions for managing thread priorities
//**************************************

int set_priority(int priority)
{
	int policy;
	struct sched_param param;
	// check priority in range
	if (priority < 1 || priority >63)
		return -1;
	// set priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	param.sched_priority = priority;
	return pthread_setschedparam(pthread_self(), policy, &param);
}

int get_priority()
{
	int policy;
	struct sched_param param;
	// get priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	return param.sched_curpriority;
}
