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
//buffer for received messages
char receive[1500];

int message_client(pid_t pid);


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
	buffer[0] = 'G';

	//Perform communication
	int channelID = message_client(result);

	return EXIT_SUCCESS;
}

//Communication with the server
//Using buffers: buffer, receive
int message_client(pid_t pid){
	int channelID, status;
	channelID = ConnectAttach(0, pid, 1, 0, 0);

	//Sends data in buffer. Puts returned data in receive buffer
	status = MsgSend(channelID, &buffer, sizeof buffer, &receive, sizeof receive);

	//Detach from server
	ConnectDetach(channelID);

	//Unnecessary return
	return channelID;
}
