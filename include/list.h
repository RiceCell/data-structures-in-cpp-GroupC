#pragma once
#include <cstddef>

template <typename T>
struct List
{
    virtual void add(const size_t i, const T &x) = 0;
    virtual T remove(const size_t i) = 0;
    virtual T get(const size_t i) const = 0;
    virtual T set(const size_t i, const T &x) = 0;
    virtual size_t size() const = 0;
};