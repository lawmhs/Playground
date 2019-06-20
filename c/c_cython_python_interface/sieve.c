#include <stdio.h>

int sieve(int *array, int array_len) {

	for(int i = 2; i < array_len; i++) {
		// starting from p^2, count up in increments of p 
		
		for(int j = i*i; j < array_len; j += i) {
			if(j >= array_len) {
				break;
			}
			array[j] = 0; // cross off the numbers
		}
	}

	return array_len;

}
