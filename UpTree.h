#ifndef UPTREE_H
#define UPTREE_H

template<class T>
class UpVertex {
public:
    int key;
    int size;
    T *parent;
    T *data;
};

template<class T>
class UpTree {
public:
    UpVertex<T> **parents;
    int size;

    UpTree(int size) : size(size) {
        for (int i = 1; i < size + 1; ++i) {
            UpVertex<T> vertex = new UpVertex<T>;
            parents[i] = new UpVertex<T>(vertex);
        }
    }

    T Find(int a) {
        if (a > size || a < 1)
            return nullptr;
        int temp1 = a;
        while (parents[a]->key != a) {
            int temp2 = a;
            a = parents[a]->key;
        }
        while (parents[temp1]->key != temp1) {
            int temp2 = temp1;
            temp1 = parents[temp1]->key;
            parents[temp2]->key = a;
        }
    }

    T Union(int a, int b) {
        if (a > size || b > size || a < 1 || b < 1)
            return nullptr;
        if (parents[a]->size > parents[b]->size) {
            parents[b] = parents[a];
            parents[a]->size += parents[b]->size;
            parents[b]->parent = parents[a]->parent;
            return Find(b);
        } else
            return Union(b, a);
    }
};

#endif //UPTREE_H
