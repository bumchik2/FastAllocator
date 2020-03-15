//#include "fast_allocator.h"
#include "fastallocator.h"
#include "list.h"
#include "profile.h"
#include "test_runner.h"


#include <list>
#include <vector>
#include <string>


template<typename T>
std::string toString (T t) {
	std::stringstream ss;
	ss << t;
	std::string str = ss.str();
	return str;
}


template<typename T, typename Allocator>
void testThisListSpeed (list<T, Allocator> l, int operationsNumber, double push_probability) {
	for (int i = 0; i < operationsNumber; ++i) {
		double k = (double)(rand() % 10000) / (double)10000;
		if (k > push_probability && l.size() != 0) {
			l.pop_back();
		} else {
			l.push_back(T());
		}
	}
}


template<typename LIST1, typename LIST2>
void checkEqual(LIST1& l1, LIST2& l2) {
	while (l1.size() != 0) {
		auto a = l1.back();
		auto b = l2.back();
		if (a != b) {
			throw runtime_error("l1 is not equal l2");
		}
		l1.pop_back();
		l2.pop_back();
	}
	if (l2.size() != 0) {
		throw runtime_error("l1 is not equal l2");
	}
}


double randDouble (double l, double r) {
	double d = static_cast<double>(rand() % 1000) / 1000.0;
	return l + (r - l) * d;
}


template<typename Container> // list<int, Allocator> or vector<int, Allocator> for e.g.
void testThisContainer (Container l, int operationsNumber, double push_probability) {
	std::list<int> checker_list;
	for (int i = 0; i < operationsNumber; ++i) {
		double k = randDouble (0, 1);
		if (k > push_probability && l.size() != 0) {
			checker_list.pop_back();
			l.pop_back();
		} else {
			int a = rand() % 1000;
			checker_list.push_back(a);
			l.push_back(a);
		}
	}
	checkEqual(l, checker_list);
}


const int N = 1000000;
const double d = 0.8;
// parametres for correctness tests


void checkSTDAllocatorCorrectness () {
	testThisContainer (list<int, std::allocator<int>>(), N, d);
	testThisContainer (vector<int, std::allocator<int>>(), N, d);
}


void checkFastAllocatorCorrectness () {
	testThisContainer (list<int, FastAllocator<int>>(), N, d);
	testThisContainer (vector<int, FastAllocator<int>>(), N, d);
}


void checkListWithDifferentAllocators () {
	testThisContainer (List<int, std::allocator<int>>(), N, d);
	testThisContainer (List<int, FastAllocator<int>>(), N, d);
}


void TestAll(int operationsNumber, double push_probability) {
	TestRunner tr;
	{ LOG_DURATION("std::list with FastAllocator " + toString(operationsNumber) + " operations")
		testThisListSpeed(list<int, FastAllocator<int>>(), operationsNumber, push_probability);
	}
	{ LOG_DURATION("std::list with std::allocator " + toString(operationsNumber) + " operations")
		testThisListSpeed(list<int, std::allocator<int>>(), operationsNumber, push_probability);
	}
	tr.RunTest(checkSTDAllocatorCorrectness, "std::containers List std::allocator Correctness");
	tr.RunTest(checkFastAllocatorCorrectness, "std::containers FastAllocator Correctness");
	tr.RunTest(checkListWithDifferentAllocators, "List with different allocators");
}


int main () {
	srand(time(0));
	TestAll(10000000, 0.8);
	return 0;
}
