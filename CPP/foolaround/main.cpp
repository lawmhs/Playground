// this file is to be used to test silly things and whatnot

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

using namespace std;

int main() {
	
	int start = clock();
	
	printf("hello world!\n");
	
	int stop = clock();
	cout << "time: " << (stop - start)/double(CLOCKS_PER_SEC)*1000 << endl;
	// the above line should measure the miliseconds of execution time
	
	
	return 0;
	
	}

