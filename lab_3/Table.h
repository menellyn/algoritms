#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

struct Node2;
struct Item;


struct InfoType{
    int num1;
    int num2;
    string str;

    InfoType(): num1(0), num2(0), str(""){};
    InfoType(int num1, int num2, string str): num1(num1), num2(num2), str(str) {};
};

// Структура элемента таблицы
struct Item{
	InfoType *info;  // указатель на информацию			

	unsigned int key1;  // ключ элемента из 1-го пространства ключей;			
	string key2;  // ключ элемента из 2-го пространства ключей;		

    KeySpace1* p1;  // связь с 1м ключом по указателю
    Node2* p2;  // связь со 2м ключом по указателю

    Item(): key1(0), key2(""), p1(nullptr), p2(nullptr), info(nullptr) {};
    Item(unsigned int key1, string key2, KeySpace1* p1, Node2* p2, InfoType *info):
            key1(key1), key2(key2), p1(p1), p2(p2), info(info) {};
    Item(unsigned int key1, string key2, InfoType* info):
            key1(key1), key2(key2), p1(nullptr), p2(nullptr), info(info) {};
    ~Item(){
            delete info;
    }	

};

// Первое пространство ключей организовано как просматриваемая таблица, 
// организованная вектором; каждый элемент таблицы имеет следующую структуру:
struct KeySpace1{
	unsigned int key;  // ключ элемента, не может быть 0
	unsigned int par;  // ключ родительского элемента			
	Item *info;  // указатель на информацию			

    KeySpace1(): key(0), par(0), info(nullptr){};

    KeySpace1(unsigned int key, unsigned int par, Item* info):
        key(key), par(par), info(info) {};
    
    KeySpace1& operator= (KeySpace1&& kSpace){  // перемещающий конструктор
        if (this != &kSpace){
            key = kSpace.key;
            par = kSpace.par;
            info = kSpace.info;
            kSpace.info = nullptr;
        }
        return *this;
    }

    KeySpace1& operator= (const KeySpace1& kSpace) = delete;

};

// Второе пространство ключей организовано как перемешанная таблица, использующая перемешивание сцеплением.
// Перемешанная таблица представлена массивом указателей на элементы таблицы, имеющие следующую структуру:
struct KeySpace2{
	string key;  // ключ элемента			
	int release_counter;  // количество использованных версий	
    int elem_counter;  // количество элементов
	Node2 *node;  // указатель на информацию		
	KeySpace2 *next;  // указатель на следующий элемент	

    KeySpace2(): key(""), release_counter(0), elem_counter(0), node(nullptr), next(nullptr) {};
    KeySpace2(string key, Node2 *node, KeySpace2 *next): 
            key(key), release_counter(1), elem_counter(1), node(node), next(next) {};
    KeySpace2(string key): key(key), release_counter(1), elem_counter(1), node(nullptr), next(nullptr) {};
};

// Указатель на информацию определяет список элементов с одинаковыми значениями ключей. 
// Элемент списка имеет следующую структуру:
struct Node2{
	int release;	// номер версии элемента		
	Item *info;		// указатель на информацию		
	Node2 *next; 		// указатель на следующий элемент	

    Node2(int rel, Item* info): release(rel), info(info), next(nullptr) {};
    Node2(Item* info): release(0), info(info), next(nullptr) {};

};

struct Table{
private:
    KeySpace1 	*ks1;  //указатель на первое пространство ключей
    KeySpace2 	**ks2;  //указатель на второе пространство ключей

    int msize1;  //размер области 1-го пространства ключей
    int msize2;	 //размер области 2-го пространства ключей
    int csize1;  //количество элементов в области 1-го пространства ключей
    int csize2;  //количество элементов в области 2-го пространства ключей

    KeySpace1* find_in_KS1(unsigned int key);
    KeySpace2* find_in_KS2(string key);
    Node2* find_in_KS2_rel(string key, int release);
    KeySpace1* add_in_KS1(unsigned int key, unsigned int par, Item *info);
    Node2* add_in_KS2(string key, Item *info);
    int hash_func(const string& s);
    bool IsParent(unsigned int par);

public:
    Table(int msize1, int msize2);
    ~Table();

    int add(unsigned int key1, unsigned int par, string key2, int info_num1, int info_num2, string info_str);
    void find(unsigned int key1, string key2);
    int del(unsigned int key1, string key2);
};