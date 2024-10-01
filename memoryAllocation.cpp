#include <iostream>
using namespace std;

// binary search tree

void arrayOfTen() {
    int* arr = new int[10];
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << "";
    }
    cout << endl;

    delete[] arr;
    arr = nullptr;
}

struct node {
    int value;
    struct node* left;
    struct node* right;
};

struct node* root = nullptr;

void insert_integer(struct node** tree, int value) {
    node* newNode = new node();
    newNode->value = value;
    if (*tree == nullptr) {
        *tree = newNode;
        return;
    }
    if ((*tree)->value == value) {
        return;
    }
    if ((*tree)->value < value) {
        insert_integer(&((*tree)->right), value);
    }
    if ((*tree)->value > value) {
        insert_integer(&((*tree)->left), value);
    }
}

void print_tree(struct node* tree) {
    if (tree != nullptr) {
        cout << tree->value << " ";
        print_tree(tree->left);
        print_tree(tree->right);
    }
}

void free_tree(struct node* tree) {
    if (tree == nullptr) {
        free_tree(tree->left);
        free_tree(tree->right);
        delete tree;
    }
}