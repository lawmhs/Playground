#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;


int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n;
    int k;
    int ret = 0;

    while(cin >> n) {
        cin >> k;
        // gets the n and k

        for(int i = 0; i < n; ++i) {
            int val;
            cin >> val;
            for(int j = 0; j < 3; ++j) {
                if(val < k) {
                    break;
                }
                else {
                    val -= k;
                }

            }
            ret += val;
        }
    }

    cout << ret << endl;
    
    return 0;

}