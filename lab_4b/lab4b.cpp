#include "BTree.h"
#include <array>
#include <typeinfo>
#include <sstream>

using namespace std;

template <typename T>
void menu(BTree<T>& tree);
template <typename T>
void print(T key, InfoType* info);
template <typename T>
int fill_tree(string adrs, BTree<T>& tree);

void TestCreate();
void TestInsert(BTree<int>& tree);
void TestDelete(BTree<int>& tree);
void TestSearch(BTree<int>& tree);
void TestFindNext(BTree<int>& tree);
void TestCopyConstructor(BTree<int>& tree);
void TestMoveConstructor(BTree<int>& tree);

#define MAX_KEY_LEN 10

int main(){
    TestCreate();
    BTree<int> my_tree;
    fill_tree("tree_int", my_tree);
    //BTree<string> str_tree;
    //fill_tree("tree_str", str_tree);
    menu(my_tree);

}

template <typename T>
void menu(BTree<T>& tree){
    cout << endl;
    cout << "menu:" << endl;
    cout << "1. Add new element\n2. Delete element" << endl;
    cout << "3. Find information\n4. Print table" << endl;
    cout << "5. Find next element\n6. Draw tree" << endl;
    int com;
    cout << "Enter command: ";
    cin >> com;
    if (com == 1){
        T key;
        float num1, num2;
        string info_str;
        cout << "Enter key: ";
        if (typeid(key).name() == typeid(string).name()){
            string str;
            cin >> str;
            if (str.length() > MAX_KEY_LEN) str = str.substr(0, MAX_KEY_LEN);
            to_string(key) = str;
        }
        else cin >> key;
        cout << "Enter num1: ";
        cin >> num1;
        cout << "Enter num2: ";
        cin >> num2;
        cout << "Enter string: ";
        cin >> info_str;
        tree.insert(key, num1, num2, info_str);
        menu(tree);
    }
    else if (com == 2){
        T key;
        cout << "Enter key: ";
        cin >> key;
        int res = tree.del(key);
        if (res == -1) cout << "Tree is empty"<< endl;
        if (res == -2) cout << "No such key" << endl;
        if (res == 0) cout << "Delete!"<< endl;
        menu(tree);
    }
    else if (com == 3){
        T key;
        cout << "Enter key: ";
        cin >> key;
        InfoType* info = tree.search(key);
        if (info == nullptr) cout << "No such key" << endl;
        else print(key, info);
        menu(tree);
    }
    else if (com ==4){
        if (tree.empty()) cout << "Tree is empty" << endl;
        else tree.print();
        menu(tree);
    }
    else if (com == 5){
        T key;
        cout << "Enter key: ";
        cin >> key;
        InfoType* info = tree.find_next(key);
        T next_key = tree.find_next_key(key);
        if (info == nullptr) cout << "No such element" << endl;
        else print(next_key, info);
        menu(tree);
    }
    else if (com == 6){
        if (tree.empty()) cout << "Tree is empty" << endl;
        else tree.draw();
        // dot -Tpng tree.dot -O
        menu(tree);
    }
}

template <typename T>
void print(T key, InfoType *info)
{
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
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << key;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << info->num1;
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << " ";
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << info->num2;
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << " ";
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << info->str;
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(50);
    cout << right;
    cout << "-----------------------------------------";
    cout << endl;
}

template <typename T>
int fill_tree(string adrs, BTree<T> &tree)
{
    ifstream in(adrs);
    T key;
    float num1, num2;
    string info;
    string str;

    while (!in.eof()){
        getline(in, str);
        if (str.length() > MAX_KEY_LEN) str = str.substr(0, MAX_KEY_LEN);
        stringstream ss(str);
        ss >> key;
        getline(in, str);
        stringstream snum1(str);
        snum1 >> num1;
        getline(in, str);
        stringstream snum2(str);
        snum2 >> num2;
        getline(in, info);
        tree.insert(key, num1, num2, info);
    }
    
    in.close();
    return 0;
}

void TestCreate(){
    BTree<int> test_tree;
    if (test_tree.empty()) cout << "TestCreate - OK" << endl;
    else cout << "TestCreate - ERROR" << endl; 
    TestInsert(test_tree);
    TestCopyConstructor(test_tree);
    TestSearch(test_tree);
    TestFindNext(test_tree);
    TestDelete(test_tree);
}

void TestInsert(BTree<int> &tree)
{
    tree.insert(7, 3.2, 44.3, "fehuw3");
    tree.insert(3, 22.1, 66.4, "dh2s");
    tree.insert(15, 21.3, 12.64, "d2ks");
    tree.insert(4, 12.5, 7.2, "g84hd");
    tree.insert(12, 11.3, 443.2, "t73wj");
    tree.insert(3, 8.5, 87.2, "fey2");
    tree.insert(21, 4.3, 65.21, "jri3");
    tree.insert(11, 43.21, 66.6, "jf3kw1");
    tree.insert(2, 77.7, 98.5, "r3js");
    vector<int> expected_LNR = {2, 3, 4, 7, 11, 12, 15, 21};
    bool LNR = expected_LNR == tree.symmetric_traversal();
    if (LNR) cout << "TestInsert - OK" << endl;
    else cout << "TestInsert - ERROR" << endl;

}

void TestDelete(BTree<int> &tree)
{
    tree.del(11);
    vector<int> expected_LNR = {2, 3, 4, 7, 12, 15, 21};    
    bool LNR = expected_LNR == tree.symmetric_traversal();
    if (LNR) cout << "TestDelete key in leaf with two keys - OK" << endl;
    else cout << "TestDelete key in leaf with two keys - ERROR" << endl;

    tree.del(15);
    expected_LNR = {2, 3, 4, 7, 12, 21};    
    LNR = expected_LNR == tree.symmetric_traversal();
    if (LNR) cout << "TestDelete leaf (parent and brother has one key) - OK" << endl;
    else cout << "TestDelete leaf (parent and brother has one key) - ERROR" << endl;

    tree.del(2);
    expected_LNR = {3, 4, 7, 12, 21};  
    LNR = expected_LNR == tree.symmetric_traversal();
    if (LNR) cout << "TestDelete leaf (parent has 2 keys and brother has 2 keys) - OK" << endl;
    else cout << "TestDelete leaf (parent has 2 keys and brother has 2 keys) - ERROR" << endl;

    tree.del(4);
    expected_LNR = {3, 7, 12, 21};  
    LNR = expected_LNR == tree.symmetric_traversal();
    if (LNR) cout << "TestDelete leaf (parent has 2 keys and brothers has 1 key) - OK" << endl;
    else cout << "TestDelete leaf (parent has 2 keys and brothers has 1 key) - ERROR" << endl;

    tree.del(21);
    expected_LNR = {3, 7, 12};  
    LNR = expected_LNR == tree.symmetric_traversal();
    if (LNR) cout << "TestDelete leaf (parent has 1 key and brother has 2 keys) - OK" << endl;
    else cout << "TestDelete leaf (parent has 1 key and brother has 2 keys) - ERROR" << endl;

}

void TestSearch(BTree<int> &tree)
{
    InfoType* info = tree.search(3);
    float num1 = 8.5;
    float num2 = 87.2;
    string str = "fey2";
    if (num1 == info->num1 && num2 == info->num2 && str == info->str) cout << "TestSearch - OK" << endl;
    else cout << "TestSearch - ERROR" << endl;

}

void TestFindNext(BTree<int> &tree)
{
    int key = tree.find_next_key(2);
    InfoType* info = tree.find_next(2);
    float num1 = 8.5;
    float num2 = 87.2;
    string str = "fey2";
    int exp_key = 3;
    if (key == exp_key && num1 == info->num1 && num2 == info->num2 && str == info->str) cout << "TestFindNext - OK" << endl;
    else cout << "TestFindNext - ERROR" << endl;
}

void TestCopyConstructor(BTree<int> &tree)
{
    BTree<int> new_tree(tree);
    vector<int> expected_LNR = {2, 3, 4, 7, 11, 12, 15, 21};
    bool LNR = expected_LNR == new_tree.symmetric_traversal();
    if (LNR) cout << "TestCopyConstructor - OK" << endl;
    else cout << "TestCopyConstructor - ERROR" << endl;
    TestMoveConstructor(new_tree);
}

void TestMoveConstructor(BTree<int> &tree)
{
    BTree<int> new_tree(std::move(tree));
    vector<int> expected_LNR = {2, 3, 4, 7, 11, 12, 15, 21};
    bool LNR = expected_LNR == new_tree.symmetric_traversal();
    if (LNR && tree.empty()) cout << "TestMoveConstructor - OK" << endl;
    else cout << "TestMoveConstructor - ERROR" << endl;
}
