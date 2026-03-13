#pragma once
#include <cstddef>

// NODE FOR SINGLY LINKED LIST
template <typename T>
struct Node
{
    T data;
    Node<T> *next;

    Node(const T &x, Node *n = nullptr)
        : data(x), next(n) {}
};
