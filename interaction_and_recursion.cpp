//
// Created by 李邦宇 on 24-10-1.
//
#include <iostream>
using namespace std;

/* 1. Write a function that accepts an integer argument and returns a boolean value. The function
 * should return true if the integer argument is greater than or equal to zero, and false otherwise.
 * Call this function with a positive and negative value to test that it works, using cout to display
 * the result. */

bool check_int_value(int value) {
    if (value >= 0) {
        return true;
    } else {
        return false;
    }
}

/* 2. Write another function that continuously asks for numbers (use cin), until the user provides
 * a negative number. */

void ask_until_negative_num_provided() {
    while (true) {
        int x;
        cout << "Please input a number: " << endl;
        cin >> x;
        if (x < 0) {
            cout << "You entered a negative number" << endl;
            return;
        }
    }
}

/* 3. Recursion is an alternative to writing loops where a function “calls itself”. Write another
 * version of the factorial function that uses Recursion instead of loops. What is one advantage and
 * one disadvantage of using Recursion as an alternative to loops? */

int recursion_factorial(int n) {
    if (n == 1) { return 1; }
    return n * recursion_factorial(n - 1);
}


/*
int main() {
    // Q1
    bool positive_int_result = check_int_value(10);
    cout << "The result of value 10 is " << positive_int_result << endl;
    bool negative_int_result = check_int_value(-10);
    cout << "The result of value -10 is " << negative_int_result << endl;

    // Q2
    ask_until_negative_num_provided();

    // Q3
    int x = 6;
    cout << recursion_factorial(x) << endl;
}
*/