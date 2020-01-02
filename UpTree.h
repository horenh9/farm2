#ifndef UPTREE_H
#define UPTREE_H

template<class T>
class UpVertex {
public:
    int key;
    T *parent;
    int size;
};

template<class T>
class UpTree {
public:
    UpVertex<T> *head;
    UpVertex<T> *parents;
    int size;

    UpTree(int size) : head(nullptr), size(size) {
        for (int i = 1; i < size + 1; ++i) {
            UpVertex<T> vertex = new UpVertex<T>;
            parents[i] = new UpVertex<T>(vertex);
        }
    }

    T Find(int a) {
        if (a > size || a < 1)
            return nullptr;
        while (parents[a] != head) {
            int temp = a;
            a = parents[a].key;
            parents[temp] = head;
        }
    }

    T Union(int a, int b) {
        if (a > size || b > size || a < 1 || b < 1)
            return nullptr;
        if (parents[a].size > parents[b].size) {
            parents[b] = parents[a];
            parents[a].size += parents[b].size;
            parents[b].parent = parents[a].parent;
            return Find(b);
        } else
            return Union(b, a);
    }
};

#endif //UPTREE_H
