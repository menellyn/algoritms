#include <cstdio>
#include <iostream>
#include <string>
#include "stack.h"

using namespace std;

bool IsOperator(char elem);
int CountOperators(string& str);
void PreToInf(string& str);


int main(){
    string str;
    cout << "Enter the formula in prefix notations (Ctrl-D to stop): " << endl;
    while(getline(cin, str)){
        cout << "Prefix formula: " << str << endl;
        cout << "Infix formula: ";
        PreToInf(str);
    }
}

void PreToInf(string& str){
    int str_size = str.size();
    int size = str.size() + CountOperators(str)*2;
    Stack inf_foarm(size);
    int count1 = 0, count2 = 0;
    for (int i = str_size - 1; i >= 0; i--){
        if (!IsOperator(str[i])){
            inf_foarm.push(str[i]);
        } 
        else{
            string elem1 = "";
            string elem2 = "";
            if (inf_foarm.back() != '('){
                elem1.push_back(inf_foarm.pop());
                if (inf_foarm.back() != '(') elem2.push_back(inf_foarm.pop());
                else{
                    count1++;
                    while (count1 != count2){
                        elem2.push_back(inf_foarm.pop());
                        if (inf_foarm.back() == ')') count2++;
                        if (inf_foarm.back() == '(') count1++;
                    }
                    elem2.push_back(inf_foarm.pop());
                    count1 = 0;
                    count2 = 0;
                }
            }
            else{
                count1++;
                while (count1 != count2){
                    elem1.push_back(inf_foarm.pop());
                    if (inf_foarm.back() == ')') count2++;
                    if (inf_foarm.back() == '(') count1++;
                }
                elem1.push_back(inf_foarm.pop());
                count1 = 0;
                count2 = 0;
                if (inf_foarm.back() != '(') elem2.push_back(inf_foarm.pop());
                else{
                    count1++;
                    while (count1 != count2){
                        elem2.push_back(inf_foarm.pop());
                        if (inf_foarm.back() == ')') count2++;
                        if (inf_foarm.back() == '(') count1++;
                    }
                    elem2.push_back(inf_foarm.pop());
                    count1 = 0;
                    count2 = 0;
                }
            }
            string tmp = '(' + elem1 + str[i] + elem2 + ')';
            for (int j = tmp.size() - 1; j >= 0; --j){
                    inf_foarm.push(tmp[j]);
            }
        }
    }
    inf_foarm.print();
}

bool IsOperator(char elem){
    string oper{"+-*/"};
    if (oper.find(elem) != string::npos) return true;
    else return false;
}

int CountOperators(string& str){
    int count = 0;
    for (int i = 0; i < str.size(); ++i){
        if (IsOperator(str[i])) count++;
    }
    return count;
}