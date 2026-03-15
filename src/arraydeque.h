// FI-LI-FO-LO: An array-based deque

// TWISTS:
//  1. Remove item via element - remove all inputted items within the deque
//  2. Boundary guards - safe get/set access that returns default values instead of crashing
//  3. Allocation Telemetry — tracks internal resize events to monitor growth efficiency
//  4. Contains - checks if an element is within the deque, returns true or false

#pragma once
#include "array.h"
#include "deque.h"
#include "list.h"

template <typename T>
class ArrayDeque : public Deque<T>, public List<T> 
{
    private:
        array<T> backingArray;
        size_t dequeSize;
        size_t startingIndex;
        size_t resizeCount;

        size_t indexing(size_t i) const { return (i + startingIndex) % backingArray.length; }
            

        void resize() {
            resizeCount++;
            int newSize = 2 * this->dequeSize;
            if (newSize <= 0)
                newSize = 1;

            array<T> newArray(newSize);
            for (int i = 0; i < this->dequeSize; i++) {
                newArray[i] = this->backingArray[indexing(i)];
            }

            // replaces the backingArray pointer to the new array
            this->backingArray = newArray;
            this->startingIndex = 0;

            return;
        }

    public:
        ArrayDeque(size_t initalCapacity = 10) : backingArray(initalCapacity), dequeSize(0), startingIndex(0), resizeCount(0) {}

        void add(const size_t i, const T& x) override {
            // boundary check
            if (i > dequeSize)
                return;

            if (this->dequeSize + 1 > this->backingArray.length)
                resize();

            // to add items to the left
            if (i < this->dequeSize / 2) {
                if (this->startingIndex == 0) {
                    this->startingIndex = this->backingArray.length - 1;
                }
                else {
                    this->startingIndex--;
                }
                
                for (size_t j = 0; j < i; j++) {
                    this->backingArray[indexing(j)] = this->backingArray[indexing(j + 1)];
                }
            }
            else {

                // for the right side
                for (size_t  j = this->dequeSize; j > i; j--) {
                    this->backingArray[indexing(j)] = this->backingArray[indexing(j - 1)];
                }

            }

            this->backingArray[indexing(i)] = x;
            this->dequeSize++;
        }

	    T remove(const size_t i) override {
            // boundary check
            if (dequeSize <= 0)
                return T();

            T temp = backingArray[indexing(i)];

            // shifting everything to the right
            if (i < this->dequeSize / 2) {
                for (size_t j = i; j > 0; j--) {
                    this->backingArray[indexing(j)] = this->backingArray[indexing(j - 1)];
                }

                this->startingIndex = indexing(1);
            }
            else {
                for (size_t j = i; j < this->dequeSize - 1; j++) {
                    this->backingArray[indexing(j)] = this->backingArray[indexing(j + 1)];
                }
            }

            this->dequeSize--;

            if (this->backingArray.length >= 3 * this->dequeSize)
                resize();

            return temp;
        }

	    T get(const size_t i) const override {
            // boundary check
            if (i >= this->dequeSize)
                return T();
            return this->backingArray[indexing(i)];
        }

	    T set(const size_t i, const T& x) override {
            T temp = get(i);
            this->backingArray[indexing(i)] = x;
            return temp;
        }

	    size_t size() const override {
            return this->dequeSize;
        }

        // from the Deque ADT
        void addFirst(const T& x) override {
            add(0, x);
        }

        void addLast(const T& x) override {
            add(this->dequeSize, x);
        }

        T removeFirst() override {
            return remove(0);
        }

	    T removeLast() override {
            if (this->dequeSize > 0)
                return remove(this->dequeSize - 1);
            else
                return remove(0);
        }

	    const T peekFirst() const override {
            return get(0);
        }

	    const T peekLast() const override {
            return get(this->dequeSize - 1);
        }

	    bool isEmpty() const override {
            return (this->dequeSize == 0);
        }

        bool contains(const T& x) const override {
            for (size_t i = 0; i < this->dequeSize - 1; i++) {
                // if found within the backing array
                if (backingArray[indexing(i)] == x) {
                    return true;
                }
            }

            return false;
        }

	    void clear() {
            this->dequeSize = 0;
            this->startingIndex = 0;
            return;
        }

        // Accessor for Telemetry twist
        size_t getResizeCount() const { return resizeCount; }
};