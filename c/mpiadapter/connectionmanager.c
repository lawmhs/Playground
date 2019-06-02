/********
* Written by Alan Wagner, 2018-2019
* Do not redistribute any portion of this code without permission.
**********/
//#define MAINTEST   // define for testing purposes
#include "adapter.h"

 int getfd_connection(connection_t *s) { return (s != NULL ? s->fd : -1); }
 int getid_connection(connection_t *s) { return (s != NULL ? s->id : -1); }
 int incseq_connection(connection_t *s) { return (s != NULL ? (++(s->seqnum) > 0) : FALSE); }


int add_connection(connection_t **c, int id, int fd, enum client_protocol type, enum client_state state) { 
    connection_t *s=NULL;   
    // check to see whether it is there
    HASH_FIND_INT( *c, &id, s);
    if ( s==NULL) {
        // new connection 
        s = malloc(sizeof(connection_t));
        s->type = type;
        s->state = state;
        s->fd = fd;
        s->seqnum = 0;
        s->tag = 0;
        s->id = id;
        HASH_ADD_INT( *c, id, s );  /* id: name of key field */
        return TRUE;
    }
    return FALSE;
}

int numconnection(connection_t **c)
{
    return HASH_COUNT(*c);
}

int getconnection(connection_t **c,int id,connection_t *t)
 {
     connection_t *s; 
     HASH_FIND_INT( *c, &id, s);
     if (s == NULL) return FALSE;
     memcpy(t,s,sizeof(connection_t));  // copy out the result  
    return TRUE;
 }
 
int getopenlist(connection_t *c,int list[], int max)
 {
    int i=0;
    connection_t *s, *tmp;
    HASH_ITER(hh, c, s, tmp) {
        if (!(s->state == CLOSED)) {
            list[i] = s->id; 
            if (i++ >= max ) return i;         
    }}
    return TRUE;
 }
 
 int getstatelist(connection_t *c, enum client_protocol type, enum client_state state,  int list[], int max)
 {
    int i=0;
    connection_t *s, *tmp;
    HASH_ITER(hh, c, s, tmp) {
        if (!(s->state == CLOSED)) {
            list[i] = s->id; 
            if (i++ >= max ) return i;         
    }}
    return TRUE;
 }
 
int getfd(connection_t **c,int id)
 {
    connection_t *s; 
    HASH_FIND_INT( *c, &id, s); 
    assert(s!=NULL);  // force this happen 
    return s->fd;
 }

 
int setstate_connection(connection_t **c,int id,enum client_state state)
 {
    connection_t *s; 
    HASH_FIND_INT( *c, &id, s); 
    //if (s==NULL) return FALSE; 
    assert(s!=NULL);  // force this happen 
    s->state = state;
    return TRUE;
 }
 
int settypestate_connection(connection_t **c,int id,enum client_protocol type, enum client_state state)
 {
    connection_t *s; 
    HASH_FIND_INT( *c, &id, s); 
    //if (s==NULL) return FALSE; 
    assert(s!=NULL);  // force this happen 
    s->state = state;
    s->type = type;
    return TRUE;
 }
 

int remove_connection(connection_t **c, int id) {
    assert(*c!=NULL);
    connection_t *s;   
    // find the connection
    HASH_FIND_INT( *c, &id, s);  
    if (s==NULL) return FALSE; 
    HASH_DEL(*c, s);  /* pointer to connection */
    free(s);
    return TRUE;
}

int test_connections(connection_t *c,int *read_fd_set)
{
    //FD_ISSET(connection[i].fd,read_fd_set)) 
    return 0;
}

void print_connections(connection_t *c) {
    connection_t *s, *tmp;
    if ( c == NULL)
        printf("NO CONNECTIONS in connection table\n");
    HASH_ITER(hh, c, s, tmp) {
        printf("Connection: [client:%d,type:%s,state:%s,fd:%d,seqnum=%ld]\n",s->id,
            client_state_strings[s->state],
            client_protocol_string(s->type),
            s->fd,
            s->seqnum
        );
    }
}


connection_t *next_connection(connection_t *c, connection_t **cur) {
    if ( *cur == NULL)  *cur = c;
    else *cur = (*cur)->hh.next;
    return *cur;
}


void delete_allconnections(connection_t **c) {
  connection_t *s, *tmp;
  HASH_ITER(hh, *c, s, tmp) {
    HASH_DEL(*c,s);  /* delete; users advances to next */
    free(s);   /* free connection struct */
  }
}

void deleteclose_allconnections(connection_t **c) {
  connection_t *s, *tmp;
  HASH_ITER(hh, *c, s, tmp) {
    //if (c->state != CLOSED) close(c->fd)
    HASH_DEL(*c,s);  /* delete; users advances to next */
    free(s);   /* free connection struct */
  }
}


#ifdef MAINTEST 
int main(int argc, char *argv[])
{
    connection_t *c = NULL;
    // test adding connections 
    printf("result was: %d\n",add_connection(&c,99,1,LOGGER,CLOSED)); 
    printf("result was: %d\n",add_connection(&c,92,1,LOGGER,CLOSED));
    printf("result was: %d\n",add_connection(&c,92,1,LOGGER,CLOSED));
    printf("result was: %d\n",add_connection(&c,95,1,LOGGER,CLOSED));  
    print_connections(c);
    // test iterating 
    connection_t *iterator = NULL;
    while (next_connection(c,&iterator))
        printf("Iterator test --- [client:%d,type:%s,state:%s,fd:%d,seqnum=%ld]\n",iterator->id,
                client_state_strings[iterator->state],
                client_protocol_string(iterator->type),
                iterator->fd,
                iterator->seqnum);
    // test removing connections
    printf("result was: %d\n",remove_connection(&c,92));
    printf("result was: %d\n",remove_connection(&c,99));
    printf("result was: %d\n",remove_connection(&c,95));
    printf("result was: %d\n",add_connection(&c,95,1,LOGGER,CLOSED));  
    print_connections(c); 
    delete_allconnections(&c);     
    printf("c is %p\n",c);    
    return 0;
}
#endif
