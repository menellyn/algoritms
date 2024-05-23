#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

struct Item;
struct KeySpace1;
struct KeySpace2;
struct Node2;


// Структура элемента таблицы
struct Item{ 		
	int key1;
	string key2; 
    int par;
    int release;	

    int num1, num2;
    string str; 	

    void save(fstream& info){
        int len = key2.length() + 1;
        info.write((char*)&key1, sizeof(int));
        info.write((char*)&len, sizeof(int));
        info.write((char*)key2.c_str(), len);
        info.write((char*)&par, sizeof(int));
        info.write((char*)&release, sizeof(int));

        len = str.length() + 1;
        info.write((char*)&num1, sizeof(int));
        info.write((char*)&num2, sizeof(int));
        info.write((char*)&len, sizeof(int));
        info.write((char*)str.c_str(), len);
    }

    void load(fstream& info){
        int len;
        info.read((char*)&key1, sizeof(int));
        info.read((char*)&len, sizeof(int));
        char * buf = new char[len];
        info.read(buf, len);
        key2 = buf;
        delete[] buf;
        info.read((char*)&par, sizeof(int));
        info.read((char*)&release, sizeof(int));

        info.read((char*)&num1, sizeof(int));
        info.read((char*)&num2, sizeof(int));
        info.read((char*)&len, sizeof(int));
        buf = new char[len];
        info.read(buf, len);
        str = buf;
        delete[] buf;
    }
};

// Первое пространство ключей организовано как просматриваемая таблица, 
// организованная вектором; каждый элемент таблицы имеет следующую структуру:
struct KeySpace1{
	int key = 0;  
	int par = 0;  		
	int info_offset;  

    void save(fstream& ks1){
        ks1.write((char*)&key, sizeof(int));
        ks1.write((char*)&par, sizeof(int));
        ks1.write((char*)&info_offset, sizeof(int));
    }

    void load(fstream& ks1){
        ks1.read((char*)&key, sizeof(int));
        ks1.read((char*)&par, sizeof(int));
        ks1.read((char*)&info_offset, sizeof(int));
    }		
};

// Второе пространство ключей организовано как перемешанная таблица, использующая перемешивание сцеплением.
// Перемешанная таблица представлена массивом указателей на элементы таблицы, имеющие следующую структуру:
struct KeySpace2{
	string key;  		
	int node_offset = -1;		
	int next_elem_offset = -1; 

    void save(fstream& ks2){
        int len = key.length() + 1;
        ks2.write((char*)&len, sizeof(int));
        ks2.write((char*)key.c_str(), len);
        ks2.write((char*)&node_offset, sizeof(int));
        ks2.write((char*)&next_elem_offset, sizeof(int));
    }

    void load(fstream& ks2){
        int len;
        ks2.read((char*)&len, sizeof(int));
        char * buf = new char[len];
        ks2.read(buf, len);
        key = buf;
        delete[] buf;
        ks2.read((char*)&node_offset, sizeof(int));
        ks2.read((char*)&next_elem_offset, sizeof(int));
    }

};

// Указатель на информацию определяет список элементов с одинаковыми значениями ключей. 
// Элемент списка имеет следующую структуру:
struct Node2{
	int release;	
    int next_node_offset = -1;	
	int info_offset;			

};

struct Table{
private:

    int msize1; 
    int msize2;	 
    int csize1;  
    bool attached;
    fstream ks1;
    fstream ks2;
    fstream nodes;
    fstream info;

    int hash_func(const string &s){
        int h=0;
        for (int i=0; i<s.size(); i++){
            h+=(int) s[i];
        }
        return h%msize2;
    }
    bool IsParent(int par);
    int find_in_KS1(int key);
    void add_in_KS1(int key, int par, int info_offset);
    int find_in_KS2(string key);
    int add_in_KS2(string key, int info_offset);

public:
    Table():msize1(0), msize2(0), csize1(0), attached(false){};
    ~Table(){
        if (attached) DetachTable();
    };

    int CreateTable(const string& name, int _msize1, int _msize2);
    int AttachTable(const string& name);
    void DetachTable();
    int add(int key1, int par, string key2, int info_num1, int info_num2, string info_str);
    Item find(int key1, string key2);
    Item find(int key1);
    Item find(string key2, int release);
    void find(const string& name, string key2);
    void find_by_par(const string& name, int par);
    void print();
    int del(int key1, string key2);
    int del(string key2, int release);
    int del(int key1);
    int del(string key2);
    bool IsEmpty(){
        if (csize1 == 0) return true;
        return false;
    };

};