#include<iostream>

using namespace std;

int main () {
    int i;
    double A[5];
    double S = 0;
    for (i = 0 ; i < 5 ; i ++) {
        cin >> A[i];
        S += A[i];
    }
    cout << (S/5) << endl;
    return 0;
}