#include <stdio.h>
#include <mpi.h>
#include <Python.h>
#include <string.h>

#include "caller.h"

#define FILE_NAME "shakespeare"
#define DESTINATION_FILE "sink"

// This program only works with text that has utf-8 characters

void printCorpus(struct Corpus * c);

int main(int arc, char** argv) {

    //initialize the MPI environment
    MPI_Init(NULL, NULL);

    // get number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // get rank of processes
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // meat of the MPI program here

    // some business starting up the python environment
    int err_caller = PyImport_AppendInittab("caller", PyInit_caller);
    Py_Initialize();
    PyRun_SimpleString("import sys\nsys.path.insert(0,'/usr/local/lib/python3.5/dist-packages/numpy/core/include/')\nsys.path.insert(1,'')");
    PyObject* caller_module = PyImport_ImportModule("caller");
    
    // MPI logic below
    int number;
    if (world_rank == 0) {

        // process 0 is supposed to read text off the internet
        // and then send it to process one
        // I'll leave out the grabbing the data from the web
        // and just send the file name off to process 1 instead
        char * file_name = FILE_NAME; // need one more for the null terminated character
        int len = strlen(file_name);
        printf("The length of the file name is %d\n", len);

        //number = greeting(number);

        MPI_Send(&len, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(file_name, len + 1, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD);

    } else if (world_rank == 1) {

        // this node receives the text file name from node 0, and then tokenizes it
        int len;
        MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("The received number is %d\n", len);

        // allocate buffer to receive, +1 to keep the null termination
        char * file_name = (char*) malloc(len + 1);

        // + 1 for null termination
        MPI_Recv(file_name, len + 1, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // send it up to python to be tokenized
        struct Corpus * c = malloc(sizeof (struct Corpus));
        // corpus object is 8 bytes, 4 for integer, 4 for the pointer of the char**

        // create a corpus and tokenize it
        fillCorpus(file_name, c);

        // send the struct in pieces, remember this only works if all the processes
        // are running on the same computer, otherwise another computer won't have any idea
        // what the addresses you're sending will mean
        //@@@
        MPI_Send(&c->size, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
        for(int i = 0; i < c->size; i++) {
            len = strlen(c->strs[i]);
            MPI_Send(&len, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(c->strs[i], len + 1, MPI_UNSIGNED_CHAR, world_rank + 1, 0, MPI_COMM_WORLD);
        }

    } else if (world_rank == 2) {

        int size;
        int word_len;
        char * word;

        struct Corpus * c = malloc(sizeof (struct Corpus));

        //receive from the previous process and fill in our corpus structure
        MPI_Recv(&size, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d as the length of the array of strings from process %d\n", world_rank, size, world_rank - 1);
        c->size = size;
        c->strs = malloc(c->size * (sizeof (char*))); // allocate the string array for the struct
        printf("Received size is %d\n", c->size);
        // start receiving the thing
        for(int i = 0; i < size; i++) {
            MPI_Recv(&word_len, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            word = (char * )malloc(word_len + 1);
            MPI_Recv(word, word_len + 1, MPI_UNSIGNED_CHAR, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            c->strs[i] = word;
        }
        // end receive of the words

        // filter out the words in the corpus, of stopwords and punctuation
        filter(c);
        printf("After filtering, the size is %d\n", c->size);

        int len;

        MPI_Send(&c->size, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
        for(int i = 0; i < c->size; i++) {
            len = strlen(c->strs[i]);
            MPI_Send(&len, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(c->strs[i], len + 1, MPI_UNSIGNED_CHAR, world_rank + 1, 0, MPI_COMM_WORLD);
        }

    } else if (world_rank == 3) {

        int size;
        int word_len;
        char * word;

        struct Corpus * c = malloc(sizeof (struct Corpus));

        //receive from the previous process and fill in our corpus structure
        MPI_Recv(&size, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d as the length of the array of strings from process %d\n", world_rank, size, world_rank - 1);
        c->size = size;
        c->strs = malloc(c->size * (sizeof (char*))); // allocate the string array for the struct
        printf("Received size is %d\n", c->size);
        // start receiving the thing
        for(int i = 0; i < size; i++) {
            MPI_Recv(&word_len, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            word = (char * )malloc(word_len + 1);
            MPI_Recv(word, word_len + 1, MPI_UNSIGNED_CHAR, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            c->strs[i] = word;
        }
        // end receive of the words
        
        FILE *fptr;
        fptr = fopen(DESTINATION_FILE, "w");

        if(fptr == NULL) {
            printf("could not open the sink\n");
            exit(1);
        }

        for(int i = 0; i < size; i++) {
            fprintf(fptr, "%s ", c->strs[i]);
        }

        fclose(fptr);

        //printCorpus(c);

        printf("tokenized text saved at %s\n", DESTINATION_FILE);
        printf("Goodbye!\n");

    }

    Py_Finalize();

    // // end MPI program

    // finalize the MPI environment.
    MPI_Finalize();

    return 0;

}

void printCorpus(struct Corpus * c) {
    printf("Begin printing the corpus of words, the size of the corpus is : %d\n", c->size);
    for(int i = 0 ; i < c->size; i++) {
        printf("%s ", c->strs[i]);
    }
    printf("\nEnd printing\n");
}

