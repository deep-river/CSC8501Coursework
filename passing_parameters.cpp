//
// Created by 李邦宇 on 24-10-1.
//
#include <iostream>
using namespace std;

void f1 (void* d, int s)
{
    if (s == sizeof(double))
    {
        double *p = static_cast<double *>(d);
        ++*p; //LINE 8
    }
    else if (s == sizeof(int))
    {
        int *p = static_cast<int *>(d);
        ++*p; //LINE 15
    }
}

/*
int main ()
{
    double v1 = {25, 75, 100};
    int v2 = {72, 76, 98};
    double* p1 = v1;
    int* p2 = v2;
    f1 (&v1, sizeof(p1));
    f1 (&v2, sizeof(p2));
    cout << v1 << "" << v2 << endl; //LINE 24
    return 0;
}
*/

/*
Assume v1 is in memory location 100 and v2 is in memory location 200 (decimal notation). What would the above program output if:

LINE 8 and LINE 15 remain the same
LINE 8 and LINE 15 are ++*p;
LINE 8 and LINE 15 are *p++;
LINE 8 and LINE 15 are *p++; and LINE 24 is
cout << &v1 << “ “ << &v2 << endl;
*/