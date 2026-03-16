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
};