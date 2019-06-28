#include <stdio.h>
#include <mpi.h>
#include <Python.h>

#include "caller.h"

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

        number = -1;
        printf("Process 0 is sending off %d to process 1!\n", number);

        number = greeting(number);

        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    } else if (world_rank == 1) {

        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);

        number = greeting(number);

        MPI_Send(&number, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
        
    } else if (world_rank == 2) {

        MPI_Recv(&number, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);

        number = greeting(number);

        MPI_Send(&number, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);

    } else if (world_rank == 3) {

        MPI_Recv(&number, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);

        number = greeting(number);

        char * file_name = "shakespeare";

        struct Corpus * c = malloc(sizeof (struct Corpus));

        fillCorpus(file_name, c);
        printf("%d\n", c->size);
        for(int i = 0; i < c->size; i++) {
            printf("%s ", c->strs[i]);
        }
        free(c);

    }

    Py_Finalize();

    // // end MPI program

    // finalize the MPI environment.
    MPI_Finalize();

    return 0;

}

