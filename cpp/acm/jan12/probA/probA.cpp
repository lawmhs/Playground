#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    // use getline
    string vip;
    string hand;
    while(cin >> vip) {
        cin >> hand;
        if(vip == "front") {
            if(hand == "1") {
                cout << "L" << endl;
            }
            if(hand == "2") {
                cout << "R" << endl;
            }
        }
        if(vip == "back") {
            if(hand == "1") {
                cout << "R" << endl;
            }
            if(hand == "2") {
                cout << "L" << endl;
            }
        }
    }

    return 0;

}




