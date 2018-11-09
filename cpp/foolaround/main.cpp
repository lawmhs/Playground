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
	
	srand(time(NULL));
	int high = 100;
	int low = 25;
	high = high - low + 1;
	
	for(int i = 0; i < 1000; ++i) {
		int num = rand() % high + low;
		cout << num << endl;
		}
	
	return 0;
	
	}

