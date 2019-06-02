/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
#ifndef ADAPGREQUEST_H
#define ADAPGREQUEST_H

#include <mpi.h>
#include "debug.h"
#include "adapter.h"

#define FIRSTCONNMSG 0
#define RESTCONNMSG 1
//const char * tagtype_strings[] = { "FIRSTCONNMSG", "RESTCONNMSG" };

//#define NDEBUG  to turn assertions off
static const int GREQDEBUG = 0;
//static const char dheader[] = ".............................................................";

// communication extra-state data structure in generalized request, structure passed between parent and child
typedef struct {
        chan_t *c;   // channel for messages
        enum chan_comm_type ctype;
        long int seqnum;
        int client;
        int nbytes;              // for a receive, records the exact number of bytes received, for send should be length of buffer
        const void *buf;         // the buffer provided by the MPI send/receive command
        int count;               // datatype count
        int tag;                 // provided by the MPI send/receive command  -- matching?
        MPI_Datatype datatype;   // for now leave it as a simple type
        MPI_Request *request;
        MPI_Status *status;
} params_struct;

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
        enum client_protocol protocol;  // type of protocol for the connection
} childargs_t;

void stop_child_thread(childargs_t **cargptr);
childargs_t *start_child_thread(char *name, int myrank, int servPort, enum client_protocol type, int maxbufsize); 
params_struct *child_process_get_mpibuf(childargs_t *args);

// Called by the parent for communicating with the child
int MPIX_chan(enum chan_comm_type type, const void *buf, int count, MPI_Datatype datatype,  int source, int tag, childargs_t *args,MPI_Request *request);
//int MPIX_chan(enum chan_comm_type type, const void *buf, int count, MPI_Datatype datatype, int source, childargs_t *args,MPI_Request *request);
int MPIX_chan_close(childargs_t *args);


#endif /* ADAPGREQUEST_H */
