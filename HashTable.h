//
// Created by sbach on 1/3/2020.
//

#ifndef FARM2_HASHTABLE_H

template<class T>
class Node {
public:
    Node(int id, T *data) : id(id), data(data), prev(nullptr), next(nullptr) {}

    Node *prev;
    Node *next;
    int id;
    T *data;

};

template<class T>
class DynamicHashTable {
public:
    Node<T> **table;
    int size;

    DynamicHashTable(int size) : size(size) {
        table = new Node<T> *[size];
        for (int i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
    }

    virtual ~DynamicHashTable() {
        for (int i = 0; i < size; ++i) {//deleting all linked lists first
            Node<T> *node = table[i];
            while (node != nullptr) {
                Node<T> *temp = node;
                node = node->next;
                delete (temp);
            }
        }
        delete[](table);//deleting the table
    }

    Node<T> *find(int id) {
        Node<T> *node = table[h(id)];
        while (node != nullptr) {
            if (node->id == id)
                return node;
            node = node->next;
        }
        return nullptr;//in case node by that id was not found
    }

    int h(int item_id) {
        return item_id % size;
    }

    int add(int id, const T *data) {
        //add expanding
        //assuming we have to check first if a server with this id already exists
        if (find(id) != nullptr)//failure
            return -1;

        Node<T> *new_node = new Node<T>(id, data);
        Node<T> *head = table[h(id)];
        //inserting new node as the first node in the linked list
        new_node->next = head;
        head->prev = new_node;
        table[h(id)] = new_node;
        return 1; //success
    }

    void expand() {
        Node<T> **new_table = new Node<T> *[size * 2];
        for (int i = 0; i < size; ++i) {
            new_table[i] = table[i];
        }
        Node<T> *temp = table;
        table = new_table;
        delete[] (temp);
        size *= 2;
    }

    int remove(int id) {
        //add shrinking
        //assuming we have to check first if a server with this id exists
        if (find(id) == nullptr)//failure
            return -1;
        Node<T> *node = table[h(id)];
        while (node != nullptr) {
            if (node->id == id) {
                Node<T> *temp = node;
                if(node->prev!= nullptr){
                    node->prev->next=node->next;
                }else{//if it's the list's head- update pointer in hash table
                    table[h(id)]=node->next;
                }
                if(node->next!= nullptr){
                    node->next->prev=node->prev;
                }
                return 1;
            }
            node = node->next;
        }
        return -1;//shouldn't get here
    }

    void shrink() {
        //?
    }

};

#define FARM2_HASHTABLE_H

#endif //FARM2_HASHTABLE_H
