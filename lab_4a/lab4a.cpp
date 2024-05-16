#include "BinaryTree.h"
#include <sstream>

using namespace std;

void TestInsert(BinaryTree<int>& tree);
void TestCreate();
void TestDelete(BinaryTree<int>& tree);
void TestSearch(BinaryTree<int>& tree);
void TestFindCloseElem(BinaryTree<int>& tree);
template <typename T>
void menu(BinaryTree<T>& tree);
template <typename T>
void print(vector<Node<T>*>& vec);
template <typename T>
int fill_tree(string adrs, BinaryTree<T>& tree);

int main(){
    
    TestCreate();

    BinaryTree<int> new_tree;
    fill_tree("tree_int", new_tree);
    new_tree.draw();
    new_tree.print(6);
    
    menu(new_tree);
    
}

template <typename T>
void menu(BinaryTree<T>& tree){
    cout << endl;
    cout << "menu:" << endl;
    cout << "1. Add new element\n2. Delete element" << endl;
    cout << "3. Find information\n4. Print table" << endl;
    cout << "5. Find close element\n6. Draw tree" << endl;
    cout << "7. Print table with key values exceeding the specified one" << endl;
    int com;
    cin >> com;
    if (com == 1){
        T key;
        string info;
        cout << "Enter key: ";
        cin >> key;
        cout << "Enter information: ";
        cin >> info;
        tree.insert(key, info);
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
        vector<Node<T>*> elems = tree.find_elems(key);
        if (elems.empty()) cout << "No such key" << endl;
        else print(elems);
        menu(tree);
    }
    else if (com ==4){
        tree.print();
        menu(tree);
    }
    else if (com == 5){
        T key;
        cout << "Enter key: ";
        cin >> key;
        vector<Node<T>*> elems = tree.find_close_elem(key);
        if (elems.empty()) cout << "No such key" << endl;
        else print(elems);
        menu(tree);
    }
    else if (com == 6){
        tree.draw();
        // dot -Tpng tree.dot -O
        menu(tree);
    }
    else if (com == 7){
        T key;
        cout << "Enter key to output items larger then specified key" << endl;
        cin >> key;
        tree.print(key);
        menu(tree);
    }
}

template <typename T>
void print(vector<Node<T>*>& vec){
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
int fill_tree(string adrs, BinaryTree<T>& tree){
    ifstream in(adrs);
    T key;
    string info;
    string str;

    while (!in.eof()){
        getline(in, str);
        stringstream ss(str);
        ss >> key;
        getline(in, info);
        tree.insert(key, info);
    }
    
    in.close();
    return 0;
}

void TestCreate()
{
    BinaryTree<int> tree;
    if (tree.is_empty()) cout << "TestCreate - OK" << endl;
    else cout << "TestCreate - ERROR" << endl; 
    TestInsert(tree);
    TestDelete(tree);
    TestSearch(tree);
    TestFindCloseElem(tree);

}

void TestInsert(BinaryTree<int>& tree){
    tree.insert(7, "fehuw3");
    tree.insert(3, "dh2s");
    tree.insert(15, "d2ks");
    tree.insert(4, "g84hd");
    tree.insert(12, "t73wj");
    tree.insert(3, "fey2");
    tree.insert(21, "jri3");
    tree.insert(11, "jf3kw1");
    tree.insert(2, "r3js");
    vector<int> expected_LNR = {2, 3, 3, 4, 7, 11, 12, 15, 21};
    vector<int> expected_NLR = {7, 3, 2, 4, 3, 15, 12, 11, 21};
    bool LNR = expected_LNR == tree.LNR_traversal();
    bool NLR = expected_NLR == tree.NLR_traversal();
    if (LNR && NLR) cout << "TestInsert - OK" << endl;
    else cout << "TestInsert - ERROR" << endl;
}

void TestDelete(BinaryTree<int>& tree)
{
    tree.del(11);
    vector<int> expected_LNR = {2, 3, 3, 4, 7, 12, 15, 21};
    vector<int> expected_NLR = {7, 3, 2, 4, 3, 15, 12, 21};
    bool LNR = expected_LNR == tree.LNR_traversal();
    bool NLR = expected_NLR == tree.NLR_traversal();
    if (LNR && NLR) cout << "TestDelete sheet - OK" << endl;
    else cout << "TestDelete sheet - ERROR" << endl;
    tree.del(4);
    expected_LNR = {2, 3, 3, 7, 12, 15, 21};
    expected_NLR = {7, 3, 2, 3, 15, 12, 21};
    LNR = expected_LNR == tree.LNR_traversal();
    NLR = expected_NLR == tree.NLR_traversal();
    if (LNR && NLR) cout << "TestDelete element with one child - OK" << endl;
    else cout << "TestDelete element with one child - ERROR" << endl;
    tree.del(7);
    expected_LNR = {2, 3, 3, 12, 15, 21};
    expected_NLR = {12, 3, 2, 3, 15, 21};
    LNR = expected_LNR == tree.LNR_traversal();
    NLR = expected_NLR == tree.NLR_traversal();
    if (LNR && NLR) cout << "TestDelete element with two child - OK" << endl;
    else cout << "TestDelete element with two child - ERROR" << endl;
}

void TestSearch(BinaryTree<int> &tree)
{
    vector<Node<int>*> elems = tree.find_elems(3);
    vector<string> info;
    for (int i = 0; i < elems.size(); ++i){
        info.push_back(elems[i]->info->str);
    }
    vector<string> expected_info = {"dh2s", "fey2"};
    if (info == expected_info) cout << "TestSearch - OK" << endl;
    else cout << "TestSearch - ERROR" << endl;

}

void TestFindCloseElem(BinaryTree<int> &tree)
{
    vector<Node<int>*> elems = tree.find_close_elem(2);
    vector<string> info;
    for (int i = 0; i < elems.size(); ++i){
        info.push_back(elems[i]->info->str);
    }
    vector<string> expected_info = {"dh2s", "fey2"};
    if (info == expected_info) cout << "TestFindCloseElem - OK" << endl;
    else cout << "TestFindCloseElem - ERROR" << endl;
}
