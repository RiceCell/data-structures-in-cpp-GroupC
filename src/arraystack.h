#pragma once
#include "array.h"
#include "list.h"

template <typename T>
class ArrayStack : public List<T>
{
private:
    array<T> arr;
    size_t n;

    void resize()
    {
        size_t newCap = (n == 0) ? 1 : 2 * n;
        array<T> b(newCap);
        for (size_t i = 0; i < n; i++)
            b[i] = arr[i];
        arr = b;
    }

public:
    ArrayStack(size_t capacity = 1) : arr(capacity), n(0) {}

    size_t size() const override
    {
        return n;
    }

    T get(const size_t i) const override
    {
        return arr[i];
    }

    T set(const size_t i, const T &x) override
    {
        T y = arr[i];
        arr[i] = x;
        return y;
    }

    void add(const size_t i, const T &x) override
    {
        if (n + 1 > arr.length)
            resize();
        for (size_t j = n; j > i; j--)
            arr[j] = arr[j - 1];
        arr[i] = x;
        n++;
    }

    T remove(const size_t i) override
    {
        T removed = arr[i];
        for (size_t j = i; j < n - 1; j++)
            arr[j] = arr[j + 1];
        n--;
        if (arr.length >= 3 * n && n > 0)
            resize();
        return removed;
    }
};