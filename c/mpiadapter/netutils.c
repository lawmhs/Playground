/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include "debug.h"
#include "netutils.h"

/**
* Split message, first send TAG and LENGTH and follow it 
* with the payload 
*/
int send_message_hdr( int socket, int length, int tag)
{
	// first send the length of message 
	message_hdr_t header;
	header.len = htonl(length);
	header.tag = htonl(tag);
	
	int numbytes = 0;

	if ( (numbytes = send(socket,&header,sizeof(message_hdr_t),0)) != sizeof(message_hdr_t))
	{	
		if ( (numbytes < 0) && (errno == EINTR) )
		{
			// EINTR while waiting for a O_EXCL lock or other request to clear a 
			// signal occured, are there other errors we should check?
			// man page says the interrupt would have occurred before any data was sent.  Should be safe to 
			// resend
			debug_r(DEBUG_WARNING,"%d: send_message()  --  EINTR, trying to send again\n", pthread_self());
			if ( (numbytes = send(socket,&header,sizeof(message_hdr_t),0)) != sizeof(message_hdr_t))
			{
				// second try failed just exit
				close(socket);
				debug_r(DEBUG_WARNING,"%d: send_message() -- header, sent a different number of bytes than expected, EINTR second try %d\n",pthread_self(),numbytes);	
				return 0;  // failed
			}
		}	else 
		{
			close(socket);
			debug_r(DEBUG_WARNING,"%d: send_message() -- header, sent a different number of bytes than expected %d -- errno %d\n",pthread_self(),numbytes,errno);
			return 0;  // failed
		}
	}
    return TRUE;
}

/**
* Split message, makes it possible to continually fill the socket buffer 
* with values until the length is reached
*/
int send_message_payload( int socket, int length, char *message)
{
	int numbytes = 0;
	
    // send the payload
    if ( (numbytes = send(socket,message,length,0)) != length )
    {			
        if ( (numbytes < 0) && (errno == EINTR) )
        {
            // EINTR while waiting for a O_EXCL lock or other request to clear a 
            // signal occured, are there other errors we should check?
            // man page says the interrupt would have occurred before any data was sent.  Should be safe to 
            // resend
            debug_r(DEBUG_WARNING,"%d: send_message()  --  EINTR, trying to send again\n",pthread_self());
            if ( (numbytes = send(socket,message,length,0)) != length )
            {
                // second try failed just exit
                close(socket);
                debug_r(DEBUG_WARNING,"%d: send_message() -- payload, sent a different number of bytes than expected, EINTR second try %d\n",pthread_self(),numbytes);
                return 0; // failed
            }
        } else if ( check_socket_result(errno,socket,numbytes,length) < 0 ) 
        {
            close(socket);
            return 0;   // failed
            // broken pipe, client has disappeared
            //debug_r(DEBUG_WARNING,"%d: send_message()  --  EPIPE, client has unexpectingly quit\n", pthread_self());
            //close(socket);
            //return FALSE;
        }
        {				
            close(socket);
            debug_r(DEBUG_WARNING,"%d: send_message() -- payload, sent a different number of bytes than expected %d, errno %d\n",pthread_self(),numbytes,errno);
            return 0; // failed
        }
    }
    return TRUE;
}



/**************************************************************************
*  Protocol for sending messages is send <length in bytes, message>
*   	-- TRUE for success
* 		-- FALSE for NOT success
*       If length is zero or message is NULL, then only the header is sent.
* 		This makes it possible to send only a TAG field, with no payload.
*//************************************************************************/
int send_message( int socket, int length, int tag, char *message)
{
	// first send the length of message 
	message_hdr_t header;
	header.len = htonl(length);
	header.tag = htonl(tag);
	
	int numbytes = 0;
		
	if ( (numbytes = send(socket,&header,sizeof(message_hdr_t),0)) != sizeof(message_hdr_t))
	{	
		if ( (numbytes < 0) && (errno == EINTR) )
		{
			// EINTR while waiting for a O_EXCL lock or other request to clear a 
			// signal occured, are there other errors we should check?
			// man page says the interrupt would have occurred before any data was sent.  Should be safe to 
			// resend
			debug_r(DEBUG_WARNING,"%d: send_message()  --  EINTR, trying to send again\n", pthread_self());
			if ( (numbytes = send(socket,&header,sizeof(message_hdr_t),0)) != sizeof(message_hdr_t))
			{
				// second try failed just exit
				close(socket);
				debug_r(DEBUG_WARNING,"%d: send_message() -- header, sent a different number of bytes than expected, EINTR second try %d\n",pthread_self(),numbytes);	
				return 0;  // failed
			}
		}	else 
		{
			close(socket);
			debug_r(DEBUG_WARNING,"%d: send_message() -- header, sent a different number of bytes than expected %d -- errno %d\n",pthread_self(),numbytes,errno);
			return 0;  // failed
		}
	}
	
	numbytes = 0;
	if ( (length != 0) && (message != NULL) )
	{
		// send the payload
		if ( (numbytes = send(socket,message,length,0)) != length )
		{			
			if ( (numbytes < 0) && (errno == EINTR) )
			{
				// EINTR while waiting for a O_EXCL lock or other request to clear a 
				// signal occured, are there other errors we should check?
				// man page says the interrupt would have occurred before any data was sent.  Should be safe to 
				// resend
				debug_r(DEBUG_WARNING,"%d: send_message()  --  EINTR, trying to send again\n",pthread_self());
				if ( (numbytes = send(socket,message,length,0)) != length )
				{
					// second try failed just exit
					close(socket);
					debug_r(DEBUG_WARNING,"%d: send_message() -- payload, sent a different number of bytes than expected, EINTR second try %d\n",pthread_self(),numbytes);
					return 0; // failed
				}
			} else if ( check_socket_result(errno,socket,numbytes,length) < 0 ) 
			{
				close(socket);
				return 0;   // failed
				// broken pipe, client has disappeared
				//debug_r(DEBUG_WARNING,"%d: send_message()  --  EPIPE, client has unexpectingly quit\n", pthread_self());
				//close(socket);
				//return FALSE;
			}
			{				
				close(socket);
				debug_r(DEBUG_WARNING,"%d: send_message() -- payload, sent a different number of bytes than expected %d, errno %d\n",pthread_self(),numbytes,errno);
				return 0; // failed
			}
		}
	}
	//debug_r(DEBUG_VERBOSE,"%d: send_message() -- tag=%d numbytes-payload-sent=%d and user specified length = %d\n", pthread_self(),tag,numbytes,length); 	
   	return TRUE;
}

/*
*  A simple send where the protocol is length and then a string
*  used for communicating with simple clients (control client)
*  length is size of buffer, which is assumed to be a null-terminated string
*   -- allows 0 sized messages, ie just sending the length and nothing else
*/

int send_simple_message( int socket, char *buffer)
{
	// first send the length of message 
	int length = 0;	
	int len = 0;
	int numbytes = 0;
	
	length = strlen(buffer);
	if ( (length < 0) ||  (length > MAXLENGTHMGS) )
	{
		debug_r(DEBUG_WARNING,"%d: send_simple_message() -- invalid length %d\n",pthread_self(),length);
		return FALSE;  
	}
	len = htonl(length);
		
	if ( (numbytes = send(socket,&len,sizeof(int),0)) != sizeof(int))
	{	
		if ( (numbytes < 0) && (errno == EINTR) )
		{
			// EINTR while waiting for a O_EXCL lock or other request to clear a 
			// signal occured, are there other errors we should check?
			// man page says the interrupt would have occurred before any data was sent.  Should be safe to 
			// resend
			debug_r(DEBUG_WARNING,"%p: send_message()  --  EINTR, trying to send again\n", pthread_self());
			if ( (numbytes = send(socket,&len,sizeof(int),0)) != sizeof(int))
			{
				// second try failed just exit
				close(socket);
				debug_r(DEBUG_WARNING,"%p: send_message() -- header, sent a different number of bytes than expected, EINTR second try %d\n",pthread_self(),numbytes);	
				return FALSE;  // failed
			}
		}	else 
		{
			close(socket);
			debug_r(DEBUG_WARNING,"%p: send_message() -- header, sent a different number of bytes than expected %d\n",pthread_self(),numbytes);
			return FALSE;  // failed
		}
	}
	
	numbytes = 0;
	if ( (length != 0) && (buffer != NULL) )
	{
		// send the payload
		if ( (numbytes = send(socket,buffer,length,0)) != length )
		{			
			if ( (numbytes < 0) && (errno == EINTR) )
			{
				// EINTR while waiting for a O_EXCL lock or other request to clear a 
				// signal occured, are there other errors we should check?
				// man page says the interrupt would have occurred before any data was sent.  Should be safe to 
				// resend
				debug_r(DEBUG_WARNING,"%p: send_simple_message()  --  EINTR, trying to send again\n",pthread_self());
				if ( (numbytes = send(socket,buffer,length,0)) != length )
				{
					// second try failed just exit
					close(socket);
					debug_r(DEBUG_WARNING,"%p: send_simple_message() -- payload, sent a different number of bytes than expected, EINTR second try %d\n",pthread_self(),numbytes);
					return FALSE; // failed
				}
			} else if ( check_socket_result(errno,socket,numbytes,length) < 0 ) 
			{
				close(socket);
				return FALSE;   // failed
			}
			{				
				close(socket);
				debug_r(DEBUG_WARNING,"%p: send_simple_message() -- payload, sent a different number of bytes than expected %d, errno %d\n",pthread_self(),numbytes,errno);
				return FALSE; // failed
			}
		}
	}
	//debug_r(DEBUG_VERBOSE,"%d: send_simple_message() -- numbytes-payload-sent=%d and length = %d\n", pthread_self(),numbytes,length); 	
   	return TRUE;
}

/*
*  A simple recieve where the protocol is length and then a string
*  used for communicating with simple clients (control client)
*  length is size of buffer
*/

int recv_simple_message( int socket, int length, char *buffer)
{
	int bytesRecved = 0;
	int totalRecved = 0;	
	int len = 0;
	int mlength = 0;   // length from socket
	
	//debug_r(DEBUG_WARNING,"RECV-MESSAGE(tid=%d)  ----------------------------- receive called on socket %d to receive %d bytes\n", pthread_self(),socket,length);
	
	while ( totalRecved < sizeof(int) )
	{
		if ( (bytesRecved = recv(socket, &mlength, sizeof(int),0)) < 0 )
		{
			if ( check_socket_result(errno,socket,bytesRecved,length) < 0 ) return -1;
			debug_r(DEBUG_WARNING,"%p: recv_simple_message() -- trying this again\n", pthread_self());
			
		} else if ( bytesRecved == 0 )
		{
			debug_r(DEBUG_WARNING,"%p: recv_simple_message() -- client closed\n", pthread_self());
			return 0;
		} else
		{
			totalRecved += bytesRecved;
		}
		//debug_r(DEBUG_WARNING,"RECV-MESSAGE(tid=%d)  -- received %d bytes out of %d\n", pthread_self(),bytesRecved,totalRecved);
	}
	len = ntohl(mlength);

	//debug_r(DEBUG_WARNING,"Recv-Message: len=%d and user specified length = %d\n",len,length);
	
	if ( len > length ) 
	{
		debug_r(DEBUG_WARNING,"%p: recv_simple_message() -- message too long to fit in buffer (len=%d,length=%d)\n",pthread_self(),len,length);
		return 0;
	}
	
	// zero out the receive buffer
	memset(buffer,0,length);
	bytesRecved = 0;
	totalRecved = 0;	
	// now receive bytes from the socket
	while ( totalRecved < len )
	{
		if ( (bytesRecved = recv(socket, buffer+totalRecved, len-totalRecved,0)) < 0 )
		{
			if ( check_socket_result(errno,socket,bytesRecved,len) < 0 ) return -1;
		} else if ( bytesRecved == 0 )
		{	
			return -1;
			// socket closed prematurely, must have been shutdown on server side
		}
		else
			totalRecved += bytesRecved;
	}
	//debug_r(DEBUG_WARNING,"Recv-Message: RETURNING len=%d \n",totalRecved);
	return totalRecved;
}

/*
*  Protocol for receiving messages is recv <length in bytes, message>
*  It returns the number of bytes received.
*  Once it receives the length it will try to copy it to the buffer provided. If the 
*  length exceeds the buffer it will return -1 for problem, 0 for null message, else number
*  of bytes copied into the buffer, if larger than length -- truncated
*/
int recv_message( int socket, int length, int *tag, char *buffer)
{
	message_hdr_t header;
	
	int bytesRecved = 0;
	int totalRecved = 0;
	int long len = 0;
	int mlength = 0;
	
	//debug_r(DEBUG_VERBOSE,"%d: recv_message() -- receive called on socket %d to receive %d bytes\n", pthread_self(),socket,length);
	
	while ( totalRecved < sizeof(message_hdr_t) )
	{
		if ( (bytesRecved = recv(socket, &header, sizeof(message_hdr_t),0)) < 0 )
		{
			if ( check_socket_result(errno,socket,bytesRecved,length) < 0) return -1;
		} else if ( bytesRecved == 0 )
		{
			debug_r(DEBUG_WARNING,"%d: recv_message() -- client closed\n", pthread_self());
			return 0;
		} else
		{
			totalRecved += bytesRecved;
		}
		//printf("RECV-MESSAGE(tid=%d)  -- received %d bytes out of %d\n", pthread_self(),bytesRecved,totalRecved);
	}
	len = ntohl(header.len);
	*tag = ntohl(header.tag);
	//printf("Recv-Message:  tag=%d len=%d and user specified length = %d\n",*tag,len,length);
	
	if ( buffer == NULL ) 
	{
		// not expecting a payload, only requested the message tag
		return 0;
	}
	
	if ( len <= 0 )
	{
		debug_r(DEBUG_WARNING,"%d: recv_message() -- client indicated a zero-length message tag=%d,len=%d\n", pthread_self(),*tag,len);
		return 0;   // no data to receive, buffer left unchanged
	} else 
	{
		// zero out the receive buffer
		memset(buffer,0,length);
		bytesRecved = 0;
		totalRecved = 0;
		mlength = MIN(length,len);
		
		// now receive bytes from the socket
		while ( totalRecved < mlength )
		{
			if ( (bytesRecved = recv(socket, buffer+totalRecved, mlength-totalRecved,0)) < 0 )
			{
				if ( check_socket_result(errno,socket,bytesRecved,length) < 0 ) return -1;
			} else if ( bytesRecved == 0 )
				break;      // socket closed prematurely, must have been shutdown on server side
			else
				totalRecved += bytesRecved;
		}
		// need to determine whether the message needs to be truncated
		if ( totalRecved < len ) 
		{
			// need to read and throw away extra bytes
			char sparebuffer[SPAREBUFFERSIZE];
			while ( totalRecved < len ) 
			{
				if ( (bytesRecved = recv(socket, sparebuffer, SPAREBUFFERSIZE,0)) < 0 )
				{
					if ( check_socket_result(errno,socket,bytesRecved,length) < 0 ) return -1;
				} else if ( bytesRecved == 0 )
					break;      // socket closed prematurely, must have been shutdown on server side
				else
					totalRecved += bytesRecved;
			}
		}
	}
	return totalRecved;
}

int check_socket_result(int errnumber, int socket, int bytes, int length)
{
debug_r(DEBUG_VERBOSE,"%d: check_socket_result() \n", pthread_self());
	if ( errnumber == EINTR ) 
	{
		// caused by an interrupt by a signal handler we should go back and do it again
		debug_r(DEBUG_VERBOSE,"%p: check_socket_result() -- INTERRUPT signal %d, recved %d bytes of %d bytes\n", pthread_self(),socket,bytes,length);
	} else if ( (errnumber == EPIPE) || (errnumber == EBADF) || (errnumber == ECONNRESET) )
	{
		debug_r(DEBUG_VERBOSE,"%p: check_socket_result() -- EPIPE/EBADF signal %d, recved %d bytes of %d bytes\n", pthread_self(),socket,bytes,length);
		return -1;
	}
	{
		debug_r(DEBUG_VERBOSE,"%p: check_socket_result() -- premature end of message errnumber %d\n", pthread_self(),errnumber);
		return -1;
		//mpibail((int)pthread_self(),"premature end of message %d\n",errnumber);
	}
	return 0;
}

// unused 
int check_socket_for_data(int socket, long timeout)
{
	fd_set socketSet;
	struct timeval selTimeout;
	int res=0;
	
	FD_ZERO(&socketSet);
	FD_SET(socket,&socketSet);
	// timeout
	selTimeout.tv_sec = timeout;
	selTimeout.tv_usec = 0;
	// find the max-socket 
	if ( select(socket+1,&socketSet, NULL, NULL, &selTimeout) == 0 )
	{
		// new message from the client
		//printf("There is no data waiting from the client\n");
	} else {
		 if ( FD_ISSET(socket,&socketSet) ) 
		 	res = 1;
			//printf("There is no data waiting from the client\n");
		//else 
			//printf("There is no data waiting from the client, not ISSET not set\n");
	}
	debug_r(DEBUG_VERBOSE,"%d: check_socket_for_data() -- returning %d\n",pthread_self(),res);
	return res;
}

// Assumed to be in network order
inline int get_msg_length(message_hdr_t *header) { return header->len; }
inline int get_msg_tag(message_hdr_t *header) { return header->tag; }
int recv_msg_header( int socket, message_hdr_t *header)
{
	int bytesRecved = 0;
	int totalRecved = 0;
	int len, tag;
	
	while ( totalRecved < sizeof(message_hdr_t) )
	{
		//if ( (bytesRecved = recv(socket, header, sizeof(message_hdr_t),0)) < 0 )
		// bug: problem whenever the recv is executed twice
		if ( (bytesRecved = recv(socket, header+totalRecved, sizeof(message_hdr_t)-totalRecved,0)) < 0 )		
		{
				//printf("recv_msg_header() --- received %d out of %d bytes\n",bytesRecved,totalRecved);
				if ( check_socket_result(errno,socket,bytesRecved,sizeof(message_hdr_t)) < 0 ) return -1;
		} else if ( bytesRecved == 0 )
		{
			debug_r(DEBUG_WARNING,"%d: recv_msg_header() -- client closed\n", pthread_self());			
			return -1;
		} else
		{
			totalRecved += bytesRecved;
		}
	}
	len = ntohl(header->len);  // convert back into machine order
	tag = ntohl(header->tag);  // convert back into machine order
	header->len = len;
	header->tag = tag;
	debug_r(DEBUG_VERBOSE,"%d: recv_msg_header() -- (bytes %d) len--tag %d--%d  converted to %d--%d\n",
								pthread_self(),totalRecved,len,tag, header->len,header->tag);
	return totalRecved;
}

// receive the payload of the message from the given header 
// RETURN -1 when buffer is too small to receive the message
int recv_msg_payload( int socket, message_hdr_t *header, char *buffer, unsigned int bsize)
{
	int bytesRecved = 0;
	int totalRecved = 0;

	debug_r(DEBUG_VERBOSE,"%d: recv_mgs_rest()  --  tag=%d len=%d \n",pthread_self(),header->tag,header->len);

	if ( (buffer == NULL) || (header == NULL) || (header->len > bsize) ) 
	{
		// need some place to put the payload or not enough buffer space to receive message.
		return -1;   
	}
	
	// zero out the receive buffer
	memset(buffer,0,header->len);
	bytesRecved = 0;
	totalRecved = 0;
	
	// now receive bytes from the socket 
	while ( totalRecved < header->len )
	{
		if ( (bytesRecved = recv(socket, buffer+totalRecved, header->len-totalRecved,0)) < 0 )
		{
			if ( check_socket_result(errno,socket,bytesRecved,header->len) < 0 ) return -1;
		} else if ( bytesRecved == 0 )
			break;      // socket closed prematurely, must have been shutdown on server side
		else
			totalRecved += bytesRecved;
	}
	return totalRecved;
}

/* hexPrint translation table */
char hexTranslate[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
/*little utility routine to print out the hex contents of a buffer */
void hexPrintBuffer(unsigned char *buffer, int bufSize) 
{
    /* loop index */
    int i;
    /* print one character at a time */
    for (i = 0; i < bufSize; i++)
        printf("%c%c ", hexTranslate[(buffer[i] & 0xF0) >> 4], hexTranslate[buffer[i] & 0x0F]);
} /*hexPrintBuffer() */

/**************************************************************************
*  Protocol for sending messages is send <length in bytes, message>
*   	-- TRUE for success
* 		-- FALSE for NOT success
*       If length is zero or message is NULL, then only the header is sent.
* 		This makes it possible to send only a TAG field, with no payload.
*************************************************************************/
void die_def(char* file, int where, char *format, ...)
{
        va_list args;
        fprintf(stderr,"(%s:%u): ",file,where);
        va_start(args,format);
        vfprintf(stderr,format,args);
        va_end(args);
        exit(-1);   //otherwise just returns 
}


unsigned long ResolveName(char name[])
{
	struct hostent *host;

	if ((host= gethostbyname(name)) == NULL)
	{
		die("gethostbyname() failed\n");
	}
	return *((unsigned long *) host->h_addr_list[0]);
}


void writeDouble(char buffer[], double value){
  int i;
  double *pt = &value;
  byteSwap((char*)pt, sizeof(double));
  for(i=0;i<sizeof(double);i++)
    buffer[i] = *((char*)pt+i );
}


double readDouble(char buffer[], int *offset){
  int len = sizeof(double);
  byteSwap(buffer+(*offset), len);
  double* value = ((double*)(buffer+(*offset)));
  (*offset) += len;
  return *value;
}


void writeInt(char buffer[], int value){
  int i;
  int *pt = &value;
  byteSwap((char*)pt, sizeof(int));
  for(i=0;i<sizeof(int);i++)
    buffer[i] = *((char*)pt+i );
}

int readInt(char buffer[], int *offset){
  int len = sizeof(int);
  byteSwap(buffer+(*offset), len);
  int* value = ((int*)(buffer+(*offset)));
  (*offset) += len;
  return *value;
}


void byteSwap(char * b, int n){
  char temp;
  int i = 0 ;
  int j = n - 1;
  while(i<j){
    temp = *(b+i);
    *(b+i) = *(b+j);
    *(b+j) = temp;
    i++;
    j--;
  }
}

/**************************************************************************
*
*  Helper routines for accepting and working with FDSETs ....
*
*************************************************************************/

/* add a fd to fd_set, and update max_fd */
int safe_fd_set(int fd, fd_set* fds, int* max_fd) {
    assert(max_fd != NULL);
    FD_SET(fd, fds);
    if (fd > *max_fd) {
        *max_fd = fd;
    }
    return 0;
}

/* clear fd from fds, update max fd if needed */
int safe_fd_clr(int fd, fd_set* fds, int* max_fd) {
    assert(max_fd != NULL);
    FD_CLR(fd, fds);
    if (fd == *max_fd) {
        (*max_fd)--;
    }
    return 0;
} 

void print_fd_set(fd_set *fds, int maxfd)
{
    int i;
    printf("FDSET (%d): ",maxfd);
    for (i=0;i<=maxfd;i++) {
        if (FD_ISSET(i,fds)) printf("[[%d]],",i); 
    } 
    printf("\n");
}

int CreateTCPServerSocket(unsigned short port)
{
    int sock;                        /* socket to create */
    struct sockaddr_in echoServAddr; /* Local address */

    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        mpibail(getpid(),"SERVER -- open socket() failed for port %d", port);

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */

    int on=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(int));

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        mpibail(getpid(),"SERVER -- bind() failed for port %d\n",port);

    /* Mark the socket so it will listen for incoming connections */
    if (listen(sock, MAXLISTEN) < 0)
        mpibail(getpid(),"SERVER -- listen() failed for port %d\n",port);

    //printf("Created socket fine %d\n",sock);
    return sock;
}

// Derived from Donahoo and Calvert Page 66, Pocket Socket Guide
int AcceptTCPConnection(int servSock)
{
        int clntSock = -1;                      /* Socket descriptor for client */
        struct sockaddr_in clntAddr;            /* Client address */
        unsigned int clntLen;                   /* Length of client address data structure */

        // set of socket descriptors for select() call
        //fd_set sockets;

        /* Set the size of the in-out parameter, client socket address */
        clntLen = sizeof(clntAddr);

        //debug_r(DEBUG_START,"%d: AcceptTCPConnection() ... Waiting for client to connect\n",pthread_self());
        clntSock = accept(servSock, (struct sockaddr *) &clntAddr,&clntLen);
        if ( clntSock < 0 )
        {
                if (errno == EINTR)
                {
                        // EINTR while waiting for a O_EXCL lock or other request to clear a 
                        // signal occured, are there other errors we should check?
                        debug_r(DEBUG_WARNING,"AcceptTCPConnection -- accept() caused EINTR, signal interrupt \n");
                        return -1;
                } else if ( errno == ECONNABORTED ) {
                        debug_r(DEBUG_WARNING,"AcceptTCPConnection -- accept() connection aborted \n");
                        return -1;                  
                } else
                {
                        // another type of error
                        debug_r(DEBUG_WARNING,"AcceptTCPConnection -- accept() error on accept call errno %d -- servSock=%d\n",errno,servSock);
                        return -1;
                }
        }

   /* clntSock is connected to a client! */
  //debug_r(DEBUG_START,"%p: AcceptTCPConnection():  Handling new client from %s\n",pthread_self(),inet_ntoa(clntAddr.sin_addr));
  return clntSock;
}

/**************************************************************************
*
*  Additional protocols for sending and receiving messages ....
*
*************************************************************************/

char  *client_protocol_string(enum client_protocol cp)
{
    switch(cp) {
        case STRING: return("string");
        case DELIMITER: return("delimiter");
        case LENGTH: return("length");
        case FIXED: return("fixed");
        case LOGGER: return("logger");   
        case CMD: return("cmd");  
        case ZMQ: return("zmq"); 
        case JSON: return("json"); 
        default: return("UNKNOWN");    
    }
}

int recv_delimited_message( int socket, int length, char *buffer, char delimiter)
{
    if ( (buffer == NULL) || (length==0) ) return -1;
    char *ptr, *eptr;   // pointer to curr and poiner to end

    for (ptr = buffer, eptr = buffer + length -1;;) {
        char c = '\0';
        int r = recv(socket,&c,1,0);
        if ( r<0 ) return -1;
        if ( r==0 ) return 0;
        *ptr++ = c;
        if ( c == delimiter ) break;  
    }
    *ptr = '\0'; 
    return ptr-buffer;   
}

int send_delimited_message( int socket, const int length, const char *buffer, const char delimiter)
{
    if ( (buffer == NULL) || (length==0) ) return -1;
    int nbytes=0;    
    if ( buffer[length-1] != delimiter) {
        debug_r(DEBUG_WARNING,"send_delimited_message() -- socket %d,  missing delimiter on message \n",socket);
        // send as ONE message, exceptional case okay to be slow
        char *tmp = malloc(length+1); 
        memcpy(tmp,buffer,length);
        tmp[length] = delimiter;
        nbytes = write(socket,tmp,length+1);  
    } else   
        nbytes = write(socket,buffer,length);  
    return nbytes;   
}


    
    
