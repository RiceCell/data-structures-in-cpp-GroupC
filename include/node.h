#pragma once
#include <cstddef>

// NODE FOR SINGLY LINKED LIST
template <typename T>
struct SingleNode
{
    T data;
    SingleNode<T> *next;

    SingleNode(const T &x, SingleNode *n = nullptr)
        : data(x), next(n) {}
};

// NODE FOR DOUBLY LINKED LIST
template <typename T>
struct DoubleNode
{
    T data;
    DoubleNode<T> *prev;
    DoubleNode<T> *next;

    DoubleNode(const T &x, DoubleNode *p = nullptr, DoubleNode *n = nullptr)
        : data(x), prev(p), next(n) {}
};