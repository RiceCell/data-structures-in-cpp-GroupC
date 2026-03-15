// FIFO Queue: A doubly linked list

// TWISTS:
//   1. Node Pooling    — reuses freed nodes instead of new/delete every time
//   2. Self-Healing    — detects and breaks cycles via Floyd's algorithm on push

#pragma once
#include "node.h"
#include <cassert>
#include <vector>

template <typename T>
class DLList
{
private:
    DoubleNode<T> *head;
    DoubleNode<T> *tail;
    size_t n;

    std::vector<DoubleNode<T> *> pool;

    DoubleNode<T> *acquire(const T &x)
    {
        if (!pool.empty())
        {
            DoubleNode<T> *recycled = pool.back();
            pool.pop_back();
            recycled->data = x;
            recycled->next = nullptr;
            recycled->prev = nullptr;
            return recycled;
        }
        return new DoubleNode<T>(x);
    }

    void release(DoubleNode<T> *u)
    {
        u->next = nullptr;
        u->prev = nullptr;
        pool.push_back(u);
    }

    // Floyd's Cycle Detection
    // Runs on every push. If a cycle is found, we break it. In normal use this never triggers — but it saves you if
    // something corrupts the list (e.g. a bad external pointer).

    void heal_cycles()
    {
        if (!head)
            return;

        DoubleNode<T> *slow = head;
        DoubleNode<T> *fast = head;

        bool cycle_found = false;
        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast)
            {
                cycle_found = true;
                break;
            }
        }

        if (!cycle_found)
            return;

        slow = head;
        while (slow != fast)
        {
            slow = slow->next;
            fast = fast->next;
        }

        DoubleNode<T> *cycle_end = slow;
        while (cycle_end->next != slow)
            cycle_end = cycle_end->next;

        cycle_end->next = nullptr;
        tail = cycle_end;
    }

public:
    DLList() : head(nullptr), tail(nullptr), n(0) {}

    ~DLList()
    {
        while (head)
        {
            DoubleNode<T> *temp = head;
            head = head->next;
            delete temp;
        }

        for (DoubleNode<T> *u : pool)
            delete u;
        pool.clear();
    }

    T push(const T &x)
    {
        DoubleNode<T> *u = acquire(x);
        if (n == 0)
            head = u;
        else {
            tail->next = u;
            u->prev = tail;
        }
            
        tail = u;
        n++;

        if (tail && tail->next != nullptr)
            heal_cycles();

        return x;
    }

    T pop()
    {
        assert(n > 0);
        T x = head->data;
        DoubleNode<T> *u = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
        
        release(u);
        n--;
        return x;
    }

    T get(size_t i)
    {
        assert(i < n);
        DoubleNode<T> *ptr = head;
        for (size_t j = 0; j < i; j++)
            ptr = ptr->next;
        return ptr->data;
    }

    size_t size() const { return n; }

    size_t pool_size() const { return pool.size(); }
};