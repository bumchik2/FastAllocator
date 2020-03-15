#pragma once


#include <stdexcept>
#include <iostream>


template<typename T, typename Allocator = std::allocator<T>>
class List {

private:

	class Node {
	public:
		Node() : left(0), right(0), value(T()) { }
		Node(T value) : left(0), right(0), value(value) { }
		~Node() { }
		Node* left;
		Node* right;
		T value;
	};

public:

	explicit List(const Allocator& alloc = Allocator());
	List (size_t count, const T& value = T(), const Allocator& alloc = Allocator());
	List(const List& l);

	~List();

	void push_front(const T& value);
	void pop_front ();

	void push_back(const T& value);
	void pop_back();

	size_t size () const;

	void insert_before(Node* node, const T& value);
	void insert_after(Node* node, const T& value);
	void erase (Node* node);

	T back() const;
	T front() const;

	void print () const;

private:

	size_t _size;
	Node* _first;
	Node* _last;

	typename Allocator::template rebind<Node>::other _alloc;

	Node* _makeNewNode(const T& value = T());

};

template<typename T, typename Allocator>
List<T, Allocator>::List(const Allocator& alloc) : _size(0), _first(0), _last(0),
		_alloc(typename Allocator::template rebind<Node>::other()) {
}


template<typename T, typename Allocator>
List<T, Allocator>::List (size_t count, const T& value, const Allocator& alloc) :
		_size(0), _first(0), _last(0), _alloc(typename Allocator::template rebind<Node>::other()) {
	for (int i = 0; i < static_cast<int>(count); ++i) {
		push_back(value);
	}
}


template<typename T, typename Allocator>
List<T, Allocator>::List (const List<T, Allocator>& l) : _size(0), _first(0),
		_last(0), _alloc(typename Allocator::template rebind<Node>::other()) {
	Node* current_node = l._first;
	while (current_node != 0) {
		push_back(current_node->value);
		current_node = current_node->right;
	}
}


template<typename T, typename Allocator>
List<T, Allocator>::~List() {
	Node* current_node = _first;
	while (current_node != 0) {
		Node* temp = current_node->right;
		_alloc.deallocate(current_node, 1);
		current_node = temp;
	}
}


template<typename T, typename Allocator>
typename List<T, Allocator>::Node* List<T, Allocator>::_makeNewNode(const T& value) {
	Node* new_node = _alloc.allocate(1);
	new (new_node) Node (value);
	return new_node;
}


template<typename T, typename Allocator>
void List<T, Allocator>::insert_before(Node* node, const T& value) {
	Node* new_node = _makeNewNode(value);
	if (_size == 0) {
		_first = new_node;
		_last = new_node;
	} else {
		if (node -> left == 0) {
			_first = new_node;
		} else {
			node -> left -> right = new_node;
			new_node -> left = node -> left;
		}
		node -> left = new_node;
		new_node -> right = node;
	}
	++_size;
}


template<typename T, typename Allocator>
void List<T, Allocator>::insert_after(Node* node, const T& value) {
	Node* new_node = _makeNewNode(value);
	if (_size == 0) {
		_first = new_node;
		_last = new_node;
	} else {
		if (node -> right == 0) {
			_last = new_node;
		} else {
			node -> right -> left = new_node;
			new_node -> right = node -> right;
		}
		node -> right = new_node;
		new_node -> left = node;
	}
	++_size;
}


template<typename T, typename Allocator>
void List<T, Allocator>::erase(Node* node) {
	if (node -> left == 0) {
		pop_front();
	} else if (node -> right == 0) {
		pop_back();
	} else {
		node -> left -> right = node -> right;
		node -> right -> left = node -> left;
		node -> ~Node();
		_alloc.deallocate(node, 1);
	}
	--_size;
}


template<typename T, typename Allocator>
void List<T, Allocator>::push_front(const T& value) {
	insert_before(_first, value);
}


template<typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
	if (_size == 0) {
		throw std::runtime_error("can't pop from empty list");
	} else if (_size == 1) {
		_first = 0;
		_last = 0;
		_alloc.deallocate(_first, 1);
	} else {
		Node* temp = _first;
		_first = _first -> right;
		_first -> left = 0;
		temp -> ~Node();
		_alloc.deallocate(temp, 1);
	}
	--_size;
}


template<typename T, typename Allocator>
void List<T, Allocator>::push_back(const T& value) {
	insert_after(_last, value);
}


template<typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
	if (_size == 0) {
		throw std::runtime_error("can't pop from empty list");
	} else if (_size == 1) {
		_first = 0;
		_last = 0;
		_alloc.deallocate(_last, 1);
	} else {
		Node* temp = _last;
		_last = _last -> left;
		_last -> right = 0;
		temp -> ~Node();
		_alloc.deallocate(temp, 1);
	}
	--_size;
}


template<typename T, typename Allocator>
size_t List<T, Allocator>::size () const {
	return _size;
}


template<typename T, typename Allocator>
T List<T, Allocator>::front () const {
	if (_size == 0) {
		throw std::runtime_error("no front element in empty List");
	} else {
		return _first -> value;
	}
}


template<typename T, typename Allocator>
T List<T, Allocator>::back () const {
	if (_size == 0) {
		throw std::runtime_error("no front element in empty List");
	} else {
		return _last -> value;
	}
}


template<typename T, typename Allocator>
void List<T, Allocator>::print () const {
	Node* current_node = _first;
	while (current_node != 0) {
		Node* temp = current_node->right;
		std::cout << current_node -> value << ' ';
		current_node = temp;
	}
	std::cout << std::endl;
}
