/*
TWISTS:

1) forEach function - To iterate over each element in the USet with a callback.
2) Simple Set Functions - To simulate interaction with other USets.
*/

#pragma once

#include <functional>
#include <optional>

template <typename T>
struct USet {
	virtual std::optional<T> find(const T& x) const = 0;
	virtual bool add(const T& x) = 0;
	virtual std::optional<T> remove(const T& x) = 0;
	virtual size_t size() const = 0;

	// Should iterate over each element in the USet, pass it as x, and do something with it.
	virtual void forEach(std::function<void(const T& x)> func) const = 0;

	// Set operations returning pointers to avoid object slicing.
	virtual bool isSubsetOf(const USet<T>& b) const = 0;
	virtual USet<T>* unionWith(const USet<T>& b) const = 0;
	virtual USet<T>* intersectionWith(const USet<T>& b) const = 0;
	virtual USet<T>* differenceWith(const USet<T>& b) const = 0;

	virtual ~USet() = default;
};