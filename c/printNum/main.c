#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <task.h>
#include <Python.h>

#include "sieve.h"
#include "functions.h"
#include "caller.h"
#include "helloworld.h"

#define DEFAULT_LIMIT 1000
#define STACK_SIZE 10000

int printToTarget; 
int buf;

// void printTask(void *lastChan) {

//     int num, i;

//     Channel *chan, *newChan;
//     chan = lastChan;
//     num = chanrecvul(chan);
//     if(num > printToTarget) {
//         taskexitall(0); // exit once we have reached the target
//     }
//     printf("Hello!, I am task number : %d!\n", num);
//     newChan = chancreate(sizeof(unsigned long), buf);
//     taskcreate(printTask, newChan, STACK_SIZE);
//     for(;;) {
//         i = chanrecvul(chan);
//         chansendul(newChan, i);
//     }


// }

// void taskmain(int argc, char **argv) {

//     int i;
//     Channel *c;

//     if(argc > 1) {
//         printToTarget = atoi(argv[1]);
//     } else {
//         printToTarget = DEFAULT_LIMIT;
//     }
//     printf("let's print to %d\n", printToTarget);

//     // make our initial channel!
//     c = chancreate(sizeof(unsigned long), buf);
//     taskcreate(printTask, c, STACK_SIZE);

//     for(i = 0;;i++) {
//         chansendul(c, i);
//     }

    

// }

// @@@ uncomment the above code for a concurrent prim enumber sieve using libmill

// this is a test main function for something else

int main(int argc, char** argv) {

    // also require testing
    //write_code(100);

    int err_caller = PyImport_AppendInittab("caller", PyInit_caller);
    if(err_caller == -1) {
        printf("Failed to append caller module\n");
        return 0;
    }
    int err_helloworld = PyImport_AppendInittab("helloworld", PyInit_helloworld);
    if(err_helloworld == -1) {
        printf("Failed to append helloworld module\n");
        return 0;
    }
    // this runs through opening a new python interpreter and then calling the code inside PyRun_SimpleString
    Py_Initialize();
    PyObject* caller_module = PyImport_ImportModule("caller");
    PyObject* helloworld_module = PyImport_ImportModule("helloworld");
    // PyRun_SimpleString("print('hello world')");
    // Py_Finalize();
    PyInit_caller();
    greeting(100);

    Node * n = malloc(sizeof (Node));
    initNode(n, 0, 0, 0);
    printf("the position of node n is (%d, %d) and its value is %d\n", n->x, n->y, n->val);
    free(n);

    Py_Finalize();

    // then : Now we try making a copy of the environment
    // PyObject* main_module = PyImport_AddModule("__main__");
    // PyObject* main_dict = PyModule_GetDict(main_module);
    // PyObject* main_dict_copy = PyDict_Copy(main_dict);

    // FILE* file_1 = fopen("test_script.py", "r");
    // PyRun_File(file_1, "test_script.py", Py_file_input, main_dict, main_dict);

    // Py_Finalize();

    return 0;
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
