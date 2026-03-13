// RUSSEL's

#pragma once
#include "array.h"
#include "list.h"

template <typename T>
class ArrayStack : public List<T>
{
private:
    array<T> arr;
    size_t size;

    void resize()
    {
        size_t newCap = (arr.length == 0) ? 1 : arr.length * 2; // Use arr.length for the calculation, not size
        if (size > 0 && arr.length >= 3 * size)
        {
            newCap = arr.length / 2;
        }

        array<T> b(newCap);
        for (size_t k = 0; k < size; k++)
        {
            b[k] = arr[k];
        }
        arr = b; //  uses custom assignment operator
    }

public:
    ArrayStack(size_t capacity = 1) : arr(capacity), size(0) {}

    size_t size() const override
    {
        return size;
    }

    T get(size_t i) const override
    {
        return arr[i];
    }

    T set(size_t i, const T &x) override
    {
        T y = arr[i];
        arr[i] = x;
        return y;
    }

    void add(size_t i, const T &x) override
    {
        assert(i < arr.length);
        if (size + 1 > arr.length)
            resize();
        for (size_t j = size; j > i; j--)
            arr[j] = arr[j - 1];
        arr[i] = x;
        size++;
    }

    T remove(size_t i) override
    {
        assert(i < arr.length);
        T removed = arr[i];
        for (size_t j = i; j < size - 1; j++)
            arr[j] = arr[j + 1];
        size--;
        if (arr.length >= 3 * size && size > 0)
            resize();
        return removed;
    }
};