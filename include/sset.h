#pragma once

template <typename T>
struct SSet
{
    virtual void insert(const T &x) = 0;
    virtual T remove(const T &x) = 0;
    virtual bool find(const T &x) const = 0;
    virtual size_t size() const = 0;
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
};

// Red-Black Tree Node
template <typename T>
struct RBTNode {
    T data;
    bool isBlack;
    RBTNode<T> *left;
    RBTNode<T> *right;
    RBTNode<T> *parent;

    RBTNode() : isBlack(true), left(nullptr), right(nullptr), parent(nullptr) {}; // for root/NIL node
    RBTNode(const T &x, bool isB = false) : data(x), isBlack(isB), left(nullptr), 
                                            right(nullptr), parent(nullptr) {};
};