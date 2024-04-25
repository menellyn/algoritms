#include <cstdio>
#include <iostream>
#include <new>
#include "stack.h"

using namespace std;

Stack::Stack(){
    top = -1;
}

Stack::Stack(int size){
    top = -1;
    capacity = size;
}

Stack::~Stack(){}

void Stack::push(char elem){
    l_stack.push_front(elem);
    ++top;
}

char Stack::pop(){
    if (top == -1) return '1';
    char elem = l_stack.head->data;
    l_stack.pop_front();
    top--;
    return elem;
}

char Stack::back(){
    if (top == -1) return '1';
    char elem = l_stack.head->data;
    return elem;    
}

void Stack::print(){
    if (top == -1) return;
    while (l_stack.head){
        cout << l_stack.head->data;
        l_stack.head = l_stack.head->next;
    }
    cout << endl;
}