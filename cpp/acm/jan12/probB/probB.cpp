#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;


int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n;
    int k;
    while(cin >> n) {

        cin >> k; // two elements on each line

        int arr [n]; // instantiate the array
        for(int i = 0; i < n; ++i) {
            cin >> arr[i]; // put everything from the second line into the array
        }

        // now find the correct element
        k = k-1; // adjust for zero based index
        while(arr[k] != 1) {
            k = (k + 1) % n; // wraps around
        }
        cout << k + 1 << endl; // need the + 1 because of the properties of mod

    }    

    return 0;

}

