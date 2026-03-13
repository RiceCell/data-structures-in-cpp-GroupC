// ==========================================
// ============== FIFO Queue ================
// ==========================================

#pragma once
#include "node.h"
#include <cassert>

template <typename T>
class SLList
{
private:
    Node<T> *head;
    Node<T> *tail;
    size_t n;

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
    }

    // Enqueue
    T push(const T &x)
    {
        Node<T> *u = new Node<T>(x);
        if (n == 0)
        {
            head = u;
        }
        else
        {
            tail->next = u;
        }
        tail = u;
        n++;
        return x;
    }

    // Dequeue
    T pop()
    {
        assert(n > 0);
        T x = head->data;
        Node<T> *u = head;
        head = head->next;
        delete u;
        n--;
        if (n == 0)
        {
            tail = nullptr;
        }
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
};