#include "LinkedList.h"
#include <new>
#include <cstdio>
#include <iostream>
using namespace std;

LinkedList::LinkedList(){
    head = nullptr;
}

/*
LinkedList::~LinkedList(){
    while (head){
        Node *node = head;
        head = head->next;
        delete node;
    }
}
*/

void LinkedList::push_front(char data){
    Node* node = new Node(data);
    node->next = head;
    head = node;
}

void LinkedList::print(){
    if (head == nullptr) return;
    Node* node = head;
    while (node) {
        cout << node->data;
        node = node->next;
    }
    cout << endl;
}

void LinkedList::clear(){
    while (head != nullptr)
    {
        Node* node = head;
        head = head->next;
        delete node;
    }
}

void LinkedList::pop_front(){
    if (head == nullptr) return;
    Node* node = head;
    head = head->next;
    delete node;
}
