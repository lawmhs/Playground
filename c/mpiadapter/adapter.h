/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
#ifndef MPIADAPTER_H
#define MPIADAPTER_H

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
#include <assert.h>

#include <mpi.h>
#include "chan.h"
#include "netutils.h"
#include "uthash/uthash.h"

#define MAXCLIENTS 5
#define FALSE 0
#define TRUE !FALSE

// timebuffer size, must be greater than 26
#define TIMEBUFSIZE 28
#define TIMEOUTTIME 30   // 30 secs
#define MAXRECVBUF 1024  

/*
Start of some code for managing multiple connections with the child processes
    -- types of clients (for TCP there needs to be a way to break up the stream into messages)
    -- fixed size messages (read x bytes from stream)
    -- length,bytes from stream 
    -- terminator '\n' 
*/
enum chan_comm_type { SEND, RECV, LOG, DISPOSE };   // pchannel for MPI process 
static const char * chan_comm_type_strings[] = { "SEND", "RECV", "LOG",  "DISPOSE" }; 

//enum client_type { UNK, STRING, LENGTH, FIXED, LOGGER, CMD};  //  three possible types of clients  --- NOT CURRENTLY USED
//static const char * client_type_strings[] = { "UNK", "STRING", "LENGTH", "FIXED", "LOGGER", "CMD" };

enum client_state { CLOSED, OPEN };  // when open, child is receiving from client OR doing a reply
static const char * client_state_strings[] = { "CLOSED", "CRECV", "CSEND", "OPEN", "CREPLY"};

// used by child for managing client connections
typedef struct {
    int id;                      // key for the hash table can fd
    int tag;
    enum client_protocol type;       // type of client may not need this and only need the protocol 
    enum client_state state;     // state of the client with respect to the protocol
    int fd;                      // file descriptor
    long int seqnum;             // sequence number 
    UT_hash_handle hh; /* makes this structure hashable */
} connection_t;

// prototypes for connection management
int add_connection(connection_t **c, int id, int fd, enum client_protocol type, enum client_state state);
int setstate_connection(connection_t **c,int id,enum client_state state);
int settypestate_connection(connection_t **c,int id,enum client_protocol type, enum client_state state);

int incseq_connection(connection_t *s);
int getid_connection(connection_t *s);
int getfd_connection(connection_t *s);  // getter for fd
connection_t *next_connection(connection_t *c, connection_t **cur);   // iterator  set cur MUST be set to NULL at start
int numconnection(connection_t **c);
int getstatelist(connection_t *c, enum client_protocol type, enum client_state state,  int list[], int max);
int getopenlist(connection_t *c,int list[], int max);
int getconnection(connection_t **c,int id,connection_t *t);
int getfd(connection_t **c,int id);

int remove_connection(connection_t **c, int id);
int test_connections(connection_t *c,int *read_fd_set);
void print_connections(connection_t *c);

void deleteclose_allconnections(connection_t **c);

#endif /* MPIADAPTER_H */
