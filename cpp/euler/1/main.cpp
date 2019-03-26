#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {

	// this is the slow algorithm, runs at O(n)
	// one can use the inclusion-exclusion principle
	// in order to get a constant time solution

	// int sum = 0;
	// for(int i = 0 ; i < 1000; ++i) {
	
	// 		if(i % 3 == 0 || i % 5 == 0) {
	// 			sum += i;
	// 		}
	// }
	// cout << sum << endl;

	// the fast solution would be to:
	// sum = sum_divis3 + sum_divis5 - sum_divis15

	int sum_3 = 333 * (3 + 999) / 2;
	int sum_5 = 199 * (5 + 995) / 2;
	int sum_15 = 66 * (15 + 990) / 2;

	int res = sum_3 + sum_5 - sum_15;

	cout << res << endl;
	
	return 1; 

}

