#include <cstdio>
#include <iostream>
#include <string>
#include "Table.h"

using namespace std;

int main(){
    int size_ks1 = 100, size_ks2 = 20;
    Table my_table(size_ks1, size_ks2);

    my_table.add(1, 0, "str1", 5, 32, "info1");
    my_table.add(2, 0, "str2", 77, 2, "new_info");
    my_table.add(3, 2, "strstr", 8, 13, "new_info2");
    my_table.add(5, 3, "str1", 51, 312, "info11");
    my_table.add(23, 2, "7ds72", 12, 13, "ye7d");
    my_table.add(326, 2, "hds8wh", 33, 18, "sdh2j");
    my_table.add(88, 2, "gsayd43g", 76, 12, "sh3bdj");
    my_table.add(374, 0, "7ds72", 36, 111, "hd73hs");
    my_table.add(773, 0, "7ds72", 382, 152, "dh3jq");
    my_table.add(97, 3, "gsayd43g", 3828, 127, "dj3j2k");
    my_table.print_table();

    Table tb_key2(my_table.find_by_key2("7ds72"));
    tb_key2.print_table();

    Table parents(my_table.find_by_parent(2));
    parents.print_table();


}