/*
Из входного потока вводится непрямоугольная матрица целых чисел 
[aij], i = 1, …, m, j = 1, …, n.  Значения m и ni заранее не известны и вводятся из входного потока.
Сформировать новую матрицу, поместив в ее i-ую строку только те числа из i-ой строки исходной матрицы, 
сумма цифр которых совпадает с суммой цифр последнего числа в этой же строке.
Исходную и полученную матрицы вывести в выходной поток с необходимыми комментариями.
*/

#include <cstdio>
#include <iostream>
#include <climits>
#include <new>

using namespace std;


void GetInt(int&);


struct Line{

    int n; //количество элементов в строке
    int *elem;

    Line(){
        n = 0;
        elem = nullptr;
    }

};

struct Matrix{

    int lines; //количество строк в матрице
    Line *matr;

    Matrix(){
        lines = 0;
        matr = nullptr;
    }

    Matrix(int lines){
        this->lines = lines;
        try{
            matr = new Line[lines];
        }
        catch (std::bad_alloc& ba){
            cout << "Memory Allocation is failed:" << ba.what() << endl;
        }
    }

    ~Matrix(){
        for(int i = 0; i < lines; ++i){
            delete[] matr[i].elem;
        }
        delete[] matr;
    }

    void input(){
        int n_i;
        cout << "Enter the number of rows in the matrix: ";
        GetInt(lines);
        try{
            matr = new Line[lines];
        }
        catch (std::bad_alloc& ba){
            cout << "Memory Allocation is failed:" << ba.what() << endl;
        }
        for (int i = 0; i < lines; ++i){
            cout << "Enter the number of elements in the row: ";
            GetInt(n_i);
            matr[i].n = n_i;
            try{
                matr[i].elem = new int[n_i];
            }
            catch (std::bad_alloc& ba){
                cout << "Memory Allocation is failed:" << ba.what() << endl;
            }
            cout << "Enter the elemets: " << endl;
            for (int j = 0; j < n_i; ++j){
                GetInt(matr[i].elem[j]);
            }
        }

    }

    void output(){
        for (int i = 0; i < lines; ++i){
            for (int j = 0; j< matr[i].n; ++j){
                cout.setf(ios::right);
                cout.width(10);
                cout << matr[i].elem[j];
            }
            cout << endl;
        }
    }
};


int SumOfNumbers(int num);
void FoarmNewMatrix(Matrix& matrix, Matrix& new_matrix);


int main(){
    Matrix matrix;
    matrix.input();
    cout << "Initial matrix: " << endl;
    matrix.output();
    cout << "Final matrix: " << endl;
    Matrix new_matrix(matrix.lines);
    FoarmNewMatrix(matrix, new_matrix);
    new_matrix.output();
}


void GetInt(int& elem){
    while (!(cin >> elem) || elem < 0){
        cout << "Wrong input. Please, try again: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
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

void FoarmNewMatrix(Matrix& matrix, Matrix& new_matrix){
    int sum;
    int count = 0;
    int num_of_elem;
    for(int i = 0; i < matrix.lines; ++i){
        num_of_elem = matrix.matr[i].n;
        sum = SumOfNumbers(matrix.matr[i].elem[num_of_elem - 1]);
        for(int j = 0; j < num_of_elem; ++j){
            if(SumOfNumbers(matrix.matr[i].elem[j]) == sum){
                ++count;
            }
        } 
        new_matrix.matr[i].n = count;
        try{
            new_matrix.matr[i].elem = new int[count];
        }
        catch (std::bad_alloc& ba){
            cout << "Memory Allocation is failed:" << ba.what() << endl;
        }
        count = 0;
        for(int j = 0; j < num_of_elem; ++j){
            if(SumOfNumbers(matrix.matr[i].elem[j]) == sum){
                new_matrix.matr[i].elem[count] = matrix.matr[i].elem[j];
                ++count;
            }
        } 
        count = 0;
    }
}