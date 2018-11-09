#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main() {
	
	// Pg. 14 exercises @@@@@@@@@@@@@@@@@@@@@@@@
	// Exercise 1.12:
	
	//~ int sum = 0;
	//~ for (int i = -100; i <= 100; ++i)
		//~ sum += i;
		//~ // should sum all the numbers from -100 to 100, so we'll end up with 0
		
	//~ cout << sum << endl;
	
	//~ // Exercise 1.13:
	
	
	//~ // this loop will sum all the numbers from 50 to 100
	//~ int sum2 = 0;
	
	//~ for (int i = 50; i <= 100; ++i)
		//~ sum2 += i;
		
	//~ // cout << sum2 << endl;
	
	//~ // this loop will print all the numbers from 10 to 0
	//~ for (int i = 10; i >= 0; i--)
		//~ cout << i << endl;
		
	//~ // this loop will prompt for two integers by the user and will print each number
	//~ // in the range of the two
	
	//~ int x1, x2;
	
	//~ cin >> x1 >> x2;
	
	//~ if (x1 > x2) {
		//~ for(int i = x2; i <= x1; i++)
			//~ cout << i << endl;
		//~ }
	//~ else if (x1 <= x2) {
		//~ for(int i = x1; i <= x2; i++)
			//~ cout << i << endl;
		//~ }
	
	//~ int currVal = 0, val = 0;

	//~ // the loop below shows that if the input cin is multiple items
	//~ // val will take the first item from the string buffer
	//~ // then do stuff, and on the next iteration of the loop it'll take the next item
	//~ // in the string buffer
	
	//~ while(cin >> val) {
		//~ cout << val << endl;
		//~ cout << "hello there!" << endl;
		//~ }
		
	// EXERCISES 1.4.4 PG 18 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
		
	//~ int currVal = 0, val = 0;
	
	//~ if (cin >> currVal) {
		//~ int cnt = 1;
		//~ while(cin >> val) {
			//~ if (val == currVal) {
				//~ ++cnt;
				//~ }
			//~ else {
					//~ cout << currVal << " occurs " << cnt << " times" << endl;
					//~ currVal = val;
					//~ cnt = 1;
				//~ }
			//~ }
			
			//~ cout << "got here yet?" << endl;
			//~ cout << currVal  << " occurs " << cnt << " times" << endl;
			
		//~ }
		
		
	
	return 0;
	
	
	
	
	}
