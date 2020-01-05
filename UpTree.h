#ifndef UPTREE_H
#define UPTREE_H

template<class T>
class UpVertex {
public:
    int key;
    int size;
    T *parent;
    T *data;

    UpVertex(int key, T *data) : key(key), size(1), parent(data), data(data) {}
};

template<class T>
class UpTree {
public:
    UpVertex<T> **parents;
    int size;

    UpTree(int size) : size(size) {
        parents = new UpVertex<T> *[size];
        for (int i = 1; i <= size; ++i) {
            UpVertex<T> *vertex = new UpVertex<T>(i, nullptr);
            parents[i] = vertex;
        }
    }

    int Find(int a) {
        if (a > size || a < 1)
            return -1;
        int temp1 = a;
        while (parents[a]->key != a)
            a = parents[a]->key;
        while (parents[temp1]->key != temp1) {
            int temp2 = temp1;
            temp1 = parents[temp1]->key;
            parents[temp2]->key = a;
        }
        return a;
    }

    int Union(int first, int second) {
        if (first > size || second > size || first < 1 || second < 1)
            return -1;
        if (parents[first]->size >= parents[second]->size) {
            parents[second] = parents[first];
            parents[first]->size += parents[second]->size;
            parents[second]->parent = parents[first]->parent;
        } else
            return Union(second, first);
        return first;
    }

    ~UpTree() {
        for (int i = 1; i < size; ++i)
            delete parents[i];
        delete[]parents;
    }
};

#endif //UPTREE_H
