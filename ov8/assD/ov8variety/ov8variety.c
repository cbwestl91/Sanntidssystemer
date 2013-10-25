#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <pthread.h>

//structure for shared memory
struct pid_data{
pthread_mutex_t pid_mutex;
pid_t pid;
};

//Create struct argument for threads
struct thread_args{
int which_thread;
pid_t pid;
};

//buffer for message passing
char buffer[1500];
//buffer for received messages
char receive[1500];

void* message_client(void* arg);

//PRIORITY FUNCTIONS
int set_priority(int priority);
int get_priority();


int main(int argc, char *argv[]) {

	printf("Welcome to the QNX Momentics IDE\n");

	//This program reads from a shared memory location
	//and prints content

	//Access shared memory
	int f_desc = shm_open("/dev/shmem/sharedpid", O_RDWR, S_IRWXU);

	struct pid_data* voidy = mmap(0, (sizeof(struct pid_data)), (PROT_READ | PROT_WRITE), MAP_SHARED, f_desc, 0);

	//Extract info from shared memory
	pid_t result = 0;

	pthread_mutex_lock(&voidy->pid_mutex);
	result = voidy->pid;
	pthread_mutex_unlock(&voidy->pid_mutex);


	printf("The pid of the other program is %d \n", result);
	buffer[0] = 'G';

	struct thread_args arg1;
	struct thread_args arg2;
	struct thread_args arg3;
	struct thread_args arg4;

	arg1.pid = result;
	arg2.pid = result;
	arg3.pid = result;
	arg4.pid = result;

	arg1.which_thread = 0;
	arg2.which_thread = 1;
	arg3.which_thread = 2;
	arg4.which_thread = 3;

	set_priority(60);
	//Perform communication through THREADSSSS, LOTS OF THREADSSSS
	pthread_t t1, t2, t3, t4;

	//main thread needs highest priority

	if(pthread_create(&t1, NULL, message_client, (void*)&arg1))
		printf("FAILED TO CREATE THREAD\n");
	if(pthread_create(&t2, NULL, message_client, (void*)&arg2))
		printf("FAILED TO CREATE THREAD\n");
	if(pthread_create(&t3, NULL, message_client, (void*)&arg3))
		printf("FAILED TO CREATE THREAD\n");
	if(pthread_create(&t4, NULL, message_client, (void*)&arg4))
		printf("FAILED TO CREATE THREAD\n");

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);

	return EXIT_SUCCESS;
}

//Communication with the server
//Using buffers: buffer, receive
void* message_client(void* arg){

	//Define thread priority
	if(((struct thread_args*)arg)->which_thread == 0){
		set_priority(22);
		printf("I HAVE PRIORITY 22\n");
	}
	else if(((struct thread_args*)arg)->which_thread == 1){
		set_priority(21);
		printf("I HAVE PRIORITY 21\n");
	}
	else if(((struct thread_args*)arg)->which_thread == 2){
		set_priority(19);
		printf("I HAVE PRIORITY 19\n");
	}
	else{
		set_priority(18);
		printf("I HAVE PRIORITY 18\n");
	}

	//Perform communication
	int channelID, status;

	channelID = ConnectAttach(0, ((struct thread_args*)arg)->pid, 1, 0, 0);

	//Sends data in buffer. Puts returned data in receive buffer
	status = MsgSend(channelID, &buffer, sizeof buffer, &receive, sizeof receive);

	printf("SERVER REPLY: %c\n", receive[0]);

	//Detach from server
	ConnectDetach(channelID);

	return 0;
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
