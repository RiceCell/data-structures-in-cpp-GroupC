#pragma once
#include "sset.h"

template <typename T>
class RedBlackTree: SSet<T> {
    private:
        RBTNode<T> *root;
        RBTNode<T> *NIL; // empty leaves
        size_t treeSize = 0;
        int searchCount = 0;

        // rotations
        void rotateLeft(RBTNode<T> *x) {
            RBTNode<T> *y = x->right;
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

        void rotateRight(RBTNode<T> *x) {
            RBTNode<T> *y = x->left;
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

        // insertion helper function
        void fixInsert(RBTNode<T> *n) {
            RBTNode<T> *unc; // uncle node

            // while the parent aint null and the color is red
            while (n->parent && !(n->parent->isBlack)) {
                if (n->parent == n->parent->parent->left) {
                    unc = n->parent->parent->right;
                    
                    if (!(unc->isBlack)) {
                        // turns both uncle and curr node to black and grandparent to red
                        n->parent->isBlack = true;
                        unc->isBlack = true;
                        n->parent->parent->isBlack = false;
                        n = n->parent->parent;
                    }
                    else {
                        if (n == n->parent->right) {
                            n = n->parent;
                            rotateLeft(n);
                        }

                        n->parent->isBlack = true;
                        n->parent->parent->isBlack = false;
                        rotateRight(n->parent->parent);
                    }

                }
                else {
                    unc = n->parent->parent->left;
                    
                    if (!(unc->isBlack)) {
                        // turns both uncle and curr node to black and grandparent to red
                        n->parent->isBlack = true;
                        unc->isBlack = true;
                        n->parent->parent->isBlack = false;
                        n = n->parent->parent;
                    }
                    else {
                        if (n == n->parent->left) {
                            n = n->parent;
                            rotateRight(n);
                        }

                        n->parent->isBlack = true;
                        n->parent->parent->isBlack = false;
                        rotateLeft(n->parent->parent);
                    }
                }
            
                root->isBlack = true;
            }
        }

        // remove helper functions
        // recursively searches for the node with the data
        // gonna actually use this for the find function too so there is also searchCount
        RBTNode<T> *search(RBTNode<T> *n, const T key) {
            this->searchCount++;

            if (n == this->NIL || key == n->data)
                return n;
            
            if (key < n->data)
                return search(n->left, key);

            return search(n->right, key);
        }

        // finds the node with minimum value
        RBTNode<T> *minimum(RBTNode<T> *n) {
            while (n->left != this->NIL) {
                n = n->left;
            }
            return n;
        }

        // moves subtree from node n to node m
        void transplant(RBTNode<T> *n, RBTNode<T> *m) {
            if (n->parent == nullptr)
                this->root = m;
            else if (n == n->parent->left)
                n->parent->left = m;
            else
                n->parent->right = m;
            
            m->parent = n->parent;
        }

        void fixRemove(RBTNode<T> *n) {
            RBTNode<T> *sibling;

            while (n != this->root && n->isBlack) {
                // when n is the left child of its parent
                if (n == n->parent->left) {
                    sibling = n->parent->right;
                    if (!(sibling->isBlack)) {
                        sibling->isBlack = true;
                        n->parent->isBlack = false;
                        rotateLeft(n->parent);
                        sibling = n->parent->right
                    }
                
                    if (sibling->left->isBlack && sibling->right->isBlack) {
                        sibling->isBlack = false;
                        n = n->parent;
                    }
                    else {
                        if (sibling->right->isBlack) {
                            sibling->left->isBlack = true;
                            sibling->isBlack = false;
                            rotateRight(sibling);
                            sibling = n->parent->right;
                        }

                        sibling->isBlack = n->parent->isBlack;
                        n->parent->isBlack = true;
                        sibling->right->isBlack = true;
                        rotateRight(n->parent);
                        n = this->root;
                    }
                }
                // when n is the right child of its parent
                else {
                    sibling = n->parent->left;
                    if (!(sibling->isBlack)) {
                        sibling->isBlack = true;
                        n->parent->isBlack = false;
                        rotateRight(n->parent);
                        sibling = n->parent->left
                    }
                
                    if (sibling->right->isBlack && sibling->left->isBlack) {
                        sibling->isBlack = false;
                        n = n->parent;
                    }
                    else {
                        if (sibling->left->isBlack) {
                            sibling->right->isBlack = true;
                            sibling->isBlack = false;
                            rotateLeft(sibling);
                            sibling = n->parent->left;
                        }

                        sibling->isBlack = n->parent->isBlack;
                        n->parent->isBlack = true;
                        sibling->left->isBlack = true;
                        rotateRight(n->parent);
                        n = this->root;
                    }
                }
            
                n->isBlack = true;
            }
        }

        int getHeight(RBTNode<T> *r) {
            if (r == this->NIL) {
                return -1;
            }

            int leftH = getHeight(r->left);
            int rightH = getHeight(r->right);
            
            return (((leftH < rightH) ? rightH : leftH) + 1);
        }

    public:
        RedBlackTree() {
            this->NIL = new RBTNode<T>();
            this->root = this->NIL;
            this->treeSize = 0;
        }

        void insert(const T &x) override {
            RBTNode<T> *newNode = new RBTNode<T>(x);
            newNode->left = newNode->right = this->NIL;

            // for temporary parent and current nodes
            RBTNode<T> *par, *curr = this->root;

            while (curr != this->NIL) {
                par = curr;
                if (newNode->data < curr->data)
                    curr = curr->left;
                else   
                    curr = curr->right;
            }

            newNode->parent = par;
            if (par == nullptr) 
                this->root = newNode;
            else if (newNode->data < par->data)
                par->left = newNode;
            else
                par->right = newNode;

            this->fixInsert(newNode);
            this->treeSize++;
        }

        T remove(const T &x) override {
            RBTNode<T> *toBeDeleted = this->search(this->root, x);
            if (toBeDeleted == this->NIL) { return T(); } // no such value was found

            // variables to aid in deletion
            T tempData = toBeDeleted->data;
            RBTNode<T> *tempNode = toBeDeleted;
            bool temp_isBlack_var = tempNode->isBlack;
            RBTNode<T> *child;

            if (toBeDeleted->left == this->NIL) {
                child = toBeDeleted->right;
                this->transplant(toBeDeleted, toBeDeleted->right);
            }
            else if (toBeDeleted->right == this->NIL) {
                child = toBeDeleted->left;
                this->transplant(toBeDeleted, toBeDeleted->left);
            }
            else {
                tempNode = this->minimum(toBeDeleted->right);
                temp_isBlack_var = tempNode->isBlack;
                child = tempNode->right;

                if (tempNode->parent == toBeDeleted) {
                    child->parent = tempNode;
                }
                else {
                    this->transplant(tempNode, tempNode->right);
                    tempNode->right = toBeDeleted->right;
                    tempNode->right->parent = tempNode;
                }

                this->transplant(toBeDeleted, tempNode);
                tempNode->left = toBeDeleted->left;
                tempNode->left->parent = tempNode;
                tempNode->isBlack = toBeDeleted->isBlack;
            }

            if (temp_isBlack_var)
                this->fixRemove(child);

            return tempData;
        }

        bool find(const T &x) const override {
            this->searchCount = 0;
            if (this->search(this->root, x) != this->NIL)
                return true;
            
            return false;
        }

        // must do find(x) first
        int currentFindCount() { return this->searchCount; }

        size_t size() const override { return this->treeSize; }

        size_t height() { return getHeight(this->root); }
};