/*
Из входного потока вводится произвольное число строк, состоящих из последовательности слов, 
разделённых одним или несколькими пробелами, знаками табуляции, запятой, точкой с запятой или точкой. 
Длина каждой строки произвольна. Конец ввода определяется концом файла.
При обработке каждой строки сформировать несколько новых строк, поместив в каждую слова 
исходной строки со знаками препинания вплоть до точки. Слова в образованных строках должны быть 
разделены только одним пробелом и знакам препинания не должен предшествовать пробел. 
Полученные строки вывести на экран.
*/

#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

void NewSentense(string str);
void SplitString(string str, char sep);


int main(){
    string str;
    cout << "Enter a string, press ENTER between strings(Ctrl-D to stop): " << endl;
    while (getline(cin, str)) {
        cout << "New strings:" << endl;
        SplitString(str, '.');
    }
}


// Формирует новое предложение
void NewSentense(string str){
    string sep2{".,;"};
    string sep{" \t"};
    string sentense;
    int n = str.size();
     for (int i = 0; i < n; ++i){
        if (sep.find(str[i]) == string::npos){
            if (sep2.find(str[i]) != string::npos && sep.find(str[i-1]) != string::npos && !sentense.empty()){
                sentense.pop_back();
            }
            sentense.push_back(str[i]);
            if ((sep.find(str[i+1]) != string::npos) || 
                (sep2.find(str[i]) != string::npos && sep2.find(str[i+1]) == string::npos)){
                sentense+= ' ';
            }
        }
    }   
    cout << sentense;
}


// Разбивает строку на предложения
void SplitString(string str, char sep){
    vector<string> sentenses;
    int n = str.size();
    int start_idx = 0, end_idx = 0;
    for (int i = 0; i < n; ++i){
        if (str[i] == sep || (i + 1) == n) {
            end_idx = i;
            string tmp;
            tmp = str.substr(start_idx, end_idx - start_idx + 1);
            sentenses.push_back(tmp);
            start_idx = end_idx + 1;
        }
    }
    for (int i = 0; i < sentenses.size(); ++i){
        NewSentense(sentenses[i]);
        cout << endl;
    }
}