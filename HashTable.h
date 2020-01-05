//
// Created by sbach on 1/3/2020.
//

#ifndef FARM2_HASHTABLE_H

#define MIN_SIZE 11
#define SHRINK_FACTOR 0.25
#define EXPAND_FACTOR 0.75

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
    int non_empty;

    DynamicHashTable(int size) : size(size) {
        table = new Node<T> *[size];
        for (int i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
    }

    virtual ~DynamicHashTable() {
        delete_lists(size, table);//deleting all linked lists first
        delete[](table);//deleting the table
    }

    void delete_lists(int size, Node<T> **curr_table) {
        for (int i = 0; i < size; ++i) {
            Node<T> *node = curr_table[i];
            while (node != nullptr) {
                Node<T> *temp = node;
                node = node->next;
                delete (temp);
            }
        }
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

    void add(int id, T *data) {
        if (add_to_table(table, id, data))
            non_empty++;
        if ((double(non_empty) / double(size)) >= EXPAND_FACTOR)
            expand();
    }

    //returns true if non_empty++
    bool add_to_table(Node<T> **curr_table, int id, T *data) {
        bool is_new_filled = false;

        Node<T> *new_node = new Node<T>(id, data);
        Node<T> *head = curr_table[h(id)];
        if (head == nullptr)
            is_new_filled = true;
        //inserting new node as the first node in the linked list
        new_node->next = head;
        if (head != nullptr)
            head->prev = new_node;
        curr_table[h(id)] = new_node;

        return is_new_filled;
    }

    void remove(int id) {

        Node<T> *node = table[h(id)];
        while (node != nullptr) {
            if (node->id == id) {
                Node<T> *temp = node;
                if (node->prev != nullptr)
                    node->prev->next = node->next;
                else {//if it's the list's head- update pointer in hash table
                    table[h(id)] = node->next;
                    if (table[h(id)] == nullptr)
                        non_empty--;
                }
                if (node->next != nullptr)
                    node->next->prev = node->prev;
            }
            node = node->next;
        }
        if (((double(non_empty) / double(size)) <= SHRINK_FACTOR) && size > MIN_SIZE)
            shrink();
    }

    void expand() {
        Node<T> **new_table = new Node<T> *[size * 2];
        size *= 2;
        non_empty = 0;
        insert_to_new_table(size / 2, new_table);
        delete_lists(size / 2, table);
        Node<T> **temp = table;
        table = new_table;
        delete[] (temp);
    }

    void shrink() {
        Node<T> **new_table = new Node<T> *[size / 2];
        size /= 2;
        non_empty = 0;
        insert_to_new_table(size, new_table);
        delete_lists(size * 2, table);
        Node<T> **temp = table;
        table = new_table;
        delete[] (temp);
    }

    void insert_to_new_table(int old_size, Node<T> **new_table) {
        for (int i = 0; i < old_size; ++i) {
            Node<T> *node = table[i];
            while (node != nullptr) {
                if (add_to_table(new_table, node->id, node->data))
                    non_empty++;
                node = node->next;
            }
        }
    }

};

#define FARM2_HASHTABLE_H

#endif //FARM2_HASHTABLE_H
