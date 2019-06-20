/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
#include "mpi.h"
#include <stdlib.h>
#include "grequest.h"

#include <Python.h>

#define MAXMSGSIZE 100
#define PORT 5555

int main(int argc, char *argv[])
{
   	int rank;
	MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;
    MPI_Request srequest;
	void *cargs = NULL;
	int tag = 0;

    set_debug_level(DEBUG_WARNING | DEBUG_START | DEBUG_VERBOSE | DEBUG_SERVER);
    enum client_protocol proto = DELIMITER;
    //enum client_protocol proto = LENGTH;

    // create an external input channel to receive data from an external source
    printf("Starting child thread .......\n");
    cargs = start_child_thread("otherone",rank,PORT,proto,MAXMSGSIZE);
    printf("Child thread is executing .......\n");
    
    char *msg = calloc(sizeof(char),MAXMSGSIZE);
    strcpy(msg,"hello\n");
    int len = strlen(msg);

	// HERE IS WHERE YOU READ OR WRITE TO THE CHILD
	int times=5;
	int count = 0;

	while (times--) 
        if (0)
        {   

            MPIX_chan(RECV,msg,MAXMSGSIZE,MPI_BYTE,status.MPI_SOURCE,tag,(childargs_t *)cargs,&srequest);
            MPI_Wait(&srequest,&status);
            MPI_Get_count( &status,MPI_CHAR,&count);
            printf("** %d ** MPI RECV ******** msg SOURCE:%d TAG:%d COUNT:%d ->%s<-\n",times,status.MPI_SOURCE,status.MPI_TAG,count,msg);
        } else {

            memset(msg, 0, MAXMSGSIZE);
            strcpy(msg,"hello\n");            
            MPIX_chan(SEND,msg,len,MPI_BYTE,99,23,(childargs_t *)cargs,&srequest);
            MPI_Wait(&srequest,&status);
            MPI_Get_count( &status,MPI_CHAR,&count);
            printf("** %d ** MPI SEND ******** msg SOURCE:%d TAG:%d COUNT:%d\n",times,status.MPI_SOURCE,status.MPI_TAG,count);
        }
	// here is where you stop the child
    printf("Stopping child thread .......\n");
    stop_child_thread((childargs_t **)&cargs);
	MPI_Finalize();
	exit(0);
}
