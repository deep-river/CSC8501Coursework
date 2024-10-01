//
// Created by 李邦宇 on 24-10-1.
//
#include <iostream>
using namespace std;

/* Create an array of characters on the stack, which contains your full name (including room for
 * a space between your first and last names). Now create a function which prints to screen your
 * name using the array you just created. */

void print_name_array(char arr[]) {
    cout << arr << endl;
}

void print_name_array_with_pointer(char* arr) {
    cout << arr << endl;
}

/* Create two arrays on the heap called “first” and “last”. Copy your first name into the “first”
 * array and your last name into the “last” array. */

/*
int main() {
    char my_name[] = "Bangyu Li";
    print_name_array_with_pointer(my_name);
}
*/