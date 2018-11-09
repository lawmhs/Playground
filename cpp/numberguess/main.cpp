#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {
	
	int gamestate = 1;
	
	// random seed to randomize the answer depending on the time
	srand(time(NULL));
	
	int answer, guess;
	
	// loop to set up the game
	
	while(gamestate == 1) {
		
		cout << "Welcome to the number guessing game!" << endl;
		int lower = 0;
		cout << "Please enter the minimum of the range of numbers you'd like to guess in : ";
		cin >> lower;
		int higher = 0;
		cout << "Please enter the maximum of the range of numbers you'd like to guess in : ";
		cin >> higher;
		
		higher = higher - lower + 1;
		// the -lower component is to make sure that when we linearly shift our randomly generated answer
		// that we don't don't end up overshooting the max
		answer = rand()  % higher + lower;
		guess = answer + 1;
		// make sure guess != answer at start of game
		
		// actual game loop
		
		while(answer != guess) {
			cout << "Guess a number : ";
			cin >> guess;
			
			if(guess > answer) {
				cout << "too high!" << endl;
				}
			else if(guess < answer) {
				cout << "too low!" << endl;
				}
			else if(guess == answer) {
				cout << "that's correct! the answer was : " << answer << endl;
				cout << "Would you like to play again? 1 for yes, anything else for no : ";
				int resp;
				cin >> resp;
				if (resp != 1) {
					gamestate = 0;
					}
				}
			}
		
		}
		
	cout << "thanks for playing!" << endl;
	
	return 0;
	}
