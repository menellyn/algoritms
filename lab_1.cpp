/*
Из входного потока вводится непрямоугольная матрица целых чисел 
[aij], i = 1, …, m, j = 1, …, n.  Значения m и ni заранее не известны и вводятся из входного потока.
Сформировать новую матрицу, поместив в ее i-ую строку только те числа из i-ой строки исходной матрицы, 
сумма цифр которых совпадает с суммой цифр последнего числа в этой же строке.
Исходную и полученную матрицы вывести в выходной поток с необходимыми комментариями.
*/

#include <cstdio>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;


void FoarmNewMatrix(vector<vector<int>> matrix);
int SumOfNumbers(int num);
void MatrixOutput(vector<vector<int>>);
int GetElem();

int main(){
    vector<vector<int>> matrix;
    vector<int> matrix_row;
    int m;
    int n_i;
    int elem;
    cout << "Enter the number of rows in the matrix: ";
    cin >> m;
    for (int i = 0; i < m; ++i){
        cout << "Enter the number of columns per row: ";
        cin >> n_i;
        matrix_row.reserve(n_i);
        cout << "Enter the row of the matrix: ";
        for (int j = 0; j < n_i; ++j){
            elem = GetElem();
            matrix_row.push_back(elem);
        }
        matrix.push_back(matrix_row);
        matrix_row.clear();
    }
    cout << "Initial matrix: " << endl;
    MatrixOutput(matrix);
    cout << "Final matrix: " << endl;
    FoarmNewMatrix(matrix);
}

int GetElem(){
    int elem;
    while (!(cin >> elem) || elem < 0){
        cout << "Wrong input. Please, try again: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
    return elem;
}


void MatrixOutput(vector<vector<int>> matrix){
    for(auto row: matrix){
        for(auto elem: row){
            cout << elem << " ";
        }
        cout << endl;
    }
}


int SumOfNumbers(int num){
    int sum = 0;
    while (num != 0){
    sum += num % 10;
    num = num / 10;
    }
    return sum;
}


void FoarmNewMatrix(vector<vector<int>> matrix){
    vector<vector<int>> new_matrix;
    vector<int> new_row;
    int sum;
    for(auto row: matrix){
        sum = SumOfNumbers(row.back());
        for(auto elem: row){
            if(SumOfNumbers(elem) == sum){
                new_row.push_back(elem);
            }
        } 
        new_matrix.push_back(new_row);
        new_row.clear();
    }
    MatrixOutput(new_matrix);
}