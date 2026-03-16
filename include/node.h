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

// NODE FOR SKIPLISTS
template <typename T>
class SkipNode {
    public:
        T data;
        size_t height;
        SkipNode **next;

        // for sentinel
        SkipNode(size_t height) : height(height) {
            next = new SkipNode*[height];

            for (size_t i = 0; i < height; i++) {
                next[i] = nullptr;
            }
        }

        SkipNode(T x, size_t height) : data(x), height(height) {
            next = new SkipNode*[height];

            for (size_t i = 0; i < height; i++) {
                next[i] = nullptr;
            }
        }

        ~SkipNode() {
            delete[] next;
        }
}