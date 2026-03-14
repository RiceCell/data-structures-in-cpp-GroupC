#pragma once

template <typename T>
class Queue
{
public:
    virtual bool add(T x) = 0;
    virtual T remove() = 0;
    virtual size_t size() const = 0;
    virtual ~Queue() {}
};