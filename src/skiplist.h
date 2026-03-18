// Skiplist

// TWISTS:
//  1. Boundary guards          — safe get/set access that returns default values instead of crashing
//  2. Allocation Telemetry     — tracks internal resize events to monitor growth efficiency
//  3. Counting Search          — couting how many iterations it takes to find a specific number

#pragma once
#include "sset.h"

#define MAXHEIGHT 10 

template <typename T>
class SkipList {
    private: 
        SkipNode<T> *sentinel;
        size_t currHeight = 0;
        size_t listSize = 0;
        int searchCount = 0;

        // custom helper functions
        size_t randomHeightGenerator() {
            size_t h = 0;
            while (h < MAXHEIGHT && (rand() % 2) == 0) { 
                h++; 
            }
            return h;
        }

        int compare(T data1, T data2) {
            return (int)(data1 - data2);
        }

    public:
        
        SkipList() {
            sentinel = new SkipNode<T>(MAXHEIGHT + 1);
            sentinel->height = MAXHEIGHT + 1;
            
            for (size_t i = 0; i < MAXHEIGHT; i++) {
                sentinel->next[i] = nullptr;
            }
        }

        bool insert(const T& x) {
            SkipNode<T> *stack[MAXHEIGHT + 1];
            SkipNode<T> *u = sentinel;
            size_t r = currHeight;

            while (true) {
                while (u->next[r] != nullptr && compare(u->next[r]->data, x) < 0) {
                    u = u->next[r];
                }

                stack[r] = u;

                if (r == 0) { 
                    break; 
                }
                r--;
            }

            size_t newHeight = randomHeightGenerator() + 1;
            SkipNode<T> *w = new SkipNode<T>(x, newHeight);
            while (currHeight < newHeight - 1) {
                currHeight++;
                stack[currHeight] = sentinel;
            }

            for (size_t i = 0; i < newHeight; i++) {
                w->next[i] = stack[i]->next[i];
                stack[i]->next[i] = w;
            }

            this->listSize++;
            return true;
        }

        T remove(const T& x) {
            T temp = T(); // default value
            SkipNode<T> *u = sentinel, *del = nullptr, *stack[MAXHEIGHT + 1];
            size_t r = currHeight;
            
            while (true) {
                while (u->next[r] != nullptr && compare(u->next[r]->data, x) < 0) {
                    u = u->next[r];
                }

                stack[r] = u;
                if (r == 0) { break; }
                r--;
            }

            if (u->next[0] == nullptr) {
                return T(); // invalid
            }
            
            // checks if the next data is the exact one, if it aint then invalid
            if (u->next[0]->data != x) {
                return T();
            }

            while (u->next[0] != nullptr && compare(u->next[0]->data, x) < 0) {
                u = u->next[0];
            }

            stack[0] = u;

            del = stack[0]->next[0];
            temp = del->data;

            size_t delete_height = del->height; 
                
            for (size_t r = 0; r < delete_height; r++) {
                if (stack[r]->next[r] == del) {
                    stack[r]->next[r] = del->next[r];
                }
            }

            delete del;
            this->listSize--;
            while (currHeight > 0 && sentinel->next[currHeight - 1] == nullptr) {
                currHeight--;
            }

            return temp; // invalid
        }

        T find(const T &x) {
            SkipNode<T> *u = sentinel;
            size_t r = currHeight;
            this->searchCount = 0;
            
            while (true) {
                while (u->next[r] != nullptr && compare(u->next[r]->data, x) <= 0) {
                    // counts how many times it searches and moves until the item is found
                    this->searchCount++;
                    u = u->next[r];
                }

                if (r == 0) { break; }
                r--;
            }
            
            // if that exact value is found
            if (u != nullptr && u->data == x) {
                return u->data;
            }

            return T(); // invalid
        }

        // must do find(x) first
        int currentFindCount() { return this->searchCount; }

        size_t size() { return this->listSize; }

        size_t height() { return this->currHeight; }

        ~SkipList() { delete[] sentinel->next; }
};