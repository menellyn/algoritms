#include <cstdio>
#include <iostream>
#include <string>
#include "Table.h"

using namespace std;
void menu(Table& table);
void print(unsigned int, string, InfoType*);

int main(){
    int size_ks1 = 100, size_ks2 = 20;
    Table my_table(size_ks1, size_ks2);


    my_table.add(1, 0, "str1", 5, 32, "info1");
    my_table.add(2, 0, "str2", 77, 2, "new_info");
    my_table.add(3, 1, "strstr", 8, 13, "new_info2");
    my_table.add(8, 3, "str1", 51, 312, "info11");
    my_table.add(23, 2, "7ds72", 12, 13, "ye7d");
    my_table.add(326, 2, "hds8wh", 33, 18, "sdh2j");
    my_table.add(88, 2, "gsayd43g", 76, 12, "sh3bdj");
    my_table.add(374, 0, "7ds72", 36, 111, "hd73hs");
    my_table.add(773, 0, "7ds72", 382, 152, "dh3jq");
    my_table.add(97, 3, "gsayd43g", 3828, 127, "dj3j2k");

    menu(my_table);

}

void menu(Table &table)
{
    cout << endl;
    cout << "menu:" << endl;
    cout << "1. Add new element\n2. Find by (key1, key2)" << endl;
    cout << "3. Delete element by (key1, key2)\n4. Find by key1\n5. Find by key2" << endl;
    cout << "6. Delete element by key1\n7. Delete elements by key2\n8. Print table" << endl;
    cout << "9. Find by parent key\n10. Find by key2 and release" << endl;
    cout << "11. Delete by key2 and release" << endl;
    int com;
    cout << "Enter command: ";
    cin >> com;
    if (com == 1){
        unsigned int key1, par;
        string key2, str_info;
        int num1, num2;
        cout << "Enter key1: ";
        cin >> key1;
        cout << "Enter parent key: ";
        cin >> par;
        cout << "Enter key2: ";
        cin >> key2;
        cout << "Enter num1: ";
        cin >> num1;
        cout << "Enter num2: ";
        cin >> num2;
        cout << "Enter str: ";
        cin >> str_info;
        int flag = table.add(key1, par, key2, num1, num2, str_info);
        if (flag == -1) cout << "Table is full" << endl;
        else if (flag == -2) cout << "Key can't be 0" << endl;
        else if (flag == -3) cout << "No such parent key = " << par << endl;
        else if (flag == -4) cout << "key1 = " << key1 << " already exists. Key must be unique" << endl;
        else if (flag == -5) cout << "(key1, key2) must be unique" << endl;
        menu(table);
    }
    else if (com == 2){
        unsigned int key1;
        string key2;
        cout << "Enter key1: ";
        cin >> key1;
        cout << "Enter key2: ";
        cin >> key2;
        InfoType* info = table.find(key1, key2);
        if (info == nullptr) cout << "No such key or pair (key1, key2)";
        else print(key1, key2, info);
        menu(table);
    }
    else if (com == 3){
        unsigned int key1;
        string key2;
        cout << "Enter key1: ";
        cin >> key1;
        cout << "Enter key2: ";
        cin >> key2;
        int flag = table.del(key1, key2);
        if (flag == -1) cout << "No such key" << endl;
        else if (flag == -2) cout << "No such pair of keys" << endl;
        else if (flag == -3) cout << "Can't delete, the key1 is the parent key" << endl;
        menu(table);
    }
    else if (com == 4){
        unsigned int key1;
        cout << "Enter key1: ";
        cin >> key1;
        pair<string, InfoType> elem = table.find(key1);
        if (elem.first == "") cout << "No such key" << endl;
        else print(key1, elem.first, &elem.second);
        menu(table);
    }
    else if(com == 5){
        string key2;
        cout << "Enter key2: ";
        cin >> key2;
        Table new_table = table.find_by_key2(key2);
        if (new_table.empty()) cout << "No such key" << endl;
        else new_table.print_table();
        menu(table);
    }
    else if (com == 6){
        unsigned int key1;
        cout << "Enter key1: ";
        cin >> key1;
        int flag = table.del(key1);
        if (flag == -1) cout << "No such key" << endl;
        menu(table);
    }
    else if (com == 7){
        string key2;
        cout << "Enter key2: ";
        cin >> key2;
        int flag = table.del(key2);
        if (flag == -1) cout << "No such key" << endl;
        menu(table);
    }
    else if (com == 8){
        table.print_table();
        menu(table);
    }
    else if (com == 9){
        unsigned int par;
        cout << "Enter parent key: ";
        cin >> par;
        Table par_table = table.find_by_parent(par);
        if (par_table.empty()) cout << "No such parent" << endl;
        else par_table.print_table();
        menu(table);
    }
    else if (com == 10){
        string key2;
        cout << "Enter key2: ";
        cin >> key2;
        int release;
        cout << "Enter release: ";
        cin >> release;
        pair<unsigned int, InfoType*> elem = table.find(key2, release);
        if (elem.first == 0) cout << "No such key or release" << endl;
        else print(elem.first, key2, elem.second);
        menu(table);
    }
    else if (com == 11){
        string key2;
        cout << "Enter key2: ";
        cin >> key2;
        int release;
        cout << "Enter release: ";
        cin >> release;
        int flag = table.del(key2, release);
        if (flag == -1) cout << "No such key" << endl;
        menu(table);
    }


}

void print(unsigned int key1, string key2, InfoType *info)
{
    cout.width(70);
    cout << right;
    cout << "-------------------------------------------------------------";
    cout << endl;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << "key1" ;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << "key2";
    cout.width(10);
    cout << right << "|";
    cout.width(10);
    cout << right << "info";
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(70);
    cout << right;
    cout << "-------------------------------------------------------------";
    cout << endl;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << key1;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << key2;
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
    cout << right << " ";
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << info->str;
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(70);
    cout << right;
    cout << "-------------------------------------------------------------";
    cout << endl;

}
