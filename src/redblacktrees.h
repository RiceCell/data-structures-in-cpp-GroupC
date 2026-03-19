#pragma once
#include "sset.h"

template <typename T>
class RedBlackTree {
    private:
        RBTNode *root;
        RBTNode *NIL; // empty leaves

        // rotations
        void rotateLeft(RBTNode *x) {
            RBTNode *y = x->right;
            x->right = y->left;

            if (y->left != this->NIL)
                y->left->parent = x;
            
            y->parent = x->parent;
            if (x->parent == nullptr)
                this->root = y;
            else if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;

            y->left = x;
            x->parent = y;
        }

        void rotatRight(RBTNode *x) {
            RBTNode *y = x->left;
            x->left = y->right;

            if (y->right != this->NIL)
                y->right->parent = x;
            
            y->parent = x->parent;
            if (x->parent == nullptr)
                this->root = y;
            else if (x == x->parent->right)
                x->parent->right = y;
            else
                x->parent->left = y;

            y->right = x;
            x->parent = y;
        }

    public:
        RedBlackTree() {
            this->NIL = new RBTNode<T>();
            this->root = this->NIL;
        }

        
};