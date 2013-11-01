#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <pthread.h>

//Prototypes
int io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
void *counter();

void error(char *s){
  perror(s);
  exit(EXIT_FAILURE);
}

pthread_mutex_t lock;
char *buffer = "AHO\n";
int i = 0;

int main(int argc, char *argv[]){
	if(pthread_mutex_init(&lock, NULL) != 0){
		printf("mutex init failed!\n");
	}



	//Create thread for counter
	pthread_t t1;
	pthread_create(&t1, NULL, &counter, NULL);



	dispatch_t *dpp;
	resmgr_attr_t resmgr_attr;
	dispatch_context_t *ctp;
	resmgr_connect_funcs_t connect_funcs;
	resmgr_io_funcs_t io_funcs;
	iofunc_attr_t io_attr;

	printf("Start resource manager\n");

	// create dispatch.
	if (!(dpp = dispatch_create()))
		error("dispatch_create()");

	// initialize resource manager attributes.
	memset(&resmgr_attr, 0, sizeof(resmgr_attr));
	resmgr_attr.nparts_max = 1;
	resmgr_attr.msg_max_size = 2048;

	// set standard connect and io functions.
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	iofunc_attr_init(&io_attr, S_IFNAM | 0666, 0, 0);

	io_funcs.write = io_write;
	io_funcs.read = io_read;

	// establish resource manager
	if (resmgr_attach(dpp, &resmgr_attr, "/dev/myresource",_FTYPE_ANY, 0, &connect_funcs, &io_funcs, &io_attr) < 0)
		error("resmgr_attach()");

	// wait forever, handling messages.
	ctp = dispatch_context_alloc(dpp);

	printf("Now entering while loop\n");
	while(1){
		if (!(ctp = dispatch_block(ctp)))
			error("dispatch_block()");
		dispatch_handler(ctp);
	}

	pthread_join(t1, NULL);

	exit(EXIT_SUCCESS);
}

//FUNCTIONS TO OVERRIDE IO

int io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb){
	printf("Now entering io_write\n");
//    int     status;
    char    *buf;

//    if ((status = iofunc_write_verify(ctp, msg, ocb, NULL)) != EOK)
//        return (status);

//    if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE)
//        return(ENOSYS);

    /* set up the number of bytes (returned by client's write()) */

    _IO_SET_WRITE_NBYTES (ctp, msg->i.nbytes);

    buf = (char *) malloc(msg->i.nbytes + 1);
    if (buf == NULL)
        return(ENOMEM);

    /*
     *  Reread the data from the sender's message buffer.
     *  We're not assuming that all of the data fit into the
     *  resource manager library's receive buffer.
     */

    resmgr_msgread(ctp, buf, msg->i.nbytes, sizeof(msg->i));
    buf [msg->i.nbytes] = '\0'; /* just in case the text is not NULL terminated */
    printf ("Received %d bytes = '%s'\n", msg -> i.nbytes, buf);

    pthread_mutex_lock(&lock);
    buffer = buf;
    pthread_mutex_unlock(&lock);

    free(buf);

//    if (msg->i.nbytes > 0)
//        ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

    return (_RESMGR_NPARTS (0));
}

int io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb){
    int         nleft;
    int         nbytes;
//    int         nparts;
//    int         status;
    char buf[255];


	printf("Now entering io_read\n");




//    if ((status = iofunc_read_verify (ctp, msg, ocb, NULL)) != EOK)
//        return (status);

//    if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE)
//        return (ENOSYS);

    /*
     *  On all reads (first and subsequent), calculate
     *  how many bytes we can return to the client,
     *  based upon the number of bytes available (nleft)
     *  and the client's buffer size
     */

    nleft = ocb->attr->nbytes - ocb->offset;
    nbytes = min (msg->i.nbytes, nleft);

    if (!ocb->offset) {
    	pthread_mutex_lock(&lock);
    	sprintf(buf, "Current value: %d\n", i);
    	pthread_mutex_unlock(&lock);


        /* set up the return data IOV */
        SETIOV(ctp->iov, buf, strlen(buf));

        /* set up the number of bytes (returned by client's read()) */
        _IO_SET_READ_NBYTES (ctp, strlen(buf));

        /*
         * advance the offset by the number of bytes
         * returned to the client.
         */

        ocb->offset = 1;

        return _RESMGR_NPARTS(1);

    } else {
        /*
         * they've asked for zero bytes or they've already previously
         * read everything
         */

        _IO_SET_READ_NBYTES (ctp, 0);

        return _RESMGR_NPARTS(0);
    }

    /* mark the access time as invalid (we just accessed it) */

//    if (msg->i.nbytes > 0)
//        ocb->attr->flags |= IOFUNC_ATTR_ATIME;
}

void *counter(){
	while(1){

		pthread_mutex_lock(&lock);
		if(strstr(buffer, "up")){
			i++;
		} else if(strstr(buffer, "down")){
			i--;
		} else if(strstr(buffer, "stop")){

		}
		pthread_mutex_unlock(&lock);

		delay(100);
	}
}
