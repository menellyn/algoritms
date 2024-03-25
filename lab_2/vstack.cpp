#include <cstdio>
#include <iostream>
#include <new>
#include "stack.h"

using namespace std;

Stack::Stack(){
    top = -1;
}

Stack::Stack(int size){
    try{
        arr = new char[size];
    }
    catch (std::bad_alloc& ba){
        cout << "Memory Allocation is failed:" << ba.what() << endl;
    }
    capacity = size;
    top = -1;
}

Stack::~Stack(){
    delete[] arr;
}

void Stack::push(char elem){
    if (top == (capacity - 1)){
        cout << "Stack Overflow" << endl;
        return;
    }
    else{
        top++;
        arr[top] = elem;
    }
}

char Stack::pop(){
    if (top == -1) { 
        cout << "Stack is empty"; 
        throw "stack is empty"; 
    } 
    else {  
        char elem = arr[top];
        top--;
        return elem; 
    } 
}

char Stack::back(){
    if (top == -1) { 
        cout << "Stack is empty"; 
        throw "stack is empty"; 
    } 
    else {  
        return arr[top]; 
    }     
}

void Stack::print(){
    if (top == -1) return;
    while (top != -1){
        cout << arr[top];
        top--;
    }
    cout << endl;
}
