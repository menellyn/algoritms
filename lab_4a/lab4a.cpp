#include "BinaryTree.h"

using namespace std;

template <typename T>
void menu(BinaryTree<T>& tree);
template <typename T>
void print(vector<Node<T>*>& vec);
template <typename T>
int fill_tree(string adrs, BinaryTree<T>& tree);

int main(){
    
    BinaryTree<int> tree;
    tree.insert(7, "fehuw3");
    tree.insert(3, "dh2s");
    tree.insert(15, "d2ks");
    tree.insert(4, "g84hd");
    tree.insert(12, "t73wj");
    tree.insert(3, "fey2");
    tree.insert(21, "jri3");
    tree.insert(11, "jf3kw1");
    tree.insert(2, "r3js");
    //tree.draw();

    BinaryTree<string> new_tree;
    fill_tree("tree1", new_tree);
    new_tree.draw();
    
    //menu(tree);
}

template <typename T>
void menu(BinaryTree<T>& tree){
    cout << "menu:" << endl;
    cout << "0. Print menu" << endl;
    cout << "1. Add new element\n2. Delete element" << endl;
    cout << "3. Find information\n4. Print table" << endl;
    cout << "5. Find close element\n6. Draw tree" << endl;
    int com;
    cin >> com;
    while (com){
        if (com == 1){
            T key;
            string info;
            cout << "Enter key: ";
            cin >> key;
            cout << "Enter information: ";
            cin >> info;
            tree.insert(key, info);
        }
        else if (com == 2){
            T key;
            cout << "Enter key: ";
            cin >> key;
            int res = tree.del(key);
            if (res == -1) cout << "Tree is empty"<< endl;
            if (res == -2) cout << "No such key" << endl;
            if (res == 0) cout << "Delete!"<< endl;
        }
        else if (com == 3){
            T key;
            cout << "Enter key: ";
            cin >> key;
            vector<Node<T>*> elems = tree.find_elems(key);
            if (elems.empty()) cout << "No such key" << endl;
            else print(elems);
        }
        else if (com ==4){
            T key;
            cout << "Enter key to output items larger then specified key (press ENTER to print entire table)" << endl;
            cin >> key;
            if (key) tree.print(key);
            else tree.print();
        }
        else if (com == 5){
            T key;
            cout << "Enter key: ";
            cin >> key;
            vector<Node<T>*> elems = tree.find_close_elem(key);
            if (elems.empty()) cout << "No such key" << endl;
            else print(elems);
        }
        else if (com == 6){
            tree.draw();
            // dot -Tpng tree.dot -O
        }
        else if (com == 0){
            cout << "menu:" << endl;
            cout << "0. Print menu" << endl;
            cout << "1. Add new element\n2. Delete element" << endl;
            cout << "3. Find information\n4. Print table" << endl;
            cout << "5. Find close element\n6. Draw tree" << endl;
            cin >> com;
        }
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

    while (!in.eof()){
        getline(in, key);
        getline(in, info);
        tree.insert(key, info);
    }
    
    in.close();
    return 0;
}