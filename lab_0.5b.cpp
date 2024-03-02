/*
Из входного потока вводится произвольное число строк, состоящих из последовательности 
десятичных восьмиразрядных чисел, которые могут быть разделены одним или несколькими пробелами 
и/или знаками табуляции. Длина каждой строки произвольна. Конец ввода определяется концом файла. 
В программе в виде констант задан ключ шифровки для букв, цифр, разделителей и знаков препинания.

При обработке каждой строки сформировать новую строку, поместив в неё расшифрованный текст. 
Полученную строку вывести на экран.
*/

#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

using namespace std;


template<typename T>
class Node{
public:
    T data;
    Node *next;

    Node(T data){
        this->data = data;
        this->next = nullptr;
    }
};


template<typename T>
class LinkedList{
public:
    Node<T> *head;

    LinkedList(){
        head = nullptr;
    }

    ~LinkedList(){
        while (head)
        {
            Node<T> *node = head;
            head = head->next;
            delete node;
        }
    }

    void push_front(T data){
        Node<T>* node = new Node<T>(data);
        node->next = head;
        head = node;
    }

    void print() {
        if (head == nullptr) return;
        Node<T>* node = head;
        while (node) {
            cout << node->data;
            node = node->next;
        }
        cout << endl;
    }

    void clear(){
        while (head != nullptr)
        {
            Node<T>* node = head;
            head = head->next;
            delete node;
        }
    }

};


int CountOfNumbers(int num);
void Decrypt(Node<int>* node);


const map<int, char> ENCRYPTION = {
    {64538264, 'a'},
    {85730137, 'c'},
    {10029473, 'k'},
    {96749856, 'g'},
    {99999999, '.'},
    {99999998, ','},
    {99999997, ';'},
    {88888888, ' '},
    {74302815, '7'},
    {37126106, '2'},          
};


int main(){
    string str;
    LinkedList<int> sequence;
    int count;
    int num;
    cout << "Enter a sequence of numbers, press ENTER between strings(Ctrl-D to stop): " << endl;
    while (getline(cin, str)) {
        stringstream ss(str);
        while(ss>>num){
            count = CountOfNumbers(num);
            if (count == 8 && num > 0){
                sequence.push_front(num);
            }
        }
        Decrypt(sequence.head);
        sequence.clear();
    }
}


void Decrypt(Node<int>* node){
    LinkedList<char> messege;
    for ( ; node != nullptr; node = node->next){
        if (ENCRYPTION.find(node->data) != ENCRYPTION.end()){
            messege.push_front(ENCRYPTION.at(node->data));
        }
    }
    messege.print();
}

int CountOfNumbers(int num){
    int count = 0;
    while (num != 0){
    ++count;
    num = num / 10;
    }
    return count;
}