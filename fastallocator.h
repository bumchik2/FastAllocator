#pragma once


#include <vector>


using std::vector;


template<size_t chunkSize>
class FixedAllocator {

public:

	FixedAllocator() { }
	~FixedAllocator();

private:

	const size_t _long_chunk_length = 1000;
	size_t _chunks_left = 0;
	vector<char*> _long_chunks;

	void _makeLongChunk();

	char* _newChunk();
	void _deleteChunk(char* p);

	template<typename T>
	friend class FastAllocator;

};


template<size_t chunkSize>
FixedAllocator<chunkSize>::~FixedAllocator() {
	for (int i = 0; i < static_cast<int>(_long_chunks.size()); ++i) {
		delete [] _long_chunks[i];
	}
}


template<size_t chunkSize>
void FixedAllocator<chunkSize>::_makeLongChunk() {
	_long_chunks.push_back(new char [_long_chunk_length * chunkSize]);
	_chunks_left += _long_chunk_length;
}


template<size_t chunkSize>
char* FixedAllocator<chunkSize>::_newChunk() {
	if (_chunks_left == 0) {
		_makeLongChunk();
	}
	--_chunks_left;
	return _long_chunks.back() + (_long_chunk_length - _chunks_left - 1) * chunkSize;
}


template<size_t chunkSize>
void FixedAllocator<chunkSize>::_deleteChunk(char* p){
}



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
