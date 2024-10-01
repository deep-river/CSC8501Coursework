#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    int a = 1;                  // a is at location 28c
    int* p1 = new int(2);        // p1 points to new int(2) at location ec0
    int** p2;
    p2 = &p1;                    // p2 points to p1 at location 290
    int** p3 = new int*;         // p3 points to new int* at location ee0
    *p3 = &a;                    // *p3 points to a at location 28c

    // Output all the requested values
    cout << "&a: " << &a << endl;            // Address of a
    cout << "p1: " << p1 << endl;            // Address stored in p1 (address of new int(2))
    cout << "&p1: " << &p1 << endl;          // Address of p1
    cout << "*p1: " << *p1 << endl;          // Value stored at the address p1 points to
    cout << "p2: " << p2 << endl;            // Address stored in p2 (address of p1)
    cout << "&p2: " << &p2 << endl;          // Address of p2
    cout << "**p2: " << **p2 << endl;        // Value at the address **p2 (dereferencing p2 to get p1, then dereferencing p1)
    cout << "p3: " << p3 << endl;            // Address stored in p3 (address of new int*)
    cout << "&p3: " << &p3 << endl;          // Address of p3
    cout << "&**p3: " << &**p3 << endl;      // Address of **p3 (dereferencing p3 to get &a, and then getting the address of a)

    return 0;
}
