
class Node{
public:
    char data;
    Node *next;

    Node(char data){
        this->data = data;
        this->next = nullptr;
    }
};


class LinkedList{
public:
    Node *head;

    LinkedList();
    //~LinkedList();

    void push_front(char data);
    void print();
    void clear();
    void pop_front();

};