#include "Table.h"

int Table::CreateTable(const string& name, int _msize1, int _msize2){
    if(attached){
        DetachTable();
    }

    filesystem::path directory_path(name);
    if (filesystem::exists(directory_path)) return -1;
    if (_msize1 < 1) return -2;
    if (_msize2 < 1) return -3;

    filesystem::create_directory(directory_path);

    string KeySpace1_name = name + "/ks1.bin";
    string KeySpace2_name = name + "/ks2.bin";
    string nodes_name = name + "/nodes.bin";
    string info_name = name + "/info.bin";

    ks1.open(KeySpace1_name, fstream::binary | fstream::out | fstream::in | fstream::trunc);
    ks2.open(KeySpace2_name, fstream::binary | fstream::out | fstream::in | fstream::trunc);
    nodes.open(nodes_name, fstream::binary | fstream::out | fstream::in | fstream::trunc);
    info.open(info_name, fstream::binary | fstream::out | fstream::in | fstream::trunc);

    msize1 = _msize1;
    msize2 = _msize2;
    KeySpace2 empty;
    KeySpace1 empty_ks1;
    ks1.write((char*)&msize1, sizeof(msize1));
    ks2.write((char*)&msize2, sizeof(msize2));
    attached = true;

    for (int i = 0; i < msize2; ++i){
        empty.save(ks2);
    }

    for (int i = 0; i < msize1; ++i){
        empty_ks1.save(ks1);
    }


    return 0;
}

int Table::AttachTable(const string &name)
{
    if(attached){
        DetachTable();
    }

    filesystem::path directory_path(name);
    if (!filesystem::exists(directory_path)) return -1;

    string KeySpace1_name = name + "/ks1.bin";
    string KeySpace2_name = name + "/ks2.bin";
    string nodes_name = name + "/nodes.bin";
    string info_name = name + "/info.bin";

    ks1.open(KeySpace1_name, fstream::binary | fstream::out | fstream::in);
    ks2.open(KeySpace2_name, fstream::binary | fstream::out | fstream::in);
    nodes.open(nodes_name, fstream::binary | fstream::out | fstream::in);
    info.open(info_name, fstream::binary | fstream::out | fstream::in);

    ks1.seekp(0, ios::beg);
    ks1.read((char*)&msize1, sizeof(int));
    ks2.seekp(0, ios::beg);
    ks2.read((char*)&msize2, sizeof(int));

    KeySpace1 ks1_elem;
    while(true){
        ks1.read((char*)&ks1_elem, sizeof(KeySpace1));
        if (ks1.eof() || ks1_elem.key == 0) break;
        csize1++;
    }
    attached = true;
    return 0;
}

void Table::DetachTable()
{
    csize1 = 0;
    msize1 = 0;
    msize2 = 0;
    attached = false;
    ks1.close();
    ks2.close();
    nodes.close();
    info.close();
}

bool Table::IsParent(int par)
{
    int element_offset = sizeof(int);
    ks1.seekp(element_offset, ios::beg);
    KeySpace1 element;
    for (int i = 0; i < csize1; ++i){
        ks1.read((char*)&element, sizeof(KeySpace1));
        if (element.par == par) return true;
    }
    return false;
}

int Table::find_in_KS1(int key)
{
    if (csize1 == 0) return -1;
    int element_offset = sizeof(int);
    ks1.seekp(element_offset, ios::beg);
    KeySpace1 element;
    bool IsTarget;
    for (int i = 0; i < csize1; ++i){
        element_offset = ks1.tellp();
        ks1.read((char*)&element, sizeof(KeySpace1));
        IsTarget = (element.key == key);
        if (IsTarget) return element_offset;
    }
    return -1;
}

void Table::add_in_KS1(int key, int par, int info_offset)
{
    KeySpace1 element, prev_element;
    element.key = key;
    element.par = par;
    element.info_offset = info_offset;
    int element_offset = sizeof(int) + csize1*sizeof(KeySpace1);;
    ks1.seekp(element_offset, ios::beg);

    if (csize1 == 0){
        element.save(ks1);
        ++csize1;
        return;
    }

    int prev_elem_offset = element_offset - sizeof(KeySpace1);
    ks1.seekp(prev_elem_offset, ios::beg);
    prev_element.load(ks1);

    while (prev_element.par > par){
        prev_element.save(ks1);
        prev_elem_offset -= sizeof(KeySpace1);
        ks1.seekp(prev_elem_offset, ios::beg);
        prev_element.load(ks1);
    }

    element_offset = ks1.tellp();
    ks1.seekp(element_offset, ios::beg);
    element.save(ks1);
    csize1++;
}

int Table::find_in_KS2(string key){

    int init_pos = hash_func(key);
    KeySpace2 elem;
    int offset = sizeof(int);
    ks2.seekp(offset, ios::beg);
    for(int i = 0; i < init_pos; ++i){
        elem.load(ks2);
    }

    offset = ks2.tellp();
    elem.load(ks2);
    offset = elem.next_elem_offset;

    if(offset == -1) return -1;

    ks2.seekp(offset, ios::beg);
    elem.load(ks2);

    while (offset != -1 && elem.node_offset != -1 && elem.key != key){
        offset = elem.next_elem_offset;
        if (offset != -1){
            ks2.seekp(offset, ios::beg);
            elem.load(ks2);
        }
    }
    if (elem.key == key) return offset;
    return -1;
}

int Table::add_in_KS2(string key, int info_offset)
{
    int init_pos = hash_func(key);
    int init_offset = sizeof(int);
    KeySpace2 init_elem;

    ks2.seekp(init_offset, ios::beg);
    for(int i = 0; i < init_pos; ++i){
        init_elem.load(ks2);
    }

    init_offset = ks2.tellp();
    init_elem.load(ks2);

    int elem_offset = find_in_KS2(key);
    KeySpace2 elem;

    Node2 new_node;
    new_node.info_offset = info_offset;
    nodes.seekp(0, ios::end);
    int new_node_offset = nodes.tellp();


    if (elem_offset == -1){
        elem.next_elem_offset = init_elem.next_elem_offset;

        ks2.seekp(0, ios::end);
        elem_offset = ks2.tellp();
        init_elem.next_elem_offset = elem_offset;

        ks2.seekp(init_offset, ios::beg);
        init_elem.save(ks2);

        elem.key = key;
        elem.node_offset = new_node_offset;

        new_node.next_node_offset = -1;
        new_node.release = 1;
    }
    else{
        ks2.seekp(elem_offset, ios::beg);
        elem.load(ks2);

        Node2 prev_node;

        nodes.seekp(elem.node_offset, ios::beg);
        nodes.read((char*)&prev_node, sizeof(Node2));

        new_node.next_node_offset = elem.node_offset;
        new_node.release = prev_node.release + 1;
        elem.node_offset = new_node_offset;
    }

    ks2.seekp(elem_offset, ios::beg);
    elem.save(ks2);

    nodes.seekp(0, ios::end);
    nodes.write((char*)&new_node, sizeof(Node2));

    return new_node_offset;
}

int Table::add(int key1, int par, string key2, int info_num1, int info_num2, string info_str)
{
    if (!attached) return -1;
    if (csize1 == msize1) return -2;
    if (key1 == 0) return -3;
    if (find_in_KS1(par) == -1 && par!=0) return -4;
    if (find_in_KS1(key1) != -1) return -5;

    info.seekp(0, ios::end);
    int new_info_offset = info.tellp();

    Item new_item;
    new_item.key1 = key1;
    new_item.key2 = key2;
    new_item.par = par;
    new_item.num1 = info_num1;
    new_item.num2 = info_num2;
    new_item.str = info_str;


    add_in_KS1(key1, par, new_info_offset);
    int node_offset = add_in_KS2(key2, new_info_offset);
    Node2 node;
    nodes.seekp(node_offset, ios::beg);
    nodes.read((char*)&node, sizeof(Node2));

    new_item.release = node.release;

    info.seekp(0, ios::end);
    new_item.save(info);

    return 0;
}

Item Table::find(int key1, string key2)
{
    Item target;
    target.key1 = 0;
    if (!attached) return target;
    if (key1 == 0 || find_in_KS1(key1) == -1 || find_in_KS2(key2) == -1) return target;

    int ks1_offset = find_in_KS1(key1);
    KeySpace1 elem_ks1;

    ks1.seekp(ks1_offset, ios::beg);
    ks1.read((char*)&elem_ks1, sizeof(KeySpace1));

    int info_offset = elem_ks1.info_offset;
    Item elem_info;

    info.seekp(info_offset, ios::beg);
    elem_info.load(info);

    if (elem_info.key2 != key2) return target;

    target = elem_info;

    return target;
}

Item Table::find(int key1)
{
    Item target;
    target.key1 = 0;

    if (!attached) return target;
    if (key1 == 0 || find_in_KS1(key1) == -1) return target;

    int ks1_offset = find_in_KS1(key1);
    KeySpace1 elem_ks1;

    ks1.seekp(ks1_offset, ios::beg);
    ks1.read((char*)&elem_ks1, sizeof(KeySpace1));

    int info_offset = elem_ks1.info_offset;

    info.seekp(info_offset, ios::beg);
    target.load(info);

    return target;
}

Item Table::find(string key2, int release){
    Item target;
    target.key1 = 0;

    if (!attached) return target;

    int ks2_offset = find_in_KS2(key2);
    if (ks2_offset == -1) return target;

    KeySpace2 elem_ks2;

    ks2.seekp(ks2_offset, ios::beg);
    elem_ks2.load(ks2);

    int node_offset = elem_ks2.node_offset;

    Node2 target_node;

    nodes.seekp(node_offset, ios::beg);
    nodes.read((char*)&target_node, sizeof(Node2));

    while (node_offset != -1 && target_node.release != release){
        node_offset = target_node.next_node_offset;
        if (node_offset != -1){
            nodes.seekp(node_offset, ios::beg);
            nodes.read((char*)&target_node, sizeof(Node2));
        }
    }

    if (node_offset == -1) return target;

    int info_offset = target_node.info_offset;
    info.seekp(info_offset, ios::beg);
    target.load(info);

    return target;

}

void Table::find(const string& name, string key2){
    if (!attached) return;

    Table out;
    out.CreateTable(name , msize1, msize2);

    int ks2_offset = find_in_KS2(key2);

    if (ks2_offset ==-1) return;

    KeySpace2 elem_ks2;

    ks2.seekp(ks2_offset, ios::beg);
    elem_ks2.load(ks2);

    int node_offset = elem_ks2.node_offset;

    Node2 target_node;

    nodes.seekp(node_offset, ios::beg);
    nodes.read((char*)&target_node, sizeof(Node2));

    int info_offset, ks1_offset;
    KeySpace1 target_ks1;
    Item target_info;
    int key1;

    while (node_offset != -1){
        info_offset = target_node.info_offset;
        info.seekp(info_offset, ios::beg);
        target_info.load(info);

        key1 = target_info.key1;
        ks1_offset = find_in_KS1(key1);

        ks1.seekp(ks1_offset, ios::beg);
        ks1.read((char*)&target_ks1, sizeof(KeySpace1));

        out.add(key1, 0, key2, target_info.num1, target_info.num2, target_info.str);

        node_offset = target_node.next_node_offset;
        if (node_offset != -1){
            nodes.seekp(node_offset, ios::beg);
            nodes.read((char*)&target_node, sizeof(Node2));
        }
    }

}

void Table::find_by_par(const string& name, int par)
{
    if (!attached) return;
    Table out;
    out.CreateTable(name, msize1, msize2);

    if (!IsParent(par)) return;
    KeySpace1 ks1_elem;
    int ks1_offset = sizeof(int);

    ks1.seekp(ks1_offset, ios::beg);

    int info_offset;
    Item info_elem;

    for (int i = 0; i < csize1; ++i){
        ks1.read((char*)&ks1_elem, sizeof(KeySpace1));
        if (ks1_elem.par == par){
            info_offset = ks1_elem.info_offset;
            info.seekp(info_offset, ios::beg);
            info_elem.load(info);

            out.add(info_elem.key1, 0, info_elem.key2, info_elem.num1, info_elem.num2, info_elem.str);
        }
    }
}

void Table::print()
{
    if (!attached) return;
    if (csize1 == 0) {
        return;
    }

    cout.width(65);
    cout << right << "--------------------------------------------------------";
    cout << endl;
    cout.width(10);
    cout << right << "|" ;
    cout.width(10);
    cout << left << "key1" ;
    cout <<  "|" ;
    cout.width(10);
    cout << left << "parent" ;
    cout <<  "|" ;
    cout.width(10);
    cout << left << "key2";
    cout <<  "|";
    cout.width(10);
    cout << left << "ver" ;
    cout <<  "|" ;
    cout.width(10);
    cout << left << "info";
    cout << "|" << endl;
    cout.width(65);
    cout << right;
    cout <<  "--------------------------------------------------------";
    cout << endl;

    int info_offset;
    Item info_elem;
    int ks1_offset = sizeof(int);
    KeySpace1 ks1_elem;
    ks1.seekp(ks1_offset, ios::beg);

    for(int i = 0; i < csize1; ++i){
        ks1.read((char*)&ks1_elem, sizeof(KeySpace1));
        info_offset = ks1_elem.info_offset;
        info.seekp(info_offset, ios::beg);
        info_elem.load(info);

        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << left << info_elem.key1;
        cout << "|" ;
        cout.width(10);
        cout << left << info_elem.par;
        cout <<  "|" ;
        cout.width(10);
        cout << left << info_elem.key2;
        cout << "|" ;
        cout.width(10);
        cout << left << info_elem.release;
        cout <<  "|" ;
        cout.width(10);
        cout << left << info_elem.num1;
        cout <<  "|" << endl;
        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << left << " ";
        cout << "|" ;
        cout.width(10);
        cout << left << " " ;
        cout <<  "|" ;
        cout.width(10);
        cout << left << " ";
        cout << "|" ;
        cout.width(10);
        cout << left << " ";
        cout <<  "|" ;
        cout.width(10);
        cout << left << info_elem.num2;
        cout <<  "|" << endl;
        cout.width(10);
        cout << right << "|" ;
        cout.width(10);
        cout << left << " ";
        cout << "|" ;
        cout.width(10);
        cout << left << " " ;
        cout <<  "|" ;
        cout.width(10);
        cout << left << " ";
        cout << "|" ;
        cout.width(10);
        cout << left << " ";
        cout <<  "|" ;
        cout.width(10);
        cout << left << info_elem.str;
        cout <<  "|" << endl;
        cout.width(65);
        cout << right;
        cout << "--------------------------------------------------------";
        cout << endl;
    }

}

int Table::del(int key1, string key2)
{
    if (!attached) return -1;
    if (find_in_KS1(key1) == -1 || find_in_KS2(key2) == -1) return -2;
    if (IsParent(key1)) return -3;

    int ks1_offset = find_in_KS1(key1);
    KeySpace1 ks1_elem;
    int info_offset;
    Item info_elem;

    ks1.seekp(ks1_offset, ios::beg);
    ks1.read((char*)&ks1_elem, sizeof(KeySpace1));

     
    info_offset = ks1_elem.info_offset;
    info.seekp(info_offset, ios::beg);
    info_elem.load(info);

    if (info_elem.key2 != key2) return -4;

    int next_ks1_offset = ks1_offset + sizeof(KeySpace1);
    KeySpace1 next_ks1_elem;

    ks1.seekp(next_ks1_offset, ios::beg);
    ks1.read((char*)&next_ks1_elem, sizeof(KeySpace1));

    while(next_ks1_elem.key != 0){
        ks1.seekp(ks1_offset, ios::beg);
        next_ks1_elem.save(ks1);
        ks1_offset = ks1.tellp();
        next_ks1_offset = ks1_offset + sizeof(KeySpace1);
        ks1.seekp(next_ks1_offset, ios::beg);
        next_ks1_elem.load(ks1);
        if (ks1.eof()) break;
    }

    ks1.seekp(ks1_offset, ios::beg);
    next_ks1_elem.key= 0;
    ks1.write((char*)&next_ks1_elem, sizeof(KeySpace1));
    csize1--;

    int ks2_offset = find_in_KS2(key2);
    KeySpace2 ks2_elem;

    ks2.seekp(ks2_offset, ios::beg);
    ks2_elem.load(ks2);

    int init_pos = hash_func(key2);
    int prev_ks2_offset = sizeof(int);
    KeySpace2 prev_ks2_elem;

    ks2.seekp(prev_ks2_offset, ios::beg);
    for(int i = 0; i < init_pos; ++i){
        prev_ks2_elem.load(ks2);
    }

    prev_ks2_offset = ks2.tellp();
    prev_ks2_elem.load(ks2);

    int init_offset = prev_ks2_offset;
    KeySpace2 init_ks2_elem = prev_ks2_elem;

    prev_ks2_offset = prev_ks2_elem.next_elem_offset;
    ks2.seekp(prev_ks2_offset, ios::beg);
    prev_ks2_elem.load(ks2);

    if (prev_ks2_elem.key == key2) prev_ks2_offset = -1;
    else{
        bool IsPrevious = (prev_ks2_elem.next_elem_offset == ks2_offset);

        while (!IsPrevious){
            prev_ks2_offset = prev_ks2_elem.next_elem_offset;
            if (prev_ks2_offset != -1){
                ks2.seekp(prev_ks2_offset, ios::beg);
                prev_ks2_elem.load(ks2);
                IsPrevious = (prev_ks2_elem.next_elem_offset == ks2_offset);
            }
        }
    }

    Node2 node;
    int node_offset = ks2_elem.node_offset;
    int prev_node_offset = -1;

    nodes.seekp(node_offset, ios::beg);
    nodes.read((char*)&node, sizeof(Node2));

    while (node.release != info_elem.release){
        prev_node_offset = node_offset;
        node_offset = node.next_node_offset;
        nodes.seekp(node_offset, ios::beg);
        nodes.read((char*)&node, sizeof(Node2));
    }

    if (prev_node_offset == -1 && node.next_node_offset == -1){
        if (prev_ks2_offset == -1 && ks2_elem.next_elem_offset == -1){
            init_ks2_elem.next_elem_offset = -1;
            ks2.seekp(init_offset, ios::beg);
            init_ks2_elem.save(ks2);
        }
        else if (prev_ks2_offset == -1 && ks2_elem.next_elem_offset != -1){
            init_ks2_elem.next_elem_offset = ks2_elem.next_elem_offset;
            ks2.seekp(init_offset, ios::beg);
            init_ks2_elem.save(ks2);
        }
        else if (prev_ks2_offset != -1){
            prev_ks2_elem.next_elem_offset = ks2_elem.next_elem_offset;
            ks2.seekp(prev_ks2_offset, ios::beg);
            prev_ks2_elem.save(ks2);
        }
    }
    else if (prev_node_offset == -1 && node.next_node_offset != -1){
        ks2_elem.node_offset = node.next_node_offset;
        ks2.seekp(ks2_offset, ios::beg);
        ks2_elem.save(ks2);
    }
    else if (prev_node_offset != -1){
        Node2 prev_node;
        nodes.seekp(prev_node_offset, ios::beg);
        nodes.read((char*)&prev_node, sizeof(Node2));
        prev_node.next_node_offset = node.next_node_offset;
        nodes.seekp(prev_node_offset, ios::beg);
        nodes.write((char*)&prev_node, sizeof(Node2));
    }

    return 0;
}

int Table::del(int key1)
{
    int ks1_offset = find_in_KS1(key1);

    if (ks1_offset == -1) return -1;

    KeySpace1 ks1_elem;
    ks1.seekp(ks1_offset, ios::beg);
    ks1.read((char*)&ks1_elem, sizeof(KeySpace1));

    int info_offset = ks1_elem.info_offset;
    Item info_elem;
    info.seekp(info_offset, ios::beg);
    info_elem.load(info);

    del(key1, info_elem.key2);
    return 0;
}

int Table::del(string key2, int release){
    Item info_elem = find(key2, release);
    if (info_elem.key1 == 0) return -1;
    int key1 = info_elem.key1;
    del(key1, key2);
    return 0;
}

int Table::del(string key2){
    int ks2_offset = find_in_KS2(key2);
    if (ks2_offset == -1) return -1;

    KeySpace2 ks2_elem;
    ks2.seekp(ks2_offset, ios::beg);
    ks2_elem.load(ks2);

    int node_offset = ks2_elem.node_offset;
    Node2 node;
    nodes.seekp(node_offset, ios::beg);
    nodes.read((char*)&node, sizeof(Node2));

    int info_offset;
    Item info_elem;

    while(node.next_node_offset != -1){
        info_offset = node.info_offset;

        info.seekp(info_offset, ios::beg);
        info_elem.load(info);

        del(info_elem.key1, key2);

        node_offset = ks2_elem.node_offset;
        nodes.seekp(node_offset, ios::beg);
        nodes.read((char*)&node, sizeof(Node2));
    }

    info_offset = node.info_offset;

    info.seekp(info_offset, ios::beg);
    info_elem.load(info);

    del(info_elem.key1, key2);

    return 0;

}