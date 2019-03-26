#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main() {

    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);

    // essentially all the evens just move down one
    // because all the odds turn into evens
    // and then back into odds
    int n;
    int val;
    cin >> n;

    for(int i = 0; i < n; ++i) {

        if(i > 0) {
            cout << " ";
        }
            
        cin >> val;
        cout << (val - ((val + 1) % 2));  
    }

    cout << endl;

    return 0;

}




