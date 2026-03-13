#pragma once
#include <cstddef>

template <typename T>
struct Node
{
    T x;
    Node *prev;
    Node *next;

    Node()
    {
        prev = next = this;
    }

    Node(const T &x) : x(x)
    {
        prev = next = this;
    }
};
