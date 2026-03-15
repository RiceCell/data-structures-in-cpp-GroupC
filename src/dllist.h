// FIFO Queue: A doubly linked list

// TWISTS:
//  1. Node Pooling     — reuses freed nodes instead of new/delete every time
//  2. Self-Healing     — detects and breaks cycles via Floyd's algorithm on push
//  3. Traversal System — can go forward or backward and insert after and before a node

#pragma once
#include "node.h"
#include <cassert>
#include <vector>

template <typename T>
class DLList
{
    private:
        DoubleNode<T> *head;
        DoubleNode<T> *tail;
        DoubleNode<T> *current; // for the manual traversal
        size_t n;

        std::vector<DoubleNode<T> *> pool;

        DoubleNode<T> *acquire(const T &x)
        {
            if (!pool.empty())
            {
                DoubleNode<T> *recycled = pool.back();
                pool.pop_back();
                recycled->data = x;
                recycled->next = nullptr;
                recycled->prev = nullptr;
                return recycled;
            }
            return new DoubleNode<T>(x);
        }

        void release(DoubleNode<T> *u)
        {
            u->next = nullptr;
            u->prev = nullptr;
            pool.push_back(u);
        }

        // Floyd's Cycle Detection
        // Runs on every push. If a cycle is found, we break it. In normal use this never triggers — but it saves you if
        // something corrupts the list (e.g. a bad external pointer).

        void heal_cycles()
        {
            if (!head)
                return;

            DoubleNode<T> *slow = head;
            DoubleNode<T> *fast = head;

            bool cycle_found = false;
            while (fast && fast->next)
            {
                slow = slow->next;
                fast = fast->next->next;
                if (slow == fast)
                {
                    cycle_found = true;
                    break;
                }
            }

            if (!cycle_found)
                return;

            slow = head;
            while (slow != fast)
            {
                slow = slow->next;
                fast = fast->next;
            }

            DoubleNode<T> *cycle_end = slow;
            while (cycle_end->next != slow)
                cycle_end = cycle_end->next;

            cycle_end->next = nullptr;
            tail = cycle_end;
        }

    public:
        DLList() : head(nullptr), tail(nullptr), current(nullptr), n(0) {}

        ~DLList()
        {
            while (head)
            {
                DoubleNode<T> *temp = head;
                head = head->next;
                delete temp;
            }

            for (DoubleNode<T> *u : pool)
                delete u;
            pool.clear();
        }

        T push(const T &x) {
            DoubleNode<T> *u = acquire(x);
            if (n == 0) {
                head = u;
                current = head; // traversal always starts at head
            }
            else {
                tail->next = u;
                u->prev = tail;
            }
                
            tail = u;
            n++;

            if (tail && tail->next != nullptr)
                heal_cycles();

            return x;
        }

        T pop() {
            assert(n > 0);
            T x = head->data;
            DoubleNode<T> *u = head;

            if (current == u)
                current = head->next;

            head = head->next;
            if (head)
                head->prev = nullptr;
            else
                tail = nullptr;
            
            release(u);
            n--;
            return x;
        }

        T get(size_t i) {
            assert(i < n);
            DoubleNode<T> *ptr = head;
            for (size_t j = 0; j < i; j++)
                ptr = ptr->next;
            return ptr->data;
        }

        size_t size() const { return n; }

        // gives current node
        T currentNode() {
            return current->data;
        }

        // moves current node to next node
        T moveNextNode() {
            // safeguarding
            if (current->next == nullptr)
                return T();

            current = current->next;
            return current->data;
        }

        // moves current node to previous node
        T movePreviousNode() {
            // safeguarding
            if (current->prev == nullptr)
                return T();

            current = current->prev;
            return current->data;
        }

        // inserts after current node
        T insertAfterNode(const T &x) {
            // if current is last index
            if (current == tail) return push(x);

            DoubleNode<T> *u = acquire(x);
            u->next = current->next;
            current->next->prev = u;
            current->next = u;
            u->prev = current;

            n++;
            return x;
        }

        // inserts before current node
        T insertBeforeNode(const T &x) {
            DoubleNode<T> *u = acquire(x);
            u->next = current;
            u->prev = current->prev;

            // if the previous is the head
            if (current->prev != nullptr) {
                current->prev->next = u;
            } 
            else {
                head = u;
            }
            current->prev = u;

            n++;
            return x;
        }

        size_t pool_size() const { return pool.size(); }
};