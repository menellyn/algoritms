#include "LinkedList.h"

class Stack{
    char *arr;
    int top;
    int capacity;
    LinkedList l_stack;

public:
    Stack();
    Stack(int size);
    ~Stack();
    void push(char elem);
    char pop();
    char back();
    void print();
};