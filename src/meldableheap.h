// Priority Queue: Meldable Heap

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
    struct HeapNode
    {
        T val;
        HeapNode *lc, *rc, *par;

        HeapNode(const T &v) : val(v), lc(nullptr), rc(nullptr), par(nullptr) {}
    };

    HeapNode *root;
    size_t count;
    size_t merge_count;

    HeapNode *meld(HeapNode *a, HeapNode *b)
    {
        if (!a)
            return b;
        if (!b)
            return a;

        if (a->val > b->val)
            std::swap(a, b);

        merge_count++;

        bool goLeft = rand() % 2;

        if (goLeft)
        {
            a->lc = meld(a->lc, b);
            if (a->lc)
                a->lc->par = a;
        }
        else
        {
            a->rc = meld(a->rc, b);
            if (a->rc)
                a->rc->par = a;
        }

        return a;
    }

    void freeTree(HeapNode *node)
    {
        if (!node)
            return;
        freeTree(node->lc);
        freeTree(node->rc);
        delete node;
    }

public:
    MeldableHeap() : root(nullptr), count(0), merge_count(0) {}

    ~MeldableHeap() { freeTree(root); }

    bool add(T x) override
    {
        HeapNode *fresh = new HeapNode(x);
        root = meld(fresh, root);
        if (root)
            root->par = nullptr;
        count++;
        return true;
    }

    T remove() override
    {
        assert(count > 0);

        T top = root->val;
        HeapNode *old = root;
        root = meld(root->lc, root->rc);
        delete old;

        if (root)
            root->par = nullptr;
        count--;
        return top;
    }

    void remove(HeapNode *node)
    {
        HeapNode *merged = meld(node->lc, node->rc);
        if (merged)
            merged->par = node->par;

        if (!node->par)
            root = merged;
        else if (node->par->lc == node)
            node->par->lc = merged;
        else
            node->par->rc = merged;

        delete node;
        count--;
    }

    void absorb(MeldableHeap<T> &other)
    {
        root = meld(root, other.root);
        if (root)
            root->par = nullptr;
        count += other.count;
        other.root = nullptr;
        other.count = 0;
    }

    T peek() const
    {
        if (count == 0)
            throw std::underflow_error("Heap is empty");
        return root->val;
    }

    size_t size() const override { return count; }
    bool empty() const { return count == 0; }

    size_t get_merge_count() const { return merge_count; }
    void reset_merge_count() { merge_count = 0; }
};