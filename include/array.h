#include <cassert>
#include <cstddef>

template <typename T>
struct array
{
    array(size_t n)
    {
        a = new T[n];
        length = n;
    }

    ~array()
    {
        delete[] a;
    }

    T &operator[](size_t i) const
    {
        assert(i < length);
        return a[i];
    }

    array(const array<T> &) = delete;

    array<T> &operator=(array<T> &b)
    {
        if (a != nullptr)
        {
            delete[] a;
        }
        a = b.a;
        b.a = nullptr;
        length = b.length;
        b.length = 0;
        return *this;
    }

    size_t length;
    T *a;
};