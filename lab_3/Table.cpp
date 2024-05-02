#include "Table.h"
#include <iomanip>

Table::Table(int msize1, int msize2):
msize1(msize1), msize2(msize2), csize1(0), csize2(0)
{
    try {
        ks1 = new KeySpace1[msize1];
        KeySpace2** ks2 = new KeySpace2*[msize2];
    }
    catch (std::bad_alloc& ex) {
        cout << "Caught bad_alloc: " << ex.what() << endl;
        return;
    }
    for (int i = 0; i < msize2;  ++i){
        ks2[i] = nullptr;
    }

}

Table::Table(const Table &other): msize1(other.msize1), msize2(other.msize2), csize1(0), csize2(0)
{
    try {
        ks1 = new KeySpace1[msize1];
        KeySpace2** ks2 = new KeySpace2*[msize2];
    }
    catch (std::bad_alloc& ex) {
        cout << "Caught bad_alloc: " << ex.what() << endl;
        return;
    }
    for (int i = 0; i < msize2;  ++i){
        ks2[i] = nullptr;
    }
    for(int i = 0; i < other.csize1; ++i){
        InfoType* info = new InfoType(other.ks1[i].info->info->num1, other.ks1[i].info->info->num2,
            other.ks1[i].info->info->str);
        Item* item = new Item(other.ks1[i].info->key1, other.ks1[i].info->key2, info);
        item->p1 = add_in_KS1(other.ks1[i].key, other.ks1[i].par, item);
        item->p2 = add_in_KS2(other.ks1[i].info->key2, item);
    }
}

Table::~Table()
{}

int Table::add(unsigned int key1, unsigned int par, string key2, int info_num1, int info_num2, string info_str)
{
    if (csize1 == msize1){
        cout << "Table is full" << endl;
        return -1;
    }
    if (key1 == 0){
        cout << "Key can't be 0" << endl;
        return -1;
    }
    if (!find_in_KS1(par) && par!=0 ){
        cout << "No such parent key = " << par << endl;
        return -1;
    }
    if (find_in_KS1(key1)){
        cout << "key1 = " << key1 << " already exists. Key must be unique" << endl;
        return -1;
    }
    if (find_in_KS1(key1) && find_in_KS2(key2)){
        cout << "(key1, key2) must be unique" << endl;
        return -1;
    }

    InfoType* info = new InfoType(info_num1, info_num2, info_str);
    Item* item = new Item(key1, key2, info);
    item->p1 = add_in_KS1(key1, par, item);
    item->p2 = add_in_KS2(key2, item);
    return 0;
}

void Table::find(unsigned int key1, string key2)
{
    if (!find_in_KS1(key1) || !find_in_KS2(key2) || key1 == 0){
        cout << "No such key" << endl;
        return;
    }
    if (find_in_KS1(key1)->info->key2 != key2){
        cout << "No such pair of keys" << endl;
        return;
    }
    InfoType* target_info = find_in_KS1(key1)->info->info;
    cout << "num1: " << target_info->num1 << endl;
    cout << "num2: " << target_info->num2 << endl;
    cout << "str: " << target_info->str << endl;
}

void Table::find(string key2, int release)
{
    Node2* node  = find_in_KS2_rel(key2, release);
    if (!node){
        cout << "No such key or release" << endl;
        return;
    }
    InfoType* target_info = node->info->info;
    cout << "num1: " << target_info->num1 << endl;
    cout << "num2: " << target_info->num2 << endl;
    cout << "str: " << target_info->str << endl;
}

int Table::del(unsigned int key1, string key2)
{
    if (!find_in_KS1(key1) || !find_in_KS2(key2)){
        cout << "No such key" << endl;
        return -1;
    }
    if (find_in_KS1(key1)->info->key2 != key2){
        cout << "No such pair of keys" << endl;
        return -1;
    }
    if (IsParent(key1)){
        cout << "Can't delete, the key1 is the parent key" << endl;
        return -1;
    }

    KeySpace1* unit1 = find_in_KS1(key1);
    Node2* unit2 = unit1->info->p2;

    Item* del_item = unit1->info;
    InfoType* del_info = del_item->info;
    del_item->info = nullptr;
    del_item->p1 = nullptr;
    del_item->p2 = nullptr;
    delete del_info;
    delete del_item;


    while ((unit1+1)->key != 0){
        unit1->key = (unit1 + 1)->key;
        unit1->par = (unit1 + 1)->par;
        unit1->info = (unit1 + 1)->info;
        unit1 = unit1 + 1;
    }
    csize1--;

    KeySpace2* ks2_unit = find_in_KS2(key2);
    csize2--;

    if (ks2_unit->elem_counter == 1){
        delete unit2;
        int init_pos = hash_func(key2);
        KeySpace2* previous = ks2[init_pos];
        if (previous->key == key2){
            previous = previous->next;
            ks2_unit->next = nullptr;
            delete ks2_unit;
            return 0;
        }
        bool IsPrevious = (previous->next->key == key2);
        while (!IsPrevious){
            previous = previous->next;
            IsPrevious = (previous->next->key == key2);
        }
        previous->next = ks2_unit->next;
        ks2_unit->next = nullptr;
        delete ks2_unit;
        return 0;
    }

    if (ks2_unit->node->release == unit2->release){
        ks2_unit->node = unit2->next;
        unit2->next = nullptr;
        delete unit2;
        ks2_unit->elem_counter--;
        return 0;
    }
    else{
        Node2* prev_node = ks2_unit->node;
        bool IsPrevious = (prev_node->next->release == unit2->release);
        while (!IsPrevious){
            prev_node = prev_node->next;
            IsPrevious = (prev_node->next->release == unit2->release);
        }
        prev_node->next = unit2->next;
        unit2->next = nullptr;
        delete unit2;
        ks2_unit->elem_counter--;
        return 0;

    }
}

int Table::del(string key, int release)
{
    Node2* del_node = find_in_KS2_rel(key, release);

    if (!del_node){
        cout << "No such key or release" << endl;
        return -1;
    }

    unsigned int key1 = del_node->info->key1;
    del(key1, key);
    return 0;
}

int Table::del(unsigned int key1)
{
    KeySpace1* del_ks1 = find_in_KS1(key1);

    if (!del_ks1){
        cout << "No such key" << endl;
        return -1;
    }

    string key2 = del_ks1->info->key2;
    del(key1, key2);
    return 0;
}

int Table::del(string key2)
{
    KeySpace2* del_ks2 = find_in_KS2(key2);

    if (!del_ks2){
        cout << "No such key" << endl;
        return -1;
    }

    Node2* del_node;
    unsigned int key1;
    int n = del_ks2->elem_counter;
    for (int i = 0; i < n; ++i){
        del_node = del_ks2->node;
        key1 = del_node->info->key1;
        del(key1, key2);
    }
    return 0;

}

void Table::print_table()
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
    for (int i = 0; i < csize1; ++i){
        Item* item_info = ks1[i].info; 
        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << right << item_info->key1;
        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << right << item_info->key2;
        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << right << item_info->info->num1;
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
        cout << right << item_info->info->num2;
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
        cout << right << item_info->info->str;
        cout.width(10);
        cout << right << "|" << endl;
        cout.width(70);
        cout << right;
        cout << "-------------------------------------------------------------";
        cout << endl;
    }
}

Table Table::find_by_parent(unsigned int par)
{
    Table out(msize1, msize2);

    for (int i = 0; i< csize1; ++i){
        if (ks1[i].par == par){
            Item* item = ks1[i].info;
            out.add(item->key1, 0, item->key2, item->info->num1, item->info->num2, item->info->str);
        }
    }

    return out;
}

Table Table::find_by_key2(string key2)
{
    Table out(msize1, msize2);

    KeySpace2* actual = find_in_KS2(key2);
    Node2* actual_node = actual->node;
    for(int i = 0; i < actual->elem_counter; ++i){
        Item* item = actual_node->info;
        if (out.find_in_KS1(item->p1->par)){
            out.add(item->key1, item->p1->par, item->key2, item->info->num1, item->info->num2, item->info->str);
        }
        else{
            out.add(item->key1, 0, item->key2, item->info->num1, item->info->num2, item->info->str);
        }
        actual_node->next;
    }

    return out;
}

int Table::hash_func(const string &s)
{
    int h=0;
    for (int i=0; i<s.size(); i++){
        h+=(int) s[i];
    }
    return h%msize2;
}

bool Table::IsParent(unsigned int par)
{
    int begin = 0;
    int end = csize1 - 1;
    int j;
    while ( begin <= end){
        j = (begin + end)/2;
        if (ks1[j].par == par) return true;
        if (ks1[j].par > par) end = j-1;
        if (ks1[j].par < par) begin = j+1;
    }
    return false;
}

KeySpace1 *Table::find_in_KS1(unsigned int key)
{
    if (csize1 == 0) return nullptr;
    KeySpace1* target = nullptr;
    KeySpace1* actual = ks1;
    bool IsTarget = (actual->key == key);
    int i = 0;
    while (!IsTarget && i < csize1){
        actual = (actual + 1);
        IsTarget = (actual->key == key);
        ++i;
    }
    if (IsTarget) target = actual;
    return target;
}

KeySpace2 *Table::find_in_KS2(string key)
{
    if (csize2 == 0) return nullptr;
    KeySpace2* target = nullptr;
    int init_pos = hash_func(key);
    KeySpace2* actual = *(ks2+init_pos);
    if (actual == nullptr) return nullptr;
    bool IsTarget = (actual->key == key);
    while (!IsTarget && actual->next){
        actual = actual->next;
        IsTarget = (actual->key == key);
    }
    if (IsTarget) target = actual;
    return target;
}

Node2 *Table::find_in_KS2_rel(string key, int release)
{
    if (csize2 == 0) return nullptr;
    KeySpace2* target_ks = find_in_KS2(key);
    Node2* target_node = nullptr;
    if (target_ks == nullptr) return target_node;

    bool IsTarget = (target_ks->node->release == release);
    Node2* tmp = target_ks->node;

    while (!IsTarget && tmp->next)
    {
        tmp = tmp->next;
        IsTarget = (tmp->release == release);
    }

    if (IsTarget) target_node = tmp;
    return target_node;
}

KeySpace1 *Table::add_in_KS1(unsigned int key, unsigned int par, Item *info)
{   
    int i = csize1;
    if (csize1 > 0){
        while (i >= 0 && ks1[i-1].par > par){
            ks1[i].key = ks1[i-1].key;
            ks1[i].par = ks1[i-1].par;
            ks1[i].info = ks1[i-1].info;
            i--;
        }
    }
    ks1[i].key = key;
    ks1[i].par = par;
    ks1[i].info = info;
    ++csize1;
    return (ks1+i);
}

Node2 *Table::add_in_KS2(string key, Item* info)
{
    int init_pos = hash_func(key);
    KeySpace2* actual = find_in_KS2(key);
    Node2* new_node = new Node2(info);
    if (actual == nullptr){
        KeySpace2* new_key = new KeySpace2(key);
        new_node->release = new_key->release_counter;
        new_key->node = new_node;
        new_key->next = *(ks2 + init_pos);
        *(ks2 + init_pos) = new_key;
    }
    else{
        actual->release_counter = actual->release_counter + 1;
        actual->elem_counter = actual->elem_counter + 1;
        new_node->release = actual->release_counter;
        new_node->next = actual->node;
        actual->node = new_node;
    }
    ++csize2;
    return new_node;
}

