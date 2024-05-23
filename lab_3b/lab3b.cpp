#include <iostream>
#include <string>
#include "Table.h"

using namespace std;

void menu(Table& table);
void print(Item& item);

int main(){
    int size_ks1 = 20, size_ks2 = 10;
    Table my_table;
    my_table.CreateTable("my_table", size_ks1, size_ks2);

    
    my_table.add(1, 0, "str1", 5, 32, "info1");
    my_table.add(2, 0, "str2", 77, 2, "new");
    my_table.add(3, 1, "str", 8, 13, "info2");
    my_table.add(8, 3, "str1", 51, 312, "info11");
    my_table.add(23, 2, "7ds", 12, 13, "ye7d");
    my_table.add(326, 2, "hd", 33, 18, "sdh2j");
    my_table.add(88, 2, "gs", 76, 12, "sh3bdj");
    my_table.add(374, 0, "7ds", 36, 111, "hd73hs");
    my_table.add(773, 0, "7ds", 382, 152, "dh3jq");
    my_table.add(97, 3, "gs", 3, 1, "dj");
    
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
        int key1, par;
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
        if (flag == -1) cout << "No table attached" << endl;
        else if (flag == -2) cout << "Table is full" << endl;
        else if (flag == -3) cout << "Key can't be 0" << endl;
        else if (flag == -4) cout << "No such parent key = " << par << endl;
        else if (flag == -5) cout << "key1 = " << key1 << " already exists. Key must be unique" << endl;
        menu(table);
    }
    else if (com == 2){
        int key1;
        string key2;
        cout << "Enter key1: ";
        cin >> key1;
        cout << "Enter key2: ";
        cin >> key2;
        Item info = table.find(key1, key2);
        if (info.key1 == 0) cout << "No such key or pair (key1, key2)";
        else print(info);
        menu(table);
    }
    else if (com == 3){
        int key1;
        string key2;
        cout << "Enter key1: ";
        cin >> key1;
        cout << "Enter key2: ";
        cin >> key2;
        int flag = table.del(key1, key2);
        if (flag == -1) cout << "No table attached" << endl;
        else if (flag == -2) cout << "No such key" << endl;
        else if (flag == -3) cout << "Can't delete, the key1 is the parent key" << endl;
        else if (flag == -4) cout << "No such pair of keys" << endl;
        menu(table);
    }
    else if (com == 4){
        int key1;
        cout << "Enter key1: ";
        cin >> key1;
        Item elem = table.find(key1);
        if (elem.key1 == 0) cout << "No such key" << endl;
        else print(elem);
        menu(table);
    }
    else if(com == 5){
        string key2, name;
        cout << "Enter key2: ";
        cin >> key2;
        cout << "Enter name of result table: ";
        cin >> name;
        table.find(name, key2);
        Table new_table;
        new_table.AttachTable(name);
        if (new_table.IsEmpty()) cout << "No such key" << endl;
        else new_table.print();
        menu(table);
    }
    else if (com == 6){
        int key1;
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
        table.print();
        menu(table);
    }
    else if (com == 9){
        int par;
        cout << "Enter parent key: ";
        cin >> par;
        string name;
        cout << "Enter name of result table: ";
        cin >> name;
        Table par_table;
        table.find_by_par(name, par);
        par_table.AttachTable(name);
        if (par_table.IsEmpty()) cout << "No such parent" << endl;
        else par_table.print();
        menu(table);
    }
    else if (com == 10){
        string key2;
        cout << "Enter key2: ";
        cin >> key2;
        int release;
        cout << "Enter release: ";
        cin >> release;
        Item elem = table.find(key2, release);
        if (elem.key1 == 0) cout << "No such key or release" << endl;
        else print(elem);
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

void print(Item &item)
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
    cout << right << item.key1;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << item.key2;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << right << item.num1;
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
    cout << right << item.num2;
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
    cout << right << item.str;
    cout.width(10);
    cout << right << "|" << endl;
    cout.width(70);
    cout << right;
    cout << "-------------------------------------------------------------";
    cout << endl;

}
