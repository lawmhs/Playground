#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

int main() {

    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);

    int n;
    int k;
    cin >> n;
    cin >> k;

    // procedure:
    // run through the array
    // keep a priority queue of pairs of ints
    // of the value and the index
    
    // use vector and then sort it instead
    vector<pair<int, int> > v;
    vector<int> indices;
    int profit = 0;

    // add elements to the array
    for(int i = 0; i < n; ++i) {
        int val;
        cin >> val;
        v.push_back(make_pair(val, i));
    }

    // then sort
    sort(v.begin(), v.end());

    // then take the last k elements
    // and put it in the back
    for(int i = 0; i < k; ++i) {
        
        pair<int, int> p = v.back();
        v.pop_back();
        profit += p.first;
        indices.push_back(p.second);

    }

    cout << profit << endl;
    // sort indices
    sort(indices.begin(), indices.end());
    
    // now we generate the intervals
    int q = -1;
    for(int i = 0; i < k; ++i) {
        if(i > 0) {
            cout << " ";
        }
        int ind = indices.front();
        indices.erase(indices.begin());

        if(i == k-1) {
            cout << n - q - 1;
            break;
        }

        cout << ind - q;
        q = ind;
    }

    cout << endl;

    return 0;
}