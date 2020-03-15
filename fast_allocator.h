#pragma once


#include "fixed_allocator.h"


template<typename T>
class FastAllocator {

public:

	FastAllocator() { }
	FastAllocator(const FastAllocator& fa) { }
	~FastAllocator() { }

	T* allocate (size_t n);
	void deallocate (T* p, size_t n);

	using value_type = T;

	template<typename U>
	struct rebind {
		typedef FastAllocator<U> other;
	};

private:

	template<size_t chunkSize>
	friend class FixedAllocator;

};


static const size_t _specialSize1 = 4;
static const size_t _specialSize2 = 24;
static  FixedAllocator<_specialSize1> _fa1;
static FixedAllocator<_specialSize2> _fa2;


template<typename T>
T* FastAllocator<T>::allocate(size_t n) {
	if (n * sizeof(T) == _specialSize1) {
		return reinterpret_cast<T*>(_fa1._newChunk());
	} else if (n * sizeof(T) == _specialSize2) {
		return reinterpret_cast<T*>(_fa2._newChunk());
	} else {
		return new T[n];
	}
}


template<typename T>
void FastAllocator<T>::deallocate(T* p, size_t n) {
	if (sizeof(T) * n == _specialSize1) {
		_fa1._deleteChunk(reinterpret_cast<char*>(p));
	} else if (sizeof(T) * n == _specialSize2) {
		_fa2._deleteChunk(reinterpret_cast<char*>(p));
	} else {
		delete[] p;
	}
}
