#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <new>
#include <fstream>
#include <vector>
#include <array>

const int t = 2;

using namespace std;

struct InfoType{
    float num1, num2;
    string str;

    InfoType(float num1, float num2, string str): num1(num1), num2(num2), str(str){};
};

template<typename T>
struct Node{
    T key[2*t - 1];
    Node<T>* ptr[2*t];
    InfoType* info[2*t - 1];
    Node<T>* parent;
    int key_counter;

    Node(): ptr{nullptr, nullptr, nullptr, nullptr}, info{nullptr, nullptr, nullptr}, parent(nullptr), key_counter(0) {};
    Node(T k, InfoType* info1): ptr{nullptr, nullptr, nullptr, nullptr}, info{info1, nullptr, nullptr}, parent(nullptr), key_counter(1) {
        key[0] = k;
    }
    Node(T k, Node<T>* first, Node<T>* second, InfoType* info1): ptr{first, second, nullptr, nullptr}, 
        info{info1, nullptr, nullptr}, parent(nullptr), key_counter(1){
            key[0] = k;
    }

    void swap(T &x, T &y, InfoType& x_info, InfoType& y_info){
        T tmp = x;
        InfoType tmp_info = x_info;
        x = y;
        x_info = y_info;
        y = tmp;
        y_info = tmp_info;
    }

    void sort2(T &x, T &y, InfoType& x_info, InfoType& y_info){
        if (x > y) swap(x, y, x_info, y_info);
    }

    void sort3(T &x, T &y, T &z, InfoType& x_info, InfoType& y_info, InfoType& z_info){
        if (x > y) swap (x, y, x_info, y_info);
        if (x > z) swap (x, z, x_info, z_info);
        if (y > z) swap (y, z, y_info, z_info);
    }

    void sort(){
        if (key_counter == 1) return;
        else if (key_counter == 2) sort2(key[0], key[1], *info[0], *info[1]);
        else if (key_counter == 3) sort3(key[0], key[1], key[2], *info[0], *info[1], *info[2]);
    }

    void insert(T k, InfoType* inf){
        key[key_counter] = k;
        info[key_counter] = inf;
        ++key_counter;
        sort();
    }

    bool is_leaf(){
        return (ptr[0] == nullptr) && (ptr[1] == nullptr) && (ptr[2] == nullptr);
    }

    int find(T k){
        for (int i = 0; i < key_counter; ++i){
            if (key[i] == k) return i;
        }
        return -1;
    }

    void become_node2(T k, Node<T>* first, Node<T>* second, InfoType* inf){
        key[0] = k;
        ptr[0] = first;
        ptr[1] = second;
        ptr[2] = nullptr;
        ptr[3] = nullptr;
        info[0] = inf;
        info[1] = nullptr;
        info[2] = nullptr;
        parent = nullptr;
        key_counter = 1;
    }

    void print(){
        for (int i = 0; i < key_counter; ++i){
            if (!is_leaf()) ptr[i]->print();
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << key[i];
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << info[i]->num1;
            cout.width(10);
            cout << right << "|" << endl;
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << " ";
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << info[i]->num2;
            cout.width(10);
            cout << right << "|" << endl;
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << " ";
            cout.width(10);
            cout << right << "|" ;
            cout.width(10);
            cout << right << info[i]->str;
            cout.width(10);
            cout << right << "|" << endl;
            cout.width(50);
            cout << right;
            cout << "-----------------------------------------";
            cout << endl;
        }
        if (!is_leaf()) ptr[key_counter]->print();
    }

    void traversal(vector<T>& vec){
        for (int i = 0; i < key_counter; ++i){
            if (!is_leaf()) ptr[i]->traversal(vec);
            vec.push_back(key[i]);
        }
        if (!is_leaf()) ptr[key_counter]->traversal(vec);

    }

    void draw(){
        ofstream file;
        file.open("tree.dot", ios_base::app);
        if (key_counter == 1) file << "\""<< key[0] << "\"" << " " << "[label=\"" << key[0] << "\"] ;" << endl;
        else{
            file <<  "\""<<  key[0] <<  "\""<< " " << "[label=\"" << key[0] << " , " << key[1] << " \"] ;" << endl;
        }
        file.close();
        for (int i = 0; i <= key_counter; ++i){
            if (!is_leaf()){
                file.open("tree.dot", ios_base::app);
                file <<  "\""<< key[0] << "\""<< "->" <<  "\""<< ptr[i]->key[0] <<  "\""<< ";"  << endl;
                file.close();
                ptr[i]->draw();
            }
        }
    }

    void del_key(T k){
        int i = find(k);
        info[i] = nullptr;

        if (key_counter == 2){
            if (info[0] == nullptr){ 
                info[0] = info[1];
                info[1] = nullptr;
                key[0] = key[1];
            }
        }

        key_counter--;
    }

    void del_key_and_info(T k){
        int i = find(k);
        InfoType* del_info = info[i];
        info[i] = nullptr;
        delete del_info;

        if (key_counter == 2){
            if (info[0] == nullptr){ 
                info[0] = info[1];
                info[1] = nullptr;
                key[0] = key[1];
            }
        }

        key_counter--;
    }

    
};

template<typename T>
class BTree{
private:
    Node<T>* root;
public:
    BTree(): root(nullptr){};
    BTree(const BTree& other);
    BTree(BTree&& other);
    ~BTree();

    void clear(Node<T>* node);
    Node<T>* copy_tree(Node<T>* this_node, Node<T>* other_node);
    InfoType* insert(T key, float num1, float num2, string str);
    Node<T>* split(Node<T>* node);
    InfoType* search(T key);
    Node<T>* find_key(T key);
    InfoType* find_next(T key);
    T find_next_key(T key);
    void print();
    vector<T> symmetric_traversal();
    void draw();
    int del(T key);
    void fix(Node<T>* leaf);
    Node<T>* redistribute(Node<T>* leaf);
    Node<T>* merge(Node<T>* leaf);
    bool empty();


};

template <typename T>
inline BTree<T>::BTree(const BTree &other)
{
    Node<T>* ptr = other.root;
    this->root = nullptr;

    this->root = copy_tree(this->root, ptr);

}

template <typename T>
inline BTree<T>::BTree(BTree &&other)
{
    this->root = other.root;
    other.root = nullptr;
}

template <typename T>
inline BTree<T>::~BTree()
{
    clear(root);
}

template <typename T>
inline void BTree<T>::clear(Node<T> *node)
{
    if (root == nullptr) return;

    for (int i = 0; i <= node->key_counter; ++i){
        if (!node->is_leaf()) clear(node->ptr[i]);
    }
    for (int i = 0; i < node->key_counter; ++i){
        delete node->info[i];
    }
    delete node;
}

template <typename T>
inline Node<T> *BTree<T>::copy_tree(Node<T>* this_node, Node<T>* other_node)
{
    if (other_node == nullptr) return nullptr;

    Node<T>* new_node = new Node<T>;

    for (int i = 0; i < other_node->key_counter; ++i){
        InfoType* info = new InfoType(other_node->info[i]->num1, other_node->info[i]->num2, other_node->info[i]->str);
        new_node->insert(other_node->key[i], info);
    }

    new_node->parent = this_node;

    for (int i = 0; i <= other_node->key_counter; ++i){
        if (!other_node->is_leaf()){
            new_node->ptr[i] = copy_tree(new_node, other_node->ptr[i]);
        }
    }

    return new_node;
}

template <typename T>
inline InfoType *BTree<T>::insert(T key, float num1, float num2, string str)
{
    if (root == nullptr){
        InfoType* info1 = new InfoType(num1, num2, str);
        Node<T>* node = new Node<T>(key, info1);
        root = node;
        return nullptr;
    }

    Node<T>* ptr = root;
    while (!ptr->is_leaf()){
        int i = ptr->find(key);
        if (i != -1){
            InfoType* old_info = ptr->info[i];
            ptr->info[i]->num1 = num1;
            ptr->info[i]->num2 = num2;
            ptr->info[i]->str = str;
            return old_info;
        }

        if (key < ptr->key[0]) ptr = ptr->ptr[0];
        else if ((ptr->key_counter == 1) || ((ptr->key_counter == 2) &&  (key < ptr->key[1]))) ptr = ptr->ptr[1];
        else ptr = ptr->ptr[2];
    }

    int i = ptr->find(key);
    if (i != -1){
        InfoType* old_info = ptr->info[i];
        ptr->info[i]->num1 = num1;
        ptr->info[i]->num2 = num2;
        ptr->info[i]->str = str;
        return old_info;
    }
    else{
        InfoType* info1 = new InfoType(num1, num2, str);
        ptr->insert(key, info1);
        while (ptr != nullptr){
            ptr = split(ptr);
        }
    }

    return nullptr;
}

template <typename T>
inline Node<T> *BTree<T>::split(Node<T> *node)
{
    if (node->key_counter < 3) return nullptr;

    Node<T>* left = new Node<T>(node->key[0], node->ptr[0], node->ptr[1], node->info[0]);
    Node<T>* right = new Node<T>(node->key[2], node->ptr[2], node->ptr[3], node->info[2]);
    if (left->ptr[0]) left->ptr[0]->parent = left;
    if (left->ptr[1]) left->ptr[1]->parent = left;
    if (right->ptr[0]) right->ptr[0]->parent = right;
    if (right->ptr[1]) right->ptr[1]->parent = right;

    if (node->parent != nullptr){
        node->parent->insert(node->key[1], node->info[1]);
        left->parent = node->parent;
        right->parent = node->parent;

        if (node->parent->ptr[0] == node){
            node->parent->ptr[3] = node->parent->ptr[2];
            node->parent->ptr[2] = node->parent->ptr[1];
            node->parent->ptr[1] = right;
            node->parent->ptr[0] = left;
        }
        else if (node->parent->ptr[1] == node){
            node->parent->ptr[3] = node->parent->ptr[2];
            node->parent->ptr[2] = right;
            node->parent->ptr[1] = left;
        }
        else if (node->parent->ptr[2] == node){
            node->parent->ptr[3] = right;
            node->parent->ptr[2] = left;
        }

        Node<T>* tmp = node->parent;
        node->parent = nullptr;
        node->ptr[3] = nullptr;
        for (int i = 0; i < node->key_counter; ++i){
            node->info[i] = nullptr;
            node->ptr[i] = nullptr;
        }
        delete node;
        return tmp;
    }
    else{
        left->parent = node;
        right->parent = node;
        node->become_node2(node->key[1], left, right, node->info[1]);
        root = node;
        return nullptr;
    }

    return nullptr;

}

template <typename T>
inline InfoType *BTree<T>::search(T key)
{
    if (root == nullptr) return nullptr;
    Node<T>* ptr = root;
    while (!ptr->is_leaf()){
        int i = ptr->find(key);
        if (i != -1) return ptr->info[i];

        if (key < ptr->key[0]) ptr = ptr->ptr[0];
        else if ((ptr->key_counter == 1) || ((ptr->key_counter == 2) &&  (key < ptr->key[1]))) ptr = ptr->ptr[1];
        else ptr = ptr->ptr[2];        
    }

    int i = ptr->find(key);
    if (i != -1) return ptr->info[i];
    return nullptr;
}

template <typename T>
inline Node<T> *BTree<T>::find_key(T key)
{
    if (root == nullptr) return nullptr;
    Node<T>* ptr = root;
    while (!ptr->is_leaf()){
        int i = ptr->find(key);
        if (i != -1) return ptr;

        if (key < ptr->key[0]) ptr = ptr->ptr[0];
        else if ((ptr->key_counter == 1) || ((ptr->key_counter == 2) &&  (key < ptr->key[1]))) ptr = ptr->ptr[1];
        else ptr = ptr->ptr[2];        
    }

    int i = ptr->find(key);
    if (i != -1) return ptr;
    return nullptr;
}

template <typename T>
inline InfoType *BTree<T>::find_next(T key)
{
    if (root == nullptr) return nullptr;

    Node<T>* ptr = find_key(key);

    if (ptr == nullptr) return nullptr;
    Node<T>* target = ptr;

    int i = ptr->find(key);
    if (target->ptr[i+1] != nullptr){
        target = target->ptr[i+1];
        while (target->ptr[0] != nullptr){
            target = target->ptr[0];
        }
        return target->info[0];
    }
    else if (i == 0 && target->key_counter == 2){
        return target->info[1];
    }
    else{
        target = ptr->parent;
        while (target != nullptr){
            if (target->ptr[0] == ptr) return target->info[0];
            if (target->ptr[1] == ptr && target->key_counter == 2) return target->info[1];
            ptr = target;
            target = ptr->parent;
        }
    }

    return nullptr;


}

template <typename T>
inline T BTree<T>::find_next_key(T key)
{
    if (root == nullptr) return key;

    Node<T>* ptr = find_key(key);

    if (ptr == nullptr) return key;
    Node<T>* target = ptr;

    int i = ptr->find(key);
    if (target->ptr[i+1] != nullptr){
        target = target->ptr[i+1];
        while (target->ptr[0] != nullptr){
            target = target->ptr[0];
        }
        return target->key[0];
    }
    else if (i == 0 && target->key_counter == 2){
        return target->key[1];
    }
    else{
        target = ptr->parent;
        while (target != nullptr){
            if (target->ptr[0] == ptr) return target->key[0];
            if (target->ptr[1] == ptr && target->key_counter == 2) return target->key[1];
            ptr = target;
            target = ptr->parent;
        }
    }

    return key;
}

template <typename T>
inline void BTree<T>::print()
{
    if (root == nullptr) return;

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

    root->print();
}

template <typename T>
inline vector<T> BTree<T>::symmetric_traversal()
{
    vector<T> vec;
    if (root == nullptr) return vec;

    root->traversal(vec);
    return vec;
}

template <typename T>
inline void BTree<T>::draw()
{
    if (root == nullptr) return;
    ofstream file;
    file.open("tree.dot");
    file << "digraph tree{" << endl;
    file.close();
    root->draw();
    file.open("tree.dot", ios_base::app);
    file << "}";
    file.close();
}

template <typename T>
inline int BTree<T>::del(T key)
{
    if (root == nullptr) return -1;
    Node<T>* del_node = find_key(key);
    if (del_node == nullptr) return -2;

    if (del_node == root && del_node->key_counter == 1 && del_node->is_leaf()){
        InfoType* del_info = del_node->info[0];
        del_node->info[0] = nullptr;
        root = nullptr;
        delete del_info;
        delete del_node;
        return 0;
    } 

    if (del_node->is_leaf() && del_node->key_counter == 2){
        del_node->del_key_and_info(key);
        return 0;
    }

    int i = del_node->find(key);
    Node<T>* target = del_node;
    if (target->ptr[i+1] != nullptr){
        target = target->ptr[i+1];
        while (target->ptr[0] != nullptr){
            target = target->ptr[0];
        }
        T swap_key = target->key[0];
        InfoType* swap_info = target->info[0];
        target->info[0] = del_node->info[i];
        target->key[0] = del_node->key[i];
        del_node->info[i] = swap_info;
        del_node->key[i] = swap_key;
        del_node = target;
    }

    del_node->del_key_and_info(key);
    fix(del_node);

    return 0;
}

template <typename T>
inline void BTree<T>::fix(Node<T> *leaf)
{
    if (leaf->key_counter != 0) return;

    Node<T>* par = leaf->parent;

    if (par->key_counter == 1 && (par->ptr[0]->key_counter == 2 || par->ptr[1]->key_counter == 2)) leaf = redistribute(leaf); 
    else if (par->key_counter == 2 && (par->ptr[0]->key_counter == 2 || par->ptr[1]->key_counter == 2 || par->ptr[2]->key_counter == 2)) leaf = redistribute(leaf); 
    else if (par->key_counter == 2) leaf = redistribute(leaf); 
    else leaf = merge(leaf);

    fix(leaf);
}

template <typename T>
inline Node<T> *BTree<T>::redistribute(Node<T> *leaf)
{
    Node<T>* par = leaf->parent;
    Node<T>* first = par->ptr[0];
    Node<T>* second = par->ptr[1];
    Node<T>* third = par->ptr[2];

    if (par->key_counter == 2 && first->key_counter < 2 && second->key_counter < 2 && third->key_counter < 2){
        if (first == leaf){
            par->ptr[0] = par->ptr[1];
            par->ptr[1] = par->ptr[2];
            par->ptr[2] = nullptr;

            par->ptr[0]->insert(par->key[0], par->info[0]);

            par->ptr[0]->ptr[2] = par->ptr[0]->ptr[1];
            par->ptr[0]->ptr[1] = par->ptr[0]->ptr[0];

            if (leaf->ptr[0] != nullptr) par->ptr[0]->ptr[0] = leaf->ptr[0];
            else if (leaf->ptr[1] != nullptr) par->ptr[0]->ptr[0] = leaf->ptr[1];

            if (par->ptr[0]->ptr[0] != nullptr) par->ptr[0]->ptr[0]->parent = par->ptr[0];


            par->del_key(par->key[0]);
            delete leaf;
        }
        else if (second == leaf){
            first->insert(par->key[0], par->info[0]);
            par->del_key(par->key[0]);

            if (leaf->ptr[0] != nullptr) first->ptr[2] = leaf->ptr[0];
            else if (leaf->ptr[1] != nullptr) first->ptr[2] = leaf->ptr[1];

            if (first->ptr[2] != nullptr) first->ptr[2]->parent = first;

            par->ptr[1] = par->ptr[2];
            par->ptr[2] = nullptr;

            delete leaf;
        }
        else if (third == leaf){
            par->ptr[2] = nullptr;
            second->insert(par->key[1], par->info[1]);
            par->del_key(par->key[1]);

            if (leaf->ptr[0] != nullptr) second->ptr[2] = leaf->ptr[0];
            else if (leaf->ptr[1] != nullptr) second->ptr[2] = leaf->ptr[1];

            if (second->ptr[2] != nullptr) second->ptr[2]->parent = second;

            delete leaf;
        }
    }
    else if (par->key_counter == 2 && (first->key_counter == 2 || second->key_counter == 2 || third->key_counter || 2)){
        if (first == leaf){

            if (leaf->ptr[0] == nullptr){
                leaf->ptr[0] = leaf->ptr[1];
                leaf->ptr[1] = nullptr;
            }

            first->insert(par->key[0], par->info[0]);

            if(second->key_counter == 2){
                par->key[0] = second->key[0];
                par->info[0] = second->info[0];

                first->ptr[1] = second->ptr[0];
                if (first->ptr[1] != nullptr) first->ptr[1]->parent = first;

                second->del_key(second->key[0]);
                second->ptr[0] = second->ptr[1];
                second->ptr[1] = second->ptr[2];
                second->ptr[2] = nullptr;
            }
            else if (third->key_counter == 2){
                par->key[0] = second->key[0];
                par->info[0] = second->info[0];
                second->key[0] = par->key[1];
                second->info[0] = par->info[1];
                par->key[1] = third->key[0];
                par->info[1] = third->info[0];

                first->ptr[1] = second->ptr[0];
                if (first->ptr[1] != nullptr) first->ptr[1]->parent = first;

                second->ptr[0] = second->ptr[1];
                second->ptr[1] = third->ptr[0];
                if (second->ptr[1] != nullptr) second->ptr[1]->parent = second;

                third->ptr[0] = third->ptr[1];
                third->ptr[1] = third->ptr[2];
                third->ptr[2] = nullptr;

                third->del_key(third->key[0]);
            }
        }
        else if (second == leaf){
            if (first->key_counter == 2){

                if (leaf->ptr[1] == nullptr){
                    leaf->ptr[1] = leaf->ptr[0];
                    leaf->ptr[0] = nullptr;
                }

                second->insert(par->key[0], par->info[0]);
                par->key[0] = first->key[1];
                par->info[0] = first->info[1];

                second->ptr[0] = first->ptr[2];
                if (second->ptr[0] != nullptr) second->ptr[0]->parent = second;


                first->del_key(first->key[1]);

                first->ptr[2] = nullptr;
            }
            else if (third->key_counter == 2){

                if (leaf->ptr[0] == nullptr){
                    leaf->ptr[0] = leaf->ptr[1];
                    leaf->ptr[1] = nullptr;
                }

                second->insert(par->key[1], par->info[1]);
                par->key[1] = third->key[0];
                par->info[1] = third->info[0];

                second->ptr[1] = third->ptr[0];
                if (second->ptr[1] != nullptr) second->ptr[1]->parent = second;

                third->del_key(third->key[0]);

                third->ptr[0] = third->ptr[1];
                third->ptr[1] = third->ptr[2];
                third->ptr[2] = nullptr;
            }
        }
        else if (third == leaf){

            if(leaf->ptr[0] != nullptr){
                leaf->ptr[1] = leaf->ptr[0];
                leaf->ptr[0] = nullptr;
            }

            third->insert(par->key[1], par->info[1]);

            if (second->key_counter == 2){
                par->key[1] = second->key[1];
                par->info[1] = second->info[1];

                leaf->ptr[0] = second->ptr[2];
                second->ptr[2] = nullptr;

                second->del_key(second->key[1]);

                if (leaf->ptr[0] != nullptr) leaf->ptr[0]->parent = leaf;
            }
            else if (first->key_counter == 2){
                par->key[1] = second->key[0];
                par->info[1] = second->info[0];

                leaf->ptr[0] = second->ptr[1];
                second->ptr[1] = second->ptr[0];

                if (leaf->ptr[0] != nullptr) leaf->ptr[0]->parent = leaf;

                second->key[0] = par->key[0];
                second->info[0] = par->info[0];
                par->key[0] = first->key[1];
                par->info[0] = first->info[1];

                second->ptr[0] = first->ptr[2];
                if (second->ptr[0] != nullptr) second->ptr[0]->parent = second;

                first->del_key(first->key[1]);
                first->ptr[2] = nullptr;
            }
        }
    }
    else if (par->key_counter == 1){
        leaf->insert(par->key[0], par->info[0]);
        if (first == leaf && second->key_counter == 2){

            if (leaf->ptr[0] == nullptr){
                leaf->ptr[0] = leaf->ptr[1];
                leaf->ptr[1] = nullptr;
            }

            par->key[0] = second->key[0];
            par->info[0] = second->info[0];

            leaf->ptr[1] = second->ptr[0];
            if (leaf->ptr[1] != nullptr) leaf->ptr[1]->parent = leaf;

            second->ptr[0] = second->ptr[1];
            second->ptr[1] = second->ptr[2];
            second->ptr[2] = nullptr;
            second->del_key(second->key[0]);
        }
        else if (second == leaf && first->key_counter == 2){

            if (leaf->ptr[1] == nullptr){
                leaf->ptr[1] = leaf->ptr[0];
                leaf->ptr[0] = nullptr;
            }

            par->key[0] = first->key[1];
            par->info[0] = first->info[1];

            leaf->ptr[0] = first->ptr[2];
            if (leaf->ptr[0] != nullptr) leaf->ptr[0]->parent = leaf;
            first->ptr[2] = nullptr;

            first->del_key(first->key[1]);
        }
    }

    return par;
}

template <typename T>
inline Node<T> *BTree<T>::merge(Node<T> *leaf)
{
    Node<T>* par = leaf->parent;

    if(par->ptr[0] == leaf){
        par->ptr[1]->insert(par->key[0], par->info[0]);
        par->ptr[1]->ptr[2] = par->ptr[1]->ptr[1];
        par->ptr[1]->ptr[1] = par->ptr[1]->ptr[0];

        if (leaf->ptr[0] != nullptr) par->ptr[1]->ptr[0] = leaf->ptr[0];
        else if (leaf->ptr[1] != nullptr) par->ptr[1]->ptr[0] = leaf->ptr[1];

        if (par->ptr[1]->ptr[0] != nullptr) par->ptr[1]->ptr[0]->parent = par->ptr[1];

        par->del_key(par->key[0]);
        delete par->ptr[0];
        par->ptr[0] = nullptr;
    }
    else if (par->ptr[1] == leaf){
        par->ptr[0]->insert(par->key[0], par->info[0]);

        if (leaf->ptr[0] != nullptr) par->ptr[0]->ptr[2] = leaf->ptr[0];
        else if (leaf->ptr[1] != nullptr) par->ptr[0]->ptr[2] = leaf->ptr[1];

        if ( par->ptr[0]->ptr[2] != nullptr) par->ptr[0]->ptr[2]->parent = par->ptr[0];

        par->del_key(par->key[0]);
        delete par->ptr[1];
        par->ptr[1] = nullptr;

    }

    if (par->parent == nullptr){
        Node<T>* tmp;
        if (par->ptr[0] != nullptr) tmp = par->ptr[0];
        else tmp = par->ptr[1];
        tmp->parent = nullptr;
        root = tmp;
        delete par;
        return tmp;
    }

    return par;
}

template <typename T>
inline bool BTree<T>::empty()
{
    if (root == nullptr) return true;
    return false;
}
