#ifndef FARMS_AVLTREE_H
#define FARMS_AVLTREE_H

#include "library2.h"

template<class Key, class T>
class vertex {
public:
    Key key;
    T *data;
    vertex *left_son;
    int height_left;
    vertex *right_son;
    int height_right;
    vertex *parent;

    vertex(Key key, T *data) :
            key(key), data(data),
            left_son(nullptr), right_son(nullptr),
            height_left(0), height_right(0),
            parent(nullptr) {}

    virtual ~vertex() {
        if (left_son != nullptr)
            delete left_son;
        if (right_son != nullptr)
            delete right_son;
        if (data != nullptr)
            delete data;
    }
};

template<class Key, class T>
vertex<Key, T> *get_vertex_by_ID(vertex<Key, T> *v, const Key id) {
    if (v == nullptr)
        return nullptr;
    if (v->key == id)
        return v;
    else if (v->key < id) {
        if (v->right_son != nullptr)
            return get_vertex_by_ID(v->right_son, id);
        else return nullptr;
    } else if (v->left_son != nullptr)
        return get_vertex_by_ID(v->left_son, id);
    else return nullptr;

}

template<class Key, class T>
class AVLtree {
public:
    vertex<Key, T> *head;

    AVLtree() {
        head = nullptr;
    }

    StatusType add_vertex(vertex<Key, T> *v) {
        if (head == nullptr) {
            head = v;
            return SUCCESS;
        }
        vertex<Key, T> *new_parent = find_a_parent(v, head);
        if (new_parent == nullptr)
            return FAILURE;
        if (v->key > new_parent->key)
            new_parent->right_son = v;
        else
            new_parent->left_son = v;
        v->parent = new_parent;
        v->height_right = 0;
        v->height_left = 0;
        update_path_height(new_parent);
        //now check if balance factor is ok, and handle it if not
        fix_balance_factor(v);
        update_path_height(v);
        return SUCCESS;
    }

    vertex<Key, T> *find_a_parent(const vertex<Key, T> *v, vertex<Key, T> *r) const {
        if (r == nullptr)
            return nullptr;
        if (r->key < v->key) {
            if (r->right_son == nullptr)//stopping cond 1
                return r;
            return find_a_parent(v, r->right_son);
        } else if (r->key > v->key) {
            if (r->left_son == nullptr)//stopping cond 2
                return r;
            return find_a_parent(v, r->left_son);
        } else
            return nullptr;//stopping cond 3
    }

    vertex<Key, T> *getLeftest(vertex<Key, T> *v) {
        if (v->left_son == nullptr)
            return v;
        return getLeftest(v->left_son);
    }

    void fertile_vertex(vertex<Key, T> *v) {//we get a vertex to delete - that has 2 sons

        vertex<Key, T> *parent = v->parent;
        vertex<Key, T> *replace = getLeftest(v->right_son);
        vertex<Key, T> *replace_parent = replace->parent;
        vertex<Key, T> *vr = v->right_son;
        vertex<Key, T> *vl = v->left_son;

        //v's parent becoming replace's parent
        v->parent = replace_parent;
        replace->parent = parent;
        if (parent != nullptr) // v is not the root
            if (parent->key > v->key) //v is a left son
                parent->left_son = replace;
            else //v is a right son
                parent->right_son = replace;
        else //v is the root
            head = replace;

        //v's children
        v->left_son = nullptr;
        vl->parent = replace;
        replace->left_son = vl;
        v->right_son = replace->right_son;
        if (replace->right_son != nullptr)
            replace->right_son->parent = v;
        if (!(vr->key == replace->key)) {
            replace->right_son = vr;
            vr->parent = replace;
            v->parent = replace_parent;
        } else {
            replace->right_son = v;
            v->parent = replace;
        }

        update_path_height(replace_parent);
    }

    void replace_with_son(vertex<Key, T> *v, vertex<Key, T> *w) {
        if (v->parent == nullptr) {
            head = w;
            if (w != nullptr)
                w->parent = nullptr;
        } else {
            if (v->parent->right_son == v) {
                v->parent->right_son = w;
                if (w != nullptr)
                    w->parent = v->parent;
            } else {
                v->parent->left_son = w;
                if (w != nullptr)
                    w->parent = v->parent;
            }
        }
        v->right_son = nullptr;
        v->left_son = nullptr;
        update_path_height(v->parent);
        v->parent = nullptr;

    }

    void remove_not_fertile(vertex<Key, T> *v) {
        //has no children
        if (v->right_son == nullptr && v->left_son == nullptr)
            replace_with_son(v, nullptr);
        //if he only has left son
        if (v->left_son != nullptr && v->right_son == nullptr)
            replace_with_son(v, v->left_son);
        //if he only has right son
        if (v->left_son == nullptr && v->right_son != nullptr)
            replace_with_son(v, v->right_son);
    }

    StatusType remove_vertex(Key id) {
        //check if there is a vertex with that id
        vertex<Key, T> *v = get_vertex_by_ID(head, id);
        if (v == nullptr)
            return FAILURE;
        vertex<Key, T> *parent = v->parent;

        if (v->left_son != nullptr && v->right_son != nullptr)
            fertile_vertex(v);
        remove_not_fertile(v);
        if (parent != nullptr)
            fix_balance_factor(parent);
        return SUCCESS;
    }

    int calc_bf(vertex<Key, T> *v) {
        if (v == nullptr)
            return 0;
        if (v->left_son == nullptr)
            if (v->right_son == nullptr)
                return 0;
            else
                return v->height_right;
        else
            return v->height_left - v->height_right;
    }

    void fix_balance_factor(vertex<Key, T> *v) {
        if (calc_bf(v) == 2) {
            if (calc_bf(v->left_son) == -1)
                LR_rotate(v);
            if (calc_bf(v->left_son) >= 0)
                LL_rotate(v);
        }
        if (calc_bf(v) == -2) {
            if (calc_bf(v->right_son) == 1)
                RL_rotate(v);
            if (calc_bf(v->left_son) <= 0)
                RR_rotate(v);
        }

        if (v->parent != nullptr) {
            if (v->left_son != nullptr)
                v->height_left = height(v->left_son);
            else
                v->height_left = 0;
            if (v->right_son != nullptr)
                v->height_right = height(v->right_son);
            else
                v->height_right = 0;
            if (v->parent->right_son != nullptr)
                v->parent->height_right = height(v->parent->right_son);
            else
                v->parent->height_right = 0;
            if (v->parent->left_son)
                v->parent->height_left = height(v->parent->left_son);
            else
                v->parent->height_left = 0;
            fix_balance_factor(v->parent);
        }
    }

    void LL_rotate(vertex<Key, T> *v) {
        vertex<Key, T> *temp = v->left_son;
        if (temp != nullptr) {
            //setting v's parent as v's left son's parent
            temp->parent = v->parent;
            if (v->parent != nullptr)
                if (v->key > v->parent->key)
                    v->parent->right_son = temp;
                else
                    v->parent->left_son = temp;
            else
                head = temp;

            //setting v's left son's right son as v's left son
            v->left_son = temp->right_son;
            if (v->left_son != nullptr)
                v->left_son->parent = v;

            //setting v's left son as v's parent
            v->parent = temp;
            temp->right_son = v;

            //updating heights
            if (v->left_son != nullptr)
                v->height_left = height(v->left_son);
            else
                v->height_left = 0;
            temp->height_right = height(temp->right_son);
        }
    }

    void RR_rotate(vertex<Key, T> *v) {
        vertex<Key, T> *temp = v->right_son;
        //setting v's parent as v's right son's parent
        if (temp != nullptr) {
            temp->parent = v->parent;
            if (v->parent != nullptr)
                if (v->key > v->parent->key)
                    v->parent->right_son = temp;
                else
                    v->parent->left_son = temp;
            else
                head = temp;

            //setting v's right son's left son as v's right son
            v->right_son = temp->left_son;
            if (v->right_son != nullptr)
                v->right_son->parent = v;

            //setting v's right son as v's parent
            v->parent = temp;
            temp->left_son = v;

            //updating heights
            if (v->right_son != nullptr)
                v->height_right = height(v->right_son);
            else
                v->height_right = 0;
            temp->height_left = height(temp->left_son);
        }
    }

    void LR_rotate(vertex<Key, T> *v) {
        //giving names to 3 critical vertices:
        vertex<Key, T> *a = v->left_son;
        vertex<Key, T> *b = a->right_son;
        vertex<Key, T> *c = v;

        //setting c's parent as b's parent
        b->parent = c->parent;
        if (c->parent != nullptr)
            if (c->key > c->parent->key)
                c->parent->right_son = b;
            else
                v->parent->left_son = b;
        else
            head = b;

        //b's left son becoming a's right son
        a->right_son = b->left_son;
        if (b->left_son != nullptr)
            a->right_son->parent = a;

        //b's right son becoming c's left son
        c->left_son = b->right_son;
        if (b->right_son != nullptr)
            c->left_son->parent = c;

        //a becoming b's left son
        b->left_son = a;
        a->parent = b;

        //c becoming b's right son
        b->right_son = c;
        c->parent = b;

        //updating heights:
        if (c->left_son != nullptr)
            c->height_left = height(c->left_son);
        else
            c->height_left = 0;
        if (a->right_son != nullptr)
            a->height_right = height(a->right_son);
        else
            a->height_right = 0;
    }

    void RL_rotate(vertex<Key, T> *v) {
        //giving names to 3 critical vertices:
        vertex<Key, T> *a = v->right_son;
        vertex<Key, T> *b = a->left_son;
        vertex<Key, T> *c = v;

        //setting c's parent as b's parent
        b->parent = c->parent;
        if (c->parent != nullptr)
            if (c->key > c->parent->key)
                c->parent->right_son = b;
            else
                v->parent->left_son = b;
        else
            head = b;

        //b's left son becoming c's right son
        c->right_son = b->left_son;
        if (b->left_son != nullptr)
            c->right_son->parent = c;

        //b's right son becoming a's left son
        a->left_son = b->right_son;
        if (b->right_son != nullptr)
            a->left_son->parent = a;

        //a becoming b's right son
        b->right_son = a;
        a->parent = b;

        //c becoming b's left son
        b->left_son = c;
        c->parent = b;

        //updating heights:
        if (c->right_son != nullptr)
            c->height_right = height(c->right_son);
        else
            c->height_right = 0;
        if (a->left_son != nullptr)
            a->height_left = height(a->left_son);
        else
            a->height_left = 0;
    }

    ~AVLtree() {
        delete head;
    }
};

static int max(int a, int b) {
    return a > b ? a : b;
}

template<class Key, class T>
static int height(vertex<Key, T> *v) {
    return 1 + max(v->height_left, v->height_right);
}

template<class Key, class T>
void update_path_height(vertex<Key, T> *v) {
    if (v == nullptr)
        return;
    if (v->left_son != nullptr)
        v->height_left = height(v->left_son);
    else
        v->height_left = 0;
    if (v->right_son != nullptr)
        v->height_right = height(v->right_son);
    else
        v->height_right = 0;
    update_path_height(v->parent);
}

#endif //FARMS_AVLTREE_H
