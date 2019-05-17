#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <task.h>

#include "sieve.h"

#define DEFAULT_LIMIT 1000
#define STACK_SIZE 10000

int printToTarget; 
int buf;

void printTask(void *lastChan) {

    int num, i;

    Channel *chan, *newChan;
    chan = lastChan;
    num = chanrecvul(chan);
    if(num > printToTarget) {
        taskexitall(0); // exit once we have reached the target
    }
    printf("Hello!, I am task number : %d!\n", num);
    newChan = chancreate(sizeof(unsigned long), buf);
    taskcreate(printTask, newChan, STACK_SIZE);
    for(;;) {
        i = chanrecvul(chan);
        chansendul(newChan, i);
    }


}

void taskmain(int argc, char **argv) {

    int i;
    Channel *c;

    if(argc > 1) {
        printToTarget = atoi(argv[1]);
    } else {
        printToTarget = DEFAULT_LIMIT;
    }
    printf("let's print to %d\n", printToTarget);

    // make our initial channel!
    c = chancreate(sizeof(unsigned long), buf);
    taskcreate(printTask, c, STACK_SIZE);

    for(i = 0;;i++) {
        chansendul(c, i);
    }

    

}

// /*
// This is a non-concurrent way to run a prime number sieve
// will follow up with an implementation of a concurrent one
// using libtask
// */
// int main() {

//     int array_len = UPPER_LIMIT + 1;

//     int nums[array_len]; // UPPER_LIMIT + 1th element is UPPER_LIMIT

//     // fill the array
//     for(int i = 0; i < array_len; i++) {
//         nums[i] = i;
//     }

//     // sieve the array
// 	sieve(nums, array_len);
    
//     // print the results
//     for(int i = 0; i < UPPER_LIMIT + 1; i++) {
//         if(nums[i] != 0) {
//             printf("%d\n", nums[i]);
//         }
//     }
//     printf("done the sequential sieve.\n");

//     // then run the concurrent one with libtask
//     // each thread has a number
//     // and each thread is responsible for filtering multiples of
//     // its thread number

//     int nextThread = 2;


//     return 0;
// }
