#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <new>
#include <stack>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

struct InfoType{
    string str;

    InfoType(string str): str(str){};
};

template<typename T>
struct Node{
    T key;
    Node<T> *left, *right, *parent;
    InfoType* info;
    int release;
    Node(T key): key(key), left(nullptr), right(nullptr), parent(nullptr), info(nullptr), release(1) {};
    Node(T key, InfoType* info): key(key), left(nullptr), right(nullptr), parent(nullptr), info(info), release(1) {};
};

template<typename T>
class BinaryTree{
private:
    Node<T>* root;
public:
    BinaryTree(): root(nullptr){};
    BinaryTree(Node<T>* root): root(root){};
    BinaryTree(const BinaryTree& other);
    ~BinaryTree();

    void insert(T key, string str);
    void print(); 
    void print(T key);
    void draw();
    Node<T>* max();
    Node<T>* min();
    int del(T key);
    Node<T>* find_node(T key);
    vector<Node<T>*> find_close_elem(T key);
    vector<Node<T>*> find_elems(T key);
    vector<Node<T>*> symmetric_traversal();
    void clear(Node<T>* node);
    vector<T> LNR_traversal();
    vector<T> NLR_traversal();
    bool is_empty();
};

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree &other)
{
    BinaryTree<T> out;
    Node<T>* ptr = other.root;
    stack<Node<T>*> stack;
    stack.push(ptr);
    while (!stack.empty()){
        ptr = stack.top();
        stack.pop();
        InfoType* info = new InfoType(ptr->info->str);
        out.insert(ptr->key, info);
        if (ptr->right != nullptr) {
            stack.push(ptr->right);
        }
        if (ptr->left != nullptr) {
            stack.push(ptr->left);
        }
    }
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
    clear(root);
}

template <typename T>
void BinaryTree<T>::insert(T key, string str)
{
    InfoType* info = new InfoType(str);
    Node<T>* node = new Node<T>(key, info);

    if (root == nullptr){
        root = node;
        return;
    }

    Node<T>* ptr = root;
    Node<T>* par = nullptr;

    while (ptr != nullptr){
        par = ptr;
        if (key < ptr->key) ptr = ptr->left;
        else if (key > ptr->key) ptr = ptr->right;
        else if (key == ptr->key){
            ptr = ptr->right;
            node->release = par->release + 1;
        }
    }

    if (key < par->key){
        par->left = node;
        node->parent = par;
        return;
    }
    if (key >= par->key){
        par->right = node;
        node->parent = par;
        return;
    }
}


template <typename T>
inline void BinaryTree<T>::print()
{
    if (root == nullptr) return;

    vector<Node<T>*> vec = symmetric_traversal();
    cout.width(50);
    cout << right;
    cout << "-----------------------------------------";
    cout << endl;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << "key" ;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << "info";
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(50);
    cout << right;
    cout << "-----------------------------------------";
    cout << endl;
    for (int i = 0; i < vec.size(); ++i){
        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << right << vec[i]->key;
        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << right << vec[i]->info->str;
        cout.width(10);
        cout << right << "|" << endl; ;
        cout.width(50);
        cout << right;
        cout << "-----------------------------------------";
        cout << endl;
    }
}

template <typename T>
inline void BinaryTree<T>::print(T key)
{
    if (root == nullptr) return;

    if (find_node(key) == nullptr){
        cout << "no such key = "<< key << endl;
        return;
    }

    if (max()->key == key) return;

    vector<Node<T>*> vec = symmetric_traversal();
    cout.width(50);
    cout << right;
    cout << "-----------------------------------------";
    cout << endl;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << "key" ;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << "info";
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(50);
    cout << right;
    cout << "-----------------------------------------";
    cout << endl;
    for (int i = 0; i < vec.size(); ++i){
        if (vec[i]->key > key){
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << vec[i]->key;
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << vec[i]->info->str;
            cout.width(10);
            cout << right << "|" << endl; ;
            cout.width(50);
            cout << right;
            cout << "-----------------------------------------";
            cout << endl;
        }
    }
}

template <typename T>
inline void BinaryTree<T>::draw()
{
    Node<T>* ptr = root;
    queue<Node<T>*> queue;
    queue.push(ptr);

    ofstream file;
    file.open("tree.dot");
    file << "digraph tree{" << endl;

    while (!queue.empty()){
        ptr = queue.front();
        queue.pop();
        file << ptr->key << ptr->release << " " << "[label=" << ptr->key << "] ;" << endl;
        if (ptr->left != nullptr) {
            queue.push(ptr->left);
            file << ptr->left->key << ptr->left->release << " " << "[label=" << ptr->left->key << "] ;" << endl;
            file << ptr->key << ptr->release << "->" << ptr->left->key << ptr->left->release << ";" << endl;
        }

        if (ptr->right != nullptr) {
            queue.push(ptr->right);
            file << ptr->right->key << ptr->right->release << " " << "[label=" << ptr->right->key << "] ;" << endl;
            file << ptr->key << ptr->release << "->" << ptr->right->key << ptr->right->release << ";" << endl;
        }

    }
    file << "}";
    file.close();

}

template <typename T>
inline Node<T>* BinaryTree<T>::max()
{
    if (root == nullptr) return nullptr;

    Node<T>* ptr = root;
    while (ptr->right != nullptr){
        ptr = ptr->right;
    }

    return ptr;
}

template <typename T>
inline Node<T> *BinaryTree<T>::min()
{
    if (root == nullptr) return nullptr;

    Node<T>* ptr = root;
    while (ptr->left != nullptr){
        ptr = ptr->left;
    }

    return ptr;
}

template <typename T>
inline int BinaryTree<T>::del(T key)
{
    if (root == nullptr) return -1;

    vector<Node<T>*> elems = find_elems(key);
    if (elems.empty()) return -2;

    Node<T>* del_node = elems[0];
    for (int i = 1; i < elems.size(); ++i){
        if ( elems[i]->release > del_node->release) del_node = elems[i];
    }

    Node<T>* par = del_node->parent;
    Node<T>* p;
    InfoType* info = del_node->info;
    del_node->info = nullptr;
    delete info;

    if (del_node->left == nullptr && del_node ->right == nullptr){
        if (par == nullptr) root = nullptr;
        else if (par->left == del_node) par->left = nullptr;
        else par->right = nullptr;
        del_node->parent = nullptr;
        delete del_node;
        return 0;
    }


    if (del_node->right && del_node->left){
        p = del_node->right;
        while (p->left != nullptr)
        {
            p = p->left;
        }

        Node<T>* p_par = p->parent;

        if(p->left || p->right){
            Node<T>* y = p->right;
            y->parent = p_par;
            if (p_par->left == p) p_par->left = y;
            else p_par->right = y;
            p->right = nullptr;
        }
        else {
            if (p_par->left == p) p_par->left = nullptr;
            else p_par->right = nullptr;
        }

        p->parent = par;
        if (par == nullptr) root = p;
        else if (par->left == del_node) par->left = p;
        else par->right = p;
        p->left = del_node->left;
        p->right = del_node->right;
        if (del_node->left) del_node->left->parent = p;
        if (del_node->right) del_node->right->parent = p;
        del_node->left = nullptr;
        del_node->right = nullptr;
        delete del_node;
        return 0;
    }

    if (del_node->right || del_node->left){
        if (del_node->right) p = del_node->right;
        else p = del_node->left;
        p->parent = par;
        if (par == nullptr) root = p;
        else if (par->left == del_node) par->left = p;
        else par->right = p;
        delete del_node;
        return 0;
    }
    return 0;
}

template <typename T>
inline Node<T> *BinaryTree<T>::find_node(T key)
{
    Node<T>* ptr = root;
    
    while (ptr != nullptr){
        if (key == ptr->key) return ptr;
        if (key < ptr->key) ptr = ptr->left;
        else if (key > ptr->key) ptr = ptr->right;
    }

    return nullptr;
}

template <typename T>
inline vector<Node<T>*> BinaryTree<T>::find_close_elem(T key)
{
    Node<T>* target = find_node(key);
    vector<Node<T>*> close_elems;
    if (target == nullptr) return close_elems;

    Node<T> *next, *previous;
    Node<T>* target2 = target;

    T min_elem = min()->key;
    T max_elem = max()->key;

    if (max_elem != key){
        if (target->right){
            next = target->right;
            while (next->left != nullptr) next = next->left;
        }
        else{
            next = target->parent;
            while (next != nullptr && next->right == target){
                target = next;
                next = target->parent;
            }
        }
    }

    if (min_elem != key){
        if (target2->left){
            previous = target2->left;
            while (previous->right != nullptr) previous = previous->right;
        }
        else{
            previous = target2->parent;
            while (previous != nullptr && previous->left == target2){
                target2 = previous;
                previous = target2->parent;
            }
        }
    }

    if (min_elem == key) close_elems = find_elems(next->key);
    if (max_elem == key) close_elems = find_elems(previous->key);
    if (min_elem != key && max_elem != key){
        if (key - previous->key <= next->key - key){
            close_elems = find_elems(previous->key);
        }
        else close_elems = find_elems(next->key);
    }

    return close_elems;
}

template <typename T>
inline vector<Node<T> *> BinaryTree<T>::find_elems(T key)
{
    vector<Node<T>*> elems;
    Node<T>* ptr = root;
    
    while (ptr != nullptr){
        if (key < ptr->key) ptr = ptr->left;
        else if (key > ptr->key) ptr = ptr->right;
        else if (key == ptr->key){
            elems.push_back(ptr);
            ptr = ptr->right;
        }
    }

    return elems;
}

template <typename T>
inline vector<Node<T> *> BinaryTree<T>::symmetric_traversal()
{

    stack<Node<T>*> stack;
    vector<Node<T>*> vec;

    if (root != nullptr){
        Node<T>* ptr = root;
        stack.push(ptr);
        while (!stack.empty()){
            while (ptr->right != nullptr) {
                stack.push(ptr->right);
                ptr = ptr->right;
            }
            ptr = stack.top();
            stack.pop();
            vec.push_back(ptr);
            while (ptr->left == nullptr && !stack.empty()){
                ptr = stack.top();
                stack.pop();
                vec.push_back(ptr);
            }
            if (ptr->left != nullptr) {
                stack.push(ptr->left);
                ptr = ptr->left;
            }
        }
    }
    return vec;
}

template <typename T>
inline void BinaryTree<T>::clear(Node<T> *node)
{
    if (node){
        clear(node->left);
        clear(node->right);
        InfoType* info = node->info;
        node->info = nullptr;
        delete info;
        delete node;
    }
}

template <typename T>
inline vector<T> BinaryTree<T>::LNR_traversal()
{
    vector<T> keys;
    stack<Node<T>*> stack;
    if (root != nullptr){
        Node<T>* ptr = root;
        stack.push(ptr);
        while (!stack.empty()){
            while (ptr->left != nullptr) {
                stack.push(ptr->left);
                ptr = ptr->left;
            }
            ptr = stack.top();
            stack.pop();
            keys.push_back(ptr->key);
            while (ptr->right == nullptr && !stack.empty()){
                ptr = stack.top();
                stack.pop();
                keys.push_back(ptr->key);
            }
            if (ptr->right != nullptr) {
                stack.push(ptr->right);
                ptr = ptr->right;
            }
        }
    }
    return keys;
}

template <typename T>
inline vector<T> BinaryTree<T>::NLR_traversal()
{
    vector<T> keys;
    stack<Node<T>*> stack;
    if (root != nullptr){
        Node<T>* ptr = root;
        stack.push(ptr);
        while (!stack.empty()){
            ptr = stack.top();
            stack.pop();
            keys.push_back(ptr->key);
            if (ptr->right != nullptr) {
                stack.push(ptr->right);
            }
            if (ptr->left != nullptr) {
                stack.push(ptr->left);
            }
        }
    }
    return keys;
}

template <typename T>
inline bool BinaryTree<T>::is_empty()
{
    if (root == nullptr) return true;
    else return false;
}
