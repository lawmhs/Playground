/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
#ifndef IOCHILD_H
#define IOCHILD_H

/********************
**  defines for the child process
*********************/
#include <limits.h>
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>

#include <mpi.h>
#include "chan.h"

// timebuffer size, must be greater than 26
#define TIMEBUFSIZE 28
#define TIMEOUTTIME 30   // 30 secs
#define MAXRECVBUF 1024  

enum chan_comm_type { SEND, RECV, DISPOSE };   // pchannel for MPI process 

// child thread arguments 
typedef struct {
        chan_t *c;
        char   *myname;
        pthread_t thread;
        int    myrank;
        int    fdpipe;     // read end of the pipe
        int    fdsend;     // pipe for writing to child, used only by main
        int    receivebuffsize;   // maximum size of the MPI receive buffer
        short int port;    // may not be needed? if simple file
        enum client_protocol protocol;  // type of protocol for the child, all connections connect using this protocol
} childargs_t;

childargs_t *start_child_thread(char *name, int myrank, int servPort, int maxbufsize);
void stop_child_thread(childargs_t **cargptr);

// Called by the parent for communicating with the child
int MPIX_chan(enum chan_comm_type type, const void *buf, int count, MPI_Datatype datatype, int source, childargs_t *args,MPI_Request *request);
int MPIX_chan_close(childargs_t *args);

#endif /* IOCHILD_H */
