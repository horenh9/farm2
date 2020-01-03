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
        for (int i = 1; i <= size ; ++i) {
            UpVertex<T> *vertex = new UpVertex<T>;
            parents[i] = vertex;
        }
    }

    int Find(int a) {
        if (a > size || a < 1)
            return -1;
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
        return a;
    }

    T Union(int a, int b) {
        if (a > size || b > size || a < 1 || b < 1)
            return -1;
        int first = Find(a), second = Find(b);
        if (parents[first]->size > parents[second]->size) {
            parents[second] = parents[first];
            parents[first]->size += parents[second]->size;
            parents[second]->parent = parents[first]->parent;
        } else
            return Union(second, first);
        return first;
    }
};

#endif //UPTREE_H
