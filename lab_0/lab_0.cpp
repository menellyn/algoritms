/* Дана последовательность из n натуральных числел. Для каждого числа исходной последовательности 
   подсчитать количество и сумму цифр в записи числа, меньших максимальной цифры
   и больших минимальной цифры. Сформировать новую последовательность, состоящую из вычисленных пар значений
   для каждого исходного числа. Упорядочить последовательность, располагая полученные пары значений
   в порядке возрастания вычисленного количества цифр в записи исходных чисел.
   Вывести исходную и полученную последовательности на экран. 
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

pair<int, int> CountAndSum(int a);


int main(){
    int n;
    int elem;
    vector<int> elem_array;
    vector<pair<int, int>> new_array;
    cout << "Enter the sequence of elements(press Ctrl-D to stop): ";
    while (cin >> elem){
        if (elem > 0){
            elem_array.push_back(elem);
        }
        else if (elem == 0){
            cout << "Don't use 0!!!" << endl;;
        }
        else{
            cout << "Don't use negative number!!!" << endl;
        }
    }
    n = elem_array.size();
    for (int i = 0; i < n; ++i){
        new_array.push_back(CountAndSum(elem_array[i]));
    }
    sort(new_array.begin(), new_array.end());
    cout << "\n";
    cout << "Initial sequence: ";
    for (int i = 0; i < n; ++i){
        cout << elem_array[i] << " ";
    } 
    cout << "\n";
    cout << "Final sequence: ";
    for (int i = 0; i < n; ++i){
        cout << "(" << new_array[i].first << " " << new_array[i].second << ") ";
    } 
    cout << "\n";
}


// The function counts the number and sum of digits of a number smaller than the maximum 
// digit and larger than the minimum digit
pair<int, int> CountAndSum(int a){
    int b = a;
    pair<int, int> count_sum(0, 0);
    int max = 0, min = 9;
    int num;
    while (a != 0){
        num = a%10;
        if (num > max){
            max = num;
        }
        if (num < min){
            min = num;
        }
        a = a/10;
    }
    while (b != 0){
        num = b%10;
        if (num < max && num > min){
            ++count_sum.first;
            count_sum.second += num;
        }
        b = b/10;
    }
    return count_sum;
}