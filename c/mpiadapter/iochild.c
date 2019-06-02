/*****************************************************
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
*
*   Functions for using a child process with generalized requests
*   Simple protocol right now is as follows:
*        -- client connects sends IN/OUT with or without a eol (stripped) on first read
*        -- outgoing data is output as specified by the MPI send command
*        -- incoming data is read (no attempt to parse the data -- which may not be satisfactory since
*           we may never know how much data we will receive on each TCP recv call.  Can still parse on
*           the MPI side, just read as bytes.
*
*   Possible configurations could be 
*       -- remove IN/OUT and simply read/write to all OPEN clients
*       -- have a READ only or WRITE only client, client can only be one or the other
*       -- for reading messages we can use LENGTH+PAYLOAD for sending and receiving, for easy parsing
*       -- add a json parser to input/output json or other types of messaging libs: protobufs, avro, thrift,
*          CBOR and amazon's ION, messasgepack .... and of course MPI message types. 
******************************************************/
#include "grequest.h"

/*****************************************************
*   Generalized Request Functions

    The query function extracts information from the datatype and sets status fields.  It is given the extra-state
    field.  The extra-state data is allocated in parent and part of the grequest start function.  The query_function
    is invoked when the child completes the operation.
*******************************************************/
//#define NDEBUG  to turn assertions off
//static const int GREQDEBUG = 0;
static const char dheader[] = ".......................................";
static const char lheader[] = "...............................";

void printerror(int err) {
        switch (err) {
            case EBADF: perror("EBADF"); break;
            case EAGAIN: perror("EAGAIN"); break;
            case EINVAL: perror("EINVAL"); break;
            default:
                printf("%d Unknown Error",err);
        }
} 

static int send_protocol_message(connection_t *conn, const void *buf, int size, int tag)
{
    int nbytes = 0;
    if (conn->type == DELIMITER) {
        // directly write bytes to stream, no additional processing, but should check for delimiter? What to do if NOT?
        nbytes = send_delimited_message(conn->fd,size,buf,'\n');  
    } else if (conn->type == LENGTH) {
        nbytes = send_message(conn->fd,size,tag,(char *)buf);     // returns either true or false 
        if (nbytes) nbytes = size;   // return number of bytes on success
        else nbytes = -1;    // return -1 for failure
    }
    else {
        debug_r(DEBUG_WARNING,"ERROR ... ERROR connection is corrupt\n");
        assert(0);
    }
    return nbytes;
}

static int receive_protocol_message(connection_t *conn, char *buf, int size, int *tag)
{
    int nbytes = 0;
    if (conn->type == DELIMITER) {
        nbytes = recv_delimited_message(conn->fd,size,buf,'\n');
        *tag = 0;
    } else if (conn->type == LENGTH) {
        nbytes = recv_message(conn->fd,size,tag,buf);  // length exceeds the buffer length -1, 0 for null message else nbytes
        // what about null messages         
    } else {
        debug_r(DEBUG_WARNING,"ERROR ... ERROR connection is corrupt\n");
        assert(0);
    }
    return nbytes;
}

static void acceptnewconnection(connection_t **c, int tcpfd, enum client_protocol ptype)
{
    /* new connection has arrived -- add to connection table to process it */
    //debug_r(DEBUG_SERVER,"%s 1 (ACCEPT)  client connected fd=%d\n",dheader,tcpfd);
    if ( numconnection(c) == MAXCLIENTS ) {
        debug_r(DEBUG_WARNING,"WARNING ..... attempt to connect to many clients ... closing the connection %d\n",tcpfd);
        close(tcpfd);
        return;
    }
    // find a free connection slot for this connection
    if ( !add_connection(c,tcpfd,tcpfd,ptype,OPEN) ) {
        printf("acceptnewconnection() --- ERROR ERROR unable to add connection\n"); 
        assert(FALSE);                       
    } 
    return;
}

static int doclientmsg(childargs_t *args, connection_t *conn, params_struct **params)
{
    int nbytes = 0;
    //int bufsize = args->receivebuffsize;
    params_struct *recv_pending_params = *params;   // params structure allocated in Grequest 
    assert(params != NULL);  // this is only called when MPI is trying to receive a message            
  
    // get the maximum message size that can be read and read into buf
    int size;
    MPI_Type_size(recv_pending_params->datatype,&size);  
    
    // data available from the client --> sending data to the MPI process 
    debug_r(DEBUG_VERBOSE,"(%s:%d)%s C1 CLIENT--->  trying to read message [client:%d,fid:%d,type:%s], nbytes=%d \n",
            __FILE__,__LINE__,dheader,conn->id,conn->fd,client_protocol_string(conn->type),recv_pending_params->count*size);
    //char *buf = malloc(sizeof(char)*recv_pending_params->count*size);  // allocate a buffer, buffer has not been allocated by grequest  
    // receive data from a client -- connection data structure, buffer holding data, the number of bytes, and the tag value (not useful in all circumstances )  
    nbytes = receive_protocol_message(conn,(char *)recv_pending_params->buf,recv_pending_params->count*size,&(recv_pending_params->tag));    
    //nbytes = recv_delimited_message(conn->fd,recv_pending_params->count*size,buf,'\n');

    debug_r(DEBUG_VERBOSE,"(%s:%d)%s  C2 CLIENT-->  after reading msg [client:%d,fid:%d,type:%s], bufsize=%d,nbytes=%d ->%s<-\n",
        __FILE__,__LINE__,dheader,conn->id,conn->fd,client_protocol_string(conn->type),recv_pending_params->count*size,nbytes,(char *)recv_pending_params->buf);
    
    if ( nbytes <= 0 ) {
        // connection was closed or there is a problem with link
        debug_r(DEBUG_WARNING,"(%s:%d)%s (CMD:CRECV) -- Client closed or EBADC error on connection for %d\n",
            __FILE__,__LINE__,dheader,conn->id);
        return(FALSE);
    } else {   
        incseq_connection(conn);    // received new msg, increment seqnum  
        //recv_pending_params->buf = buf;  this is wrong the buffer is allocated by application and shared 
        recv_pending_params->nbytes = nbytes;                                                                                
        recv_pending_params->seqnum = conn->seqnum;  // set sequence number in extra for MPI
        recv_pending_params->client = conn->id;      // set ID in extra for MPI
        //printf("%s 3(CMD:CRECV) CHILD in Child -- calling complete %p\n",dheader,recv_pending_params);
        MPI_Grequest_complete(*(recv_pending_params->request));  // completes the recv params request
        debug_r(DEBUG_VERBOSE,"(%s:%d)%s 3(CMD:CRECV) --  sent to MPI seqnum=%ld,msg:%s\n",__FILE__,__LINE__,dheader,conn->seqnum,(char *)recv_pending_params->buf);
        *params = NULL;  // reset MPI recv structure to NULL --  buffer freed by grequest
    }
    return(TRUE);
}

static int doselect(fd_set *fd_set,int maxfd, childargs_t *args,struct timeval *timeout)
{
    int ret=0;
    //print_fd_set(fd_set,maxfd);  // should be three SERVER-SOCK, PIPE-SOCK, and CONNECTED-CLIENT
    //sleep(1); // ALAN
    if ( (ret = select(maxfd+1,fd_set,NULL,NULL,timeout)) < 0 )
    {
        printerror(errno);
        mpibail(args->myrank,"[%s] ERROR bad return from select ----- terminating child ret=%d,%d\n ................\n",args->myname,ret,errno);            
    } else if ( ret == 0) {   
        // select returned 0 -- timeout
        debug_r(DEBUG_VERBOSE,"%s %d:TIMEOUT:  child_thread(): timeout %d ----- trying select again\n",dheader,args->myrank,ret);
        return FALSE;  // select timed out nothing to do 
    } else ;    
    //print_fd_set(fd_set,maxfd);
    return TRUE;
}

/****************************************
*   Child Process
*****************************************/
#define MAXCLIENTS 5   // could just be 1 -- how many to wait
#define BUFSIZE 1024
/* child thread that does the request and completes the operation */
void *child_thread(void *ptr)
{
    connection_t *c = NULL;  // manage the connections 
    childargs_t *args = (childargs_t *)ptr;
    int finished = FALSE;
    int servSock;
    int maxfd = 0;

    /* Initialize a timeout data structure. */
    struct timeval timeout;
    timeout.tv_sec = TIMEOUTTIME;
    timeout.tv_usec = 0;
    
    // make a socket, initialize the set of active sockets
    servSock = CreateTCPServerSocket(args->port);   // listens
    debug_r(DEBUG_VERBOSE,"(%s:%d)%s 0. SOCKET-CREATED child_thread() -- port:%d, socket:%d \n",__FILE__, __LINE__,dheader,args->port,servSock);
    
    // set up for the select call -- Initialize the file descriptor set to server socket and pipe socket
    fd_set active_fd_set;    // active set of FDs used to initialize the read_set 
    fd_set read_fd_set;      // read set given to select 
    FD_ZERO(&active_fd_set); 
    FD_SET(servSock, &active_fd_set);   // add server socket    
    maxfd = servSock;    // currently the only socket  -- need to keep this pointing to max 
    safe_fd_set(args->fdpipe,&active_fd_set,&maxfd);    // adds pipe fd and resets maxfd if necessary
    
    // saves a pointer to the structure shared between MPI and the Child, freed once the MPI request has been completed
    params_struct *recv_pending_params = NULL;  // ptr  when there is an MPI recv buffer waiting to be filled

    // loop waiting for a return --- should not fail
    int tcpfd = -1;
    while ( (tcpfd = AcceptTCPConnection(servSock)) < 0) {
        debug_r(DEBUG_WARNING,"WARNING ..... unable to accept connection Accept returned %d\n",tcpfd);
    } 
    
    // do nothing until at least ONE client connects
    debug_r(DEBUG_VERBOSE,"(%s:%d)%s 1. SELECT ACCEPT -- first client connected %d\n",__FILE__, __LINE__,dheader,tcpfd);
    acceptnewconnection(&c,tcpfd,args->protocol);
    //safe_fd_set(tcpfd,&active_fd_set,&maxfd);   // add client to active FD set  --- added below
        
    while (!finished) {  
        read_fd_set = active_fd_set;  // basic two FDs
        if ( recv_pending_params ) {
            // only add in the clients when there is a pending MPI request 
            connection_t *citem = NULL;
            while ( next_connection(c,&citem) ) {   
                if ( citem->state == OPEN )  safe_fd_set(citem->fd,&read_fd_set,&maxfd);                 
            }
        } 

        /************
            conditions on the select: (driven from the MPI side)
                accepting a new client:
                signal of read/write request from MPI 
                --> if read request then wait for a client to send some data 
                --> if write request then write data to client 
        *************/
        debug_r(DEBUG_VERBOSE,"(%s:%d)%s START-WHILE end of loop checking for messages \n",__FILE__, __LINE__,lheader);    
        if ( !doselect(&read_fd_set,maxfd, args,&timeout) )  continue;   // time-out, do again 
        
        // ACCEPT NEW CONNECTION received one or more read request -- need to iterate over all of the sockets in readset 
        int tcpfd = -1;
        if (FD_ISSET(servSock, &read_fd_set)) {   
            /* NEW CONNECTION ----  if a connection request has arrived, process it */
            if ( (tcpfd = AcceptTCPConnection(servSock)) < 0) {
                debug_r(DEBUG_WARNING,"WARNING ..... unable to accept connection Accept returned %d\n",tcpfd);
            } else {
                debug_r(DEBUG_VERBOSE,"(%s:%d)%s A SELECT ACCEPT -- new client connected %d\n",__FILE__, __LINE__,dheader,tcpfd);
                acceptnewconnection(&c,tcpfd,args->protocol);
                safe_fd_set(tcpfd,&active_fd_set,&maxfd);   // add client to active FD set 
            }            
        }
                
        // PIPE --- MPI request is in the pipe, get params structure ..
        if (FD_ISSET(args->fdpipe, &read_fd_set)) {  
            debug_r(DEBUG_VERBOSE,"(%s:%d)%s A. MPI REQUEST\n",__FILE__,__LINE__,dheader); 
            recv_pending_params = child_process_get_mpibuf(args);  // drain the pipe and get the buffer for exchanging data with MPI process
            assert(recv_pending_params);     // should no longer be NULL
        } 
        
        // What happens when recv_pending_params is ZERO but something is being received on an FD? 
                
        // MPI request  --- SEND 
        if ( recv_pending_params && (recv_pending_params->ctype == SEND) ) {
            int size;
            int nbytes = 0;
            params_struct *pending_params = recv_pending_params; 
            MPI_Type_size(pending_params->datatype,&size); 
            debug_r(DEBUG_VERBOSE,"(%s:%d)%s A. MPI-SEND-->CLIENT MPI sending msg to client  [SOURCE:%d,seqnum:%ld,size=%d]\n",
                                            __FILE__,__LINE__,dheader,pending_params->client,pending_params->seqnum,size*pending_params->count);
            // send to all open connections 
            connection_t *t = NULL;
            //print_connections(*c);
            while ( next_connection(c,&t) ) {  
                // writing the bytes to socket                                                                                                                  
                //nbytes = write(t->fd,pending_params->buf,size*pending_params->count);  
                nbytes = send_protocol_message(t,pending_params->buf,size*pending_params->count,pending_params->tag);                  
                if ( nbytes < 0)   // send a string to direct to socket
                {
                    // trouble sending -- close connection 
                     debug_r(DEBUG_VERBOSE,"(%s:%d)%s  MPI-SEND-->CLIENT WARNING, trouble sending %d bytes to client %d [SOURCE:%d,seqnum:%ld,size=%d,msg=%s]\n",
                                    __FILE__,__LINE__,dheader,nbytes,t->id,pending_params->client,pending_params->seqnum,size*pending_params->count,pending_params->buf); 
                    remove_connection(&c,t->id);   // what to do to notify the child????             
                    close(t->fd);   // what to do to notify the child????
                 }  
                debug_r(DEBUG_VERBOSE,"(%s:%d)%s B. MPI-SEND-->CLIENT sent %d bytes to client %d [SOURCE:%d,seqnum:%ld,bytes=%d]\n",
                        __FILE__,__LINE__,dheader,nbytes,t->id,pending_params->client,pending_params->seqnum,size*pending_params->count);  
            }  
            // MPI sending to clients but all there are no clients..... drop message
            if (  numconnection(&c) == 0 ) {
                debug_r(DEBUG_WARNING,"(%s:%d)%s (SEND)-->CLIENT WARNING no OUT client, dropping msg [SOURCE:%d,seqnum:%ld,count=%d,msgsize=%d]\n",
                    __FILE__,__LINE__,dheader,pending_params->client,pending_params->seqnum,pending_params->count,size*pending_params->count); 
            }
            MPI_Grequest_complete(*(pending_params->request));
            recv_pending_params = NULL;   
        }
        
        // MPI request -- RECV
        // PENDING MPI receive, wait to receive data from one of the clients to finish the handling of the receive.  In order for data to flow
        // there has to be a corresponding MPI_RECV command 
        // Doing it way allows for a recv to happen and then for a client to connect and send data, MPI MUST not issue another command when there
        // is already a pending receive on the request.  Cannot have two outstanding requests on the same MPIX-recv command (investigate separate at some point)
        if ( recv_pending_params && (recv_pending_params->ctype == RECV) ) {
            debug_r(DEBUG_VERBOSE,"(%s:%d)%s A. CLIENT---> checking to see whether any of the clients have data \n",__FILE__, __LINE__,dheader);
            connection_t *citem = NULL;
            int i=0;
            while ( next_connection(c,&citem) ) {  
                i++;         
                if ( FD_ISSET(getfd_connection(citem),&read_fd_set) ) {
                    debug_r(DEBUG_VERBOSE,"(%s:%d)%s B. CLIENT--->  %d there is a matching msg for MPI Recv()\n",__FILE__, __LINE__,dheader,citem->id);   
                    if ( !doclientmsg(args,citem,&recv_pending_params) )    
                    {              
                        citem->state = CLOSED;  // may need to clean up connections that are closed but not deleted
                        close(citem->fd);
                        //safe_fd_clr(citem->fd,&active_fd_set,&maxfd); // closed connection, clear the read bit -- from the permanent set?
                    }
                }// end-if
            } // end-while                 
        }  // end-if  


        if ( recv_pending_params && (recv_pending_params->ctype == DISPOSE) ) {
            debug_r(DEBUG_START,"%s DISPOSE MPI signaled exit .. terminating child process\n",dheader);
            free(recv_pending_params);   // careful here, in this case the child is freeing it, allocated in parent by MPIX_chan_close()
            recv_pending_params = NULL; 
            break;        
        }
        
        debug_r(DEBUG_VERBOSE,"(%s:%d)%s END-WHILE end of loop checking for messages\n",__FILE__, __LINE__,lheader);    
    } // end-while         
    deleteclose_allconnections(&c);
    close(servSock);
    // end of select 
    debug_r(DEBUG_START,"%s EXITING CHILD\n",dheader);    
    pthread_exit(NULL);
}
