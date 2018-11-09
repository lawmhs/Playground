#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {

	// initialize random seed
	srand(time(NULL));
	
	int steps = 0;
	cout << "how many steps should our random walk take?" << endl;
	cin >> steps;
	cout << "initializing random walk with " << steps << " steps" << endl;
	
	int positions[steps]; // keep track of positions as time elapses
	int pos = 0; // initialize the walk at 0
	
	for(int i = 0; i < steps; i++) {
		
		int x = rand();
		
		if((x % 2) == 0) {
			pos++;
			}
		else {
			pos--;
			}
		
		positions[i] = pos;
		
		}
	
	cout << "our random walk ended at " << pos << endl;
	
	return 0;
}
