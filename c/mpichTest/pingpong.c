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

    // ping pong example
    int PING_PONG_LIMIT = 100;
    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2;
    while(ping_pong_count < PING_PONG_LIMIT) {
        if(world_rank == ping_pong_count % 2) {
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("%d send and incremented ping_pong_count %d to %d\n", world_rank, ping_pong_count, partner_rank);
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d received ping_pong_count %d from %d\n", world_rank, ping_pong_count, partner_rank);
        }
    }

    // finalize the MPI environment.
    MPI_Finalize();

    return 0;

}