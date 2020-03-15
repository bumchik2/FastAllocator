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
