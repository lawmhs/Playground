#include <stdio.h>
#include <mpi.h>

#define DEFAULT_LIMIT 10000
#define NUM_OF_PRIMES_TO_GENERATE 100

int main(int arc, char** argv) {

    // initialize the MPI environment
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

    // int token = 1;
    // int no_div = -1;
    // int recv = -1;
    // int p = -1; // p will turn into a prime when it can

    // int num_limit = 20;

    // while(num_limit > 0) {
    //     // run 100 times

    //     // receive from previous element

    //     // different cases for different processes:
    //     if(world_rank == 0) {

    //         token++;
    //         num_limit--;

    //         if(recv == -2) {
    //             token = -2;
    //         }

    //         printf("Process %d is now sending off value %d to process %d\n", world_rank, token, (world_rank + 1) % world_size);
    //         MPI_Send(&token, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);

    //         // the generator needs to receive after

    //     } else {

    //         if(world_rank == world_size - 1) {
    //             MPI_Recv(&recv, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //             num_limit--; // drainage
    //             if(num_limit == 0) {
    //                 token = -2;
    //                 MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
    //                 // begin drainage
    //             } 
    //             MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);

    //         } else {
    //             // the cases in between
    //             MPI_Recv(&recv, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //             printf("Process %d has just received value %d from process %d\n", world_rank, recv, world_rank - 1);
    //             if(p == -1) {
    //                 // if you haven't been assigned a value yet
    //                 // the first value that passes through will become the value
    //                 p = recv;
    //                 printf("%d\n", p);
    //             } else {
    //                 // check if divisible, if it isn't, pass it on
    //                 // otherwise, pass a -1 forward
    //                 if(recv == -2) {
    //                     // this is the end case
    //                     token = -2;
    //                     printf("Process %d is now communicating a close to process %d\n", world_rank, (world_rank + 1) % world_size);
    //                     MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
    //                     printf("Closing the filter for %d\n", p);
    //                     break;
    //                 } else if(recv % p != 0) {
    //                     token = recv;
    //                 } else {
    //                     token = -1; // this is probably redundant as we can just send the recv value off
    //                 }
    //                 printf("Process %d is now sending off value %d to process %d", world_rank, token, (world_rank + 1) % world_size);
    //                 MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
    //             }


    //         }

    //         if(world_rank == 0) {
    //             MPI_Recv(&no_div, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //         }

    //     }

    // }

    int p = -1;
    int token = 1; // send variable
    int recv = -1; // receiver variable

    int count = 100;

    while(count > 0) {
        if (world_rank != 0) {

            MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("Process %d received token %d from process %d\n", world_rank, token, world_rank - 1);

            if(world_rank != world_size - 1) {

                if(p == -1) {
                    p = token;
                    if(token != -1) {
                       printf("%d\n", p); 
                    }
                    token = -1; // the number made it through to a -1, so we stop sending it forward
                } else {
                    if(token % p == 0) {
                        token = -1; // number gets culled
                    }
                }

            } else {

                if(recv == -2) {
                    break;
                }

                count--;
                if(count == 0) {
                    token = -2;
                }

            }
        } else {
            // Set the token's value if you are process 0
            token++;
        }
        MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size,
                0, MPI_COMM_WORLD);

        // Now process 0 can receive from the last process.
        if (world_rank == 0) {
            MPI_Recv(&recv, 1, MPI_INT, world_size - 1, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("Process %d received token %d from process %d\n", world_rank, token, world_size - 1);
            if(recv == -2) {
                // time to quit
                token = -2; // set token to be -2.
            }
        }

        count--;

    }
    

    // finalize the MPI environment.
    MPI_Finalize();

    return 0;

}