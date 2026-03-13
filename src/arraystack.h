// ==========================================
// ============== FILO Queue ================
// ==========================================

#pragma once
#include <iostream>
#include "array.h"
#include "list.h"

template <typename T>
class ArrayStack : public List<T>
{
private:
    array<T> arr;
    size_t n;
    size_t resize_count = 0;

    void resize()
    {
        resize_count++;

        size_t newCap = (n == 0) ? 1 : 2 * n;
        array<T> b(newCap);
        for (size_t i = 0; i < n; i++)
            b[i] = arr[i];
        for (size_t i = n; i < newCap; i++)
            b[i] = T();

        arr = b;
    }

public:
    ArrayStack(size_t capacity = 1) : arr(capacity), n(0) {}

    size_t size() const override { return n; }

    T get(const size_t i) const override
    {
        if (i >= n)
            return T();
        return arr[i];
    }

    T set(const size_t i, const T &x) override
    {
        if (i >= n)
            return T();
        T y = arr[i];
        arr[i] = x;
        return y;
    }

    void add(const size_t i, const T &x) override
    {
        if (n + 1 > arr.length)
            resize();
        if (i < n)
        {
            for (size_t j = n; j > i; j--)
                arr[j] = arr[j - 1];
        }

        arr[i] = x;
        n++;
    }

    T remove(const size_t i) override
    {
        T removed = arr[i];
        for (size_t j = i; j < n - 1; j++)
            arr[j] = arr[j + 1];

        arr[n - 1] = T();

        n--;
        if (arr.length >= 3 * n && n > 0)
            resize();
        return removed;
    }
};