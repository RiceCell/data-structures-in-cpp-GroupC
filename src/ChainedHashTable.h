/*
TWISTS:

1) Custom Hash Function for Primitive T Elements - Use standard C++ hashing as a base, and then pass it through a strong "mixer" or "finalizer." (MurmurHash3 64-bit avalanche mixer)
2) Custom Hash Function for ChainedHashTable<T> Elements - To support a Set of Sets. Use an XOR sum over the hashed elements, XOR is commutative, so order of elements does not matter as should a USet.
3) forEach function - To iterate over each element in the USet with a callback.
4) Simple Set Functions - To simulate interaction with other USets.
*/

#pragma once

#include "array.h"
#include "arraystack.h"
#include "uset.h"
#include <optional>
// For the Hash Function
#include <functional>
#include <cstdint>

template <typename T>
struct FastAvalancheHash {
	size_t operator()(const T& x) const {
		// 1. Get a base hash representation of the element
		size_t h = std::hash<T>{}(x);

		// 2. Apply the MurmurHash3 64-bit mix function
		h ^= h >> 33;
		h *= 0xff51afd7ed558ccdULL;
		h ^= h >> 33;
		h *= 0xc4ceb9fe1a85ec53ULL;
		h ^= h >> 33;

		return h;
	}
};

template <typename T>
class ChainedHashTable : public USet<T> {
public:
	ChainedHashTable()
		: arr(2), n{ 0 } 
	{ };

	std::optional<T> find(const T& x) const override {
		size_t j{ FastAvalancheHash<T>{}(x) % arr.length };
		for (size_t i{ 0 }; i < arr[j].size(); i++) {
			if (x == arr[j].get(i))
				return arr[j].get(i);
		}
		return std::nullopt;
	}

	bool add(const T& x) override {
		if (find(x).has_value()) {
			return false;
		}
		if (n+1 > arr.length) {
			this->resize();
		}
		size_t j{ FastAvalancheHash<T>{}(x) % arr.length };
		arr[j].add(arr[j].size(), x);
		n++;
		return true;
	}

	std::optional<T> remove(const T& x) override {
		size_t j{ FastAvalancheHash<T>{}(x) % arr.length };
		for (size_t i{ 0 }; i < arr[j].size(); i++) {
			T y = arr[j].get(i);
			if (x == y) {
				arr[j].remove(i);
				n--;
				return y;
			}
		}
		return std::nullopt;
	}

	size_t size() const override {
		return n;
	}

	void resize() {
		// If the array is empty (length 0) initialize to 2, else double.
		size_t newLength = (arr.length == 0) ? 2 : arr.length * 2;

		array<ArrayStack<T>> newArr(newLength);
		FastAvalancheHash<T> hasher;

		for (size_t i = 0; i < arr.length; i++) {
			for (size_t k = 0; k < arr[i].size(); k++) {
				T x = arr[i].get(k);

				// Rehash the element using the NEW array length
				size_t j = hasher(x) % newLength;

				newArr[j].add(newArr[j].size(), x);
			}
		}

		arr = newArr;
	}

	void forEach(std::function<void(const T& x)> func) const override {
		for (size_t i = 0; i < arr.length; i++) {
			for (size_t k = 0; k < arr[i].size(); k++) {
				func(arr[i].get(k));
			}
		}
	}

	bool isSubsetOf(const USet<T>& b) const override {
		bool isSubsetFlag{ true };
		this->forEach(
			[&](const T& x) {
				// If element x is not in b, then this is not a subset of b.
				if (!b.find(x).has_value()) {
					isSubsetFlag = false;
				}
			}
		);
		return isSubsetFlag;
	}

	USet<T>* unionWith(const USet<T>& b) const override {
		ChainedHashTable<T>* unionSet{ new ChainedHashTable<T> };
		this->forEach(
			// Add each element of this to unionSet.
			[&](const T& x) {
				unionSet->add(x);
			}
		);
		b.forEach(
			// Add each element of b to unionSet.
			[&](const T& x) {
				unionSet->add(x);
			}
		);
		return unionSet;
	}

	USet<T>* intersectionWith(const USet<T>& b) const override {
		ChainedHashTable<T>* intersectionSet{ new ChainedHashTable<T> };
		this->forEach(
			// If this element can also be found in b, then it is in their intersection.
			[&](const T& x) {
				if (b.find(x).has_value()) {
					intersectionSet->add(x);
				}
			}
		);
		return intersectionSet;
	}

	USet<T>* differenceWith(const USet<T>& b) const override {
		ChainedHashTable<T>* differenceSet{ new ChainedHashTable<T> };
		this->forEach(
			// If this element can not be found in b, then it is in their difference.
			[&](const T& x) {
				if (!b.find(x).has_value()) {
					differenceSet->add(x);
				}
			}
		);
		return differenceSet;
	}

	size_t hashCode() const {
		size_t combinedHash = 0;
		FastAvalancheHash<T> elementHasher;

		// Iterate through all subarrays
		for (size_t i = 0; i < arr.length; i++) {
			// Iterate through all items in the current subarray's chain
			for (size_t j = 0; j < arr[i].size(); j++) {
				// XOR sum the strongly-mixed hashes of each element
				combinedHash ^= elementHasher(arr[i].get(j));
			}
		}

		// Mix the final combined hash with the total size 
		// to distinguish between empty tables and tables holding zeroes.
		size_t finalHash = combinedHash ^ n;
		finalHash ^= finalHash >> 33;
		finalHash *= 0xff51afd7ed558ccdULL;
		finalHash ^= finalHash >> 33;

		return finalHash;
	}

	~ChainedHashTable() = default;

private:
	array<ArrayStack<T>> arr;
	size_t n;
};

// Template specialization for hashing a ChainedHashTable<T>
template <typename T>
struct FastAvalancheHash<ChainedHashTable<T>> {
	size_t operator()(const ChainedHashTable<T>& table) const {
		return table.hashCode();
	}
};