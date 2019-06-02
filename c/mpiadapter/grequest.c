/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
#include "grequest.h"
void *child_thread(void *ptr);

static const char dheader[] = ".............................................................";
/* simple functions for initializing params stucture, this structure is saved between the
MPI main and the children, MPI fills out part of it and the rest is completed by the child
MPI creates it and it NOT NULL when there is data to receive.
*/
void complete_params_request(int fd, params_struct **params)     // need to set params to NULL
{
    params_struct *recv_pending_params = *params;
    if (recv_pending_params) {
        int size;
        MPI_Type_size(recv_pending_params->datatype,&size);  
        size = size*recv_pending_params->count;
        recv_pending_params->buf = NULL;  
        recv_pending_params->tag = FIRSTCONNMSG;
        recv_pending_params->count = 0;   // zero message 
        recv_pending_params->seqnum = 0;
        recv_pending_params->client = fd;  // clients are named by their fd 
        debug_r(DEBUG_SERVER,"%s 1 (ACCEPT) completing message [client:%d,type:%s,tag:%d] -- %p,request:%p\n",
            dheader,fd,client_protocol_string(DELIMITER),recv_pending_params->tag,recv_pending_params,recv_pending_params->request);
        MPI_Grequest_complete(*(recv_pending_params->request));  // completes the recv params request
        debug_r(DEBUG_SERVER,"%s 1 (ACCEPT) sent to MPI seqnum=???,maxsize=%d bytes [client:%d,type:%s]\n",dheader,size,fd,client_protocol_string(DELIMITER));                                    
        *params = NULL;  // have taken care of that message
    }
}

void stop_child_thread(childargs_t **cargptr)
{
    childargs_t *cargs = *cargptr;
    debug_r(DEBUG_SERVER,"================================ stop_child_thread() closing MPIX channels ====================\n");
    MPIX_chan_close(cargs);
	// terminating ......
	pthread_join(cargs->thread,NULL);    // no return value
    debug_r(DEBUG_SERVER,"================================ stop_child_thread() Child Joined ====================\n");	
	chan_dispose(cargs->c);  // Do NOT dispose of channel while child is still using it!!!! 
    free(cargs->myname);
	free(cargs);
	*cargptr = NULL;
	debug_r(DEBUG_SERVER,"================================ stop_child_thread() cargs freed ====================\n");	
}

childargs_t *start_child_thread(char *name, int myrank, int servPort, enum client_protocol type, int maxbufsize) 
{
    /* pipe()  creates a pipe (an object that allows unidirectional data flow) 
    and allocates a pair of file descriptors.  The first descriptor connects to the read end of the pipe; 
    the second connects to the write end. 
    */
    int ret;
    int fdpipe[2];   //File descriptors for creating a pipe
    ret = pipe(fdpipe);    // create pipe, check to see if successful
    if ( ret < 0 ) {
        debug_r(DEBUG_WARNING,"start_child_thread() -- Error creating pipe -- pipe()  ret=%d \n",ret); 
        return NULL;   
    }      
    debug_r(DEBUG_SERVER,"================================ start_child_thread() [rank:%d] ====================\n",myrank);	  
   
    // allocate cargs structure to for child , provide it with a port and channel -- released in stop_child_thread()
    childargs_t *cargs = malloc(sizeof(childargs_t));
    cargs->myname = strdup(name);   // not safe --- doesn't check return
    cargs->myrank = myrank;
    cargs->port = servPort;
    cargs->c = chan_init(1);  // channel can buffer one message
    cargs->fdpipe = fdpipe[0]; // pass read end of pipe to child
    cargs->fdsend = fdpipe[1];    // write end of the pipe
    cargs->receivebuffsize = maxbufsize;   // fix this can be set when the programs starts.
    cargs->protocol = type;
    pthread_t thread;
    // now spawn the child, the child will listen and accept connections 
    ret = pthread_create(&thread, NULL, child_thread, (void *) cargs);
    cargs->thread = thread;   // potential sharing problem -- but child NEVER needs to know its who thread ID.
    if (ret < 0 ) {
        debug_r(DEBUG_WARNING,"start_child_thread() -- Error starting child-- pthread_create()  ret=%d \n",ret);
        free(cargs->myname);
        free(cargs); 
        return NULL;   
    }    
    return cargs;
}


/************************************************************
**  Child process
*************************************************************/

/*
    Logic for processing messages too and from the MPI parent, need to modify 
    with from_msg, to_msg, ONLY called when select has indicated there is a message to from 
    the parent to child, or a message from the child, will need to change this one.
*/
params_struct *child_process_get_mpibuf(childargs_t *args)  { 
    int v;
    int r;
    params_struct *params = NULL;
    read(args->fdpipe, &v, 4);  // empty the pipe
    //printf("%s CHILD -- value in pipe: %s\n",dheader,chan_comm_type_strings[v]);
    r = chan_recv(args->c,(void **)&params);   // receiving the buffer address from the parent.
    //printf("%s CHILD -- chan_recv() return value:%d (0 is SUCCESS)\n",dheader,r);
    if ( r < 0) {
        printf("Error Error return on chan_recv \n");
        assert(FALSE);                
    } 
    return params;
}

/********************************************************************************************
* IMPORTANT NOTE NOTE May depend of VERSION of MPICH as whether these are PRE-DEFINED
**********************************************************************************************/
// computes the status that should be returned for generalized request, also includes information
// about successful/unsuccessful cancellation of the request.  query invoked by wait/test{any|some|all}. 
// also invoked by MPI_REQUEST_GET_STATUS
//typedef int MPI_Grequest_query_function(void *extra_state, MPI_Status *status);   // Are typedefs already defined (uncomment -- version of MPICH?)
// clean up any user defined state ... 
//typedef int MPI_Grequest_free_function(void *extra_state);  // Are typedefs already defined (uncomment -- version of MPICH?)
// attempts to cancel some request
//typedef int MPI_Grequest_cancel_function(void *extra_state, int complete);  // Are typedefs already defined (uncomment -- version of MPICH?)

static int query_function(void *extra_state, MPI_Status *status);
static int free_function(void *extra_state);
static int cancel_function(void *extra_state, int complete);

static int query_function(void *extra_state, MPI_Status *status)
{
    if (GREQDEBUG) printf("GREREQUEST query_function(%p):  Called MPI grequest query function()\n",extra_state);
    params_struct *params = (params_struct *) extra_state;  // params structure that is malloced by the child
    //MPI_Count count;   // was commented out in  MPIchild
    //MPI_Get_elements_x(params->status, (params->datatype), &count);  // may need to comment out in MPIchild
    //MPI_Status_set_elements_x(status, (params->datatype), count);    // in MPIchild this was just count not params->count
    MPI_Status_set_elements_x(status, (params->datatype), params->count);  //datatype:datatype associated with count (handle), count:number of elements to associate with status (integer)
    MPI_Status_set_cancelled(status,0);
    status->MPI_SOURCE = params->client;
    status->MPI_TAG = params->tag; 
    //status->MPI_TAG = MPI_UNDEFINED;    was set to be undefined in MPIchild
    if (GREQDEBUG) printf("GREREQUEST query_function(%p):  Returning from MPI grequest query [SRC:%d,TAG:%d] %d of count %d\n",
                                                                                    extra_state,params->client,params->tag,params->nbytes,params->count);

    return MPI_SUCCESS;
}

// clean up any user defined state ... 
static int free_function(void *extra_state)
{
    if (GREQDEBUG) printf("GREREQUEST free_function(%p,request:%p):  Called MPI grequest free function()\n",extra_state,((params_struct *) extra_state)->status);
    free(((params_struct *) extra_state)->status); 
    //MPI_Type_free(((params_struct *)extra_state)->datatype);
    free(extra_state);    // params structure -- what frees the buffer inside this structure?
    return MPI_SUCCESS;
}

static int cancel_function(void *extra_state, int complete)
{
    if (GREQDEBUG) printf("GREREQUEST cancel_function(%p):  Called MPI grequest cancel function()\n",extra_state);
    return MPI_SUCCESS;
}


/* MPI external server connection -- Mallocs the params structures and status structure*/
static params_struct  *chan_comm_init(const void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Request *request)
{
    params_struct *params;
    MPI_Status *status;
    status = (MPI_Status *) malloc(sizeof(MPI_Status));       // saved structure to copy out when complete -- is this needed? MPI frees this (?)    
    params = (params_struct *) malloc(sizeof(params_struct));  // params structure allocated by call to MPIX_s/r freed in free_function.
    // NOTE ANY_TAG is -1 and ANY_SOURCE is -2, need to decide what to do when it is some other client --- may not match up (only one params structure in child)
    // Should queue up params when there are multiple and match on client or first ANY to operate as MPI
    if (GREQDEBUG) printf("GREREQUEST chan_comm_init(%p):  Allocating GREQUEST data() and status (%p) [count=%d,src:%d,tag=%d]\n",params,status,count,source,tag);
    params->buf = buf;   // assigns buffer provided by the MPI send/receive call in the MPI program.
    params->count = count;
    params->nbytes = 0;
    params->seqnum = 0;   // who maintains sequence numbers? -- should come from the connection information in child process
    params->client = source;
    params->tag = tag;
    params->request = request;
    params->status = status;
    //printf("Just before type_dup %s\n",params->buf);
    //MPI_Type_dup(datatype, params->datatype);
    params->datatype = datatype;
    MPI_Grequest_start(query_function,free_function,cancel_function, (void *)params, request);
    return params;
}

/*
*  Initializes a generalized request, allocates allocates params and request structure, these 
*  structures are freed by the grequest--free routine when the request is completed. Writes to
*  the pipe to notify child of pending read/write, sends the buffer to the child.  Depending on
*  whether it is a read or write the child fills the buffer with the message calls complete to
*  trigger completion by the parent  -- TYPE is SEND, RECV, LOG, DISPOSE
*  source is only used for SEND to a client
*/
int MPIX_chan(enum chan_comm_type type, const void *buf, int count, MPI_Datatype datatype,  int source, int tag, childargs_t *args,MPI_Request *request)
{
    params_struct *params;
    params = chan_comm_init(buf,count,datatype,source,tag,request);  // allocates params and request structure, freed by grequest--free routine when completed
    //params->count = count; done in init
    //params->datatype = datatype;   // done in init
    params->ctype = type;     // either SEND or RECV, type of communication
    //params->client = source;  // done in init
    //params->tag = tag;  // in init
    params->c = args->c; // child arguments --- MPI side needs to know which end of the pchan pipe to write to (needs to keep the args around)
    write(args->fdsend, &(params->ctype),4);  // write to pipe to notify child there is a some type of communication event
    chan_send(args->c,params);   // send the pointer to params data by pchan 
    return MPI_SUCCESS;    
}

int MPIX_chan_close(childargs_t *args)   // includes sending a message
{
    params_struct *params = (params_struct *) malloc(sizeof(params_struct));  // DISPOSE needs to free params 
    params->ctype = DISPOSE;
    params->c = args->c;
    write(args->fdsend, &(params->ctype),4);
    chan_send(args->c,params);  
    // close the channels and free allocated data
    chan_close(args->c);
    return MPI_SUCCESS;
}
