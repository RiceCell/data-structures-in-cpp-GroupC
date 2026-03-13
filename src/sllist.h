
// FIFO Queue: A singly linked list

// TWISTS:
//   1. Node Pooling    — reuses freed nodes instead of new/delete every time
//   2. Self-Healing    — detects and breaks cycles via Floyd's algorithm on push

#pragma once
#include "node.h"
#include <cassert>
#include <vector>

template <typename T>
class SLList
{
private:
    Node<T> *head;
    Node<T> *tail;
    size_t n;

    std::vector<Node<T> *> pool;

    Node<T> *acquire(const T &x)
    {
        if (!pool.empty())
        {
            Node<T> *recycled = pool.back();
            pool.pop_back();
            recycled->data = x;
            recycled->next = nullptr;
            return recycled;
        }
        return new Node<T>(x);
    }

    void release(Node<T> *u)
    {
        u->next = nullptr;
        pool.push_back(u);
    }

    // Floyd's Cycle Detection
    // Runs on every push. If a cycle is found, we break it. In normal use this never triggers — but it saves you if
    // something corrupts the list (e.g. a bad external pointer).

    void heal_cycles()
    {
        if (!head)
            return;

        Node<T> *slow = head;
        Node<T> *fast = head;

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

        Node<T> *cycle_end = slow;
        while (cycle_end->next != slow)
            cycle_end = cycle_end->next;

        cycle_end->next = nullptr;
        tail = cycle_end;
    }

public:
    SLList() : head(nullptr), tail(nullptr), n(0) {}

    ~SLList()
    {
        while (head)
        {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
        }

        for (Node<T> *u : pool)
            delete u;
        pool.clear();
    }

    T push(const T &x)
    {
        Node<T> *u = acquire(x);
        if (n == 0)
            head = u;
        else
            tail->next = u;
        tail = u;
        n++;

        heal_cycles();
        return x;
    }

    T pop()
    {
        assert(n > 0);
        T x = head->data;
        Node<T> *u = head;
        head = head->next;
        release(u);
        n--;
        if (n == 0)
            tail = nullptr;
        return x;
    }

    T get(size_t i)
    {
        assert(i < n);
        Node<T> *ptr = head;
        for (size_t j = 0; j < i; j++)
            ptr = ptr->next;
        return ptr->data;
    }

    size_t size() const { return n; }

    size_t pool_size() const { return pool.size(); }
};