/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
#ifndef NETUTILS_H
#define NETUTILS_H 1

#include <stdio.h>			/* for printf() and fprintf() */ 
#include <pthread.h>        /* for POSIX threads */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <fcntl.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>

#include <errno.h>
#include <signal.h>
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <netdb.h> 		/* for gethostbyname */

#include "debug.h"

#define FALSE 0
#define TRUE !FALSE

// apple/mac systems define MIN/MAX in sys/params.h, but linux systems do not
#ifndef __APPLE__
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))
#define MAX(a,b)        (((a) > (b)) ? (a) : (b))
#else
#include <sys/param.h>
#endif

/*
Start of some code for managing multiple connections with the child processes
    -- types of clients (for TCP there needs to be a way to break up the stream into messages)
    -- fixed size messages (read x bytes from stream)
    -- length,bytes from stream
    -- terminator '\n'
*/
enum client_protocol { UNK, STRING, DELIMITER, LENGTH, FIXED, LOGGER, CMD, ZMQ, JSON};  
char *client_protocol_string(enum client_protocol);

#define MAXLISTEN 5    // value given to listen, max listen queue

/* for send and recv message */
#define SPAREBUFFERSIZE 100
struct message_hdr
{
	//unsigned long int len;
	//unsigned long int tag;
	unsigned int len;
	unsigned int tag;	
};
typedef struct message_hdr message_hdr_t;

#include <pthread.h>
// for shared message pthreads message passing
typedef struct {
	pthread_mutex_t *mtx;
	pthread_cond_t *have_message;
	pthread_cond_t *empty_message;
	int isFull;
	int length;
	char *message;
} MGS_LOCK;
#define MAXLENGTHMGS  4096

// prototypes
int sharedmemmsg_send(MGS_LOCK *lock, int length, char *message);
int sharedmemmsg_recv(MGS_LOCK *lock, int length, char *message);
int sharedmemmsg_init(MGS_LOCK *lock);
int sharedmemmsg_free(MGS_LOCK *lock);

// prototypes
int recv_simple_message( int socket, int length, char *buffer);
int send_simple_message( int socket, char *buffer);
int recv_delimited_message( int socket, int length, char *buffer, char delimiter);
int send_delimited_message( int socket, const int length, const char *buffer, const char delimiter);

int send_message( int socket, int length, int tag, char *message);
int recv_message( int socket, int length, int *tag, char *buffer);

// split message protocol
int send_message_hdr( int socket, int length, int tag);
int send_message_payload( int socket, int length, char *message);

int get_msg_length(message_hdr_t *header);  // inline
int get_msg_tag(message_hdr_t *header);  // inline
int recv_msg_header( int socket, message_hdr_t *header);
int recv_msg_payload( int socket, message_hdr_t *header, char *buffer, unsigned int bsize);

unsigned long ResolveName(char name[]);
#define die(fmt, ...) die_def(__FILE__, __LINE__, fmt, ## __VA_ARGS__)
void die_def(char* file, int where, char *format, ...);
// socket
int check_socket_result(int errnumber, int socket, int bytes, int length);  
int check_socket_for_data(int socket, long timeout); // untested

// prototypes for utility functions
void hexPrintBuffer(unsigned char *buffer, int bufSize);  // character array defined before it make enum??
void byteSwap(char * b, int n);
void writeDouble(char buffer[], double value);
double readDouble(char buffer[], int *offset);
void writeInt(char buffer[], int value);
int readInt(char buffer[], int *offset);

// working select and accept 
void print_fd_set(fd_set *fds, int maxfd);
int safe_fd_set(int fd, fd_set* fds, int* max_fd);
int safe_fd_clr(int fd, fd_set* fds, int* max_fd);
int CreateTCPServerSocket(unsigned short port);  
int AcceptTCPConnection(int servSock);

#endif


