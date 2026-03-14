// Priority Queue: Meldable Heap
//
// TWIST: Merge Counter — tracks actual merge operations to empirically verify O(log n) behavior of the randomized merge

#pragma once
#include "../include/queue.h"
#include <cstdlib>
#include <cassert>
#include <stdexcept>

template <typename T>
class MeldableHeap : public Queue<T>
{
private:
    struct Node
    {
        T x;
        Node *left, *right, *parent;
        Node(const T &val) : x(val), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node *r;
    size_t n;
    size_t merge_count;

    Node *merge(Node *h1, Node *h2)
    {
        if (h1 == nullptr)
            return h2;
        if (h2 == nullptr)
            return h1;

        if (h1->x > h2->x)
        {
            Node *temp = h1;
            h1 = h2;
            h2 = temp;
        }

        merge_count++;

        if (rand() % 2)
        {
            h1->left = merge(h1->left, h2);
            if (h1->left != nullptr)
                h1->left->parent = h1;
        }
        else
        {
            h1->right = merge(h1->right, h2);
            if (h1->right != nullptr)
                h1->right->parent = h1;
        }
        return h1;
    }

    void destroyNode(Node *u)
    {
        if (u == nullptr)
            return;
        destroyNode(u->left);
        destroyNode(u->right);
        delete u;
    }

public:
    MeldableHeap() : r(nullptr), n(0), merge_count(0) {}

    ~MeldableHeap()
    {
        destroyNode(r);
    }

    bool add(T x) override
    {
        Node *u = new Node(x);
        r = merge(u, r);
        if (r != nullptr)
            r->parent = nullptr;
        n++;
        return true;
    }

    T remove() override
    {
        assert(n > 0);
        T x = r->x;
        Node *temp = r;
        r = merge(r->left, r->right);
        delete temp;
        if (r != nullptr)
            r->parent = nullptr;
        n--;
        return x;
    }

    void remove(Node *u)
    {
        Node *merged = merge(u->left, u->right);
        if (merged != nullptr)
            merged->parent = u->parent;

        if (u->parent == nullptr)
            r = merged;
        else if (u->parent->left == u)
            u->parent->left = merged;
        else
            u->parent->right = merged;

        delete u;
        n--;
    }

    void absorb(MeldableHeap<T> &other)
    {
        r = merge(r, other.r);
        if (r != nullptr)
            r->parent = nullptr;
        n += other.n;
        other.r = nullptr;
        other.n = 0;
    }

    T peek() const
    {
        if (n == 0)
            throw std::underflow_error("Heap is empty");
        return r->x;
    }

    size_t size() const override { return n; }
    bool empty() const { return n == 0; }
    size_t get_merge_count() const { return merge_count; }
    void reset_merge_count() { merge_count = 0; }
};