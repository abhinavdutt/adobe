#include <atomic>
#include <deque>
#include <mutex>
#include <unordered_map>
#include <cstdlib>
#include <iostream>

using namespace std;
#include "ThreadPool.h"

template <class ELEM_TYPE>
MemoryPool<ELEM_TYPE> ::MemoryPool(std::function<ELEM_TYPE*(size_t)> alloc_fun, std::function<void(void*)> free_fun)
{
	_alloc_fun = alloc_fun;
	_free_fun  = free_fun;
}

template <class ELEM_TYPE>
MemoryPool<ELEM_TYPE> :: ~MemoryPool()
{
	for (auto& it : _slot_map)
	{
		auto& q = it.second;
		while (!q.empty())
		{
			auto v = q.front();
			q.pop_front();
			_free_fun(v);
		}
	}
}

template <class ELEM_TYPE>
ELEM_TYPE *MemoryPool<ELEM_TYPE>::alloc(size_t size)
{
	if (!size)
	{
		return nullptr;
	}

	//*ptr = nullptr;
	std::unique_lock<std::mutex> pool_lock(_pool_mu);
	auto& q = _slot_map[size];
	if (q.empty())
	{
		ptr = _alloc_fun(size);
		if (ptr)
		{
			_ptr_map[ptr] = size;
			_used_mem += size;
		}
	}
	else
	{
		ptr = q.front();
		q.pop_front();
	}
	return ptr;
}

template <class ELEM_TYPE>
void MemoryPool<ELEM_TYPE>::free( ELEM_TYPE*)
{
	if (!ptr)
	{
		return;
	}

	std::unique_lock<std::mutex> pool_lock(_pool_mu);
	auto size = _ptr_map[ptr];
	_slot_map[size].push_front(ptr);
}

// wraper on malloc() to typecast return type of malloc()
template <class ELEM_TYPE>
ELEM_TYPE* myalloc(size_t size) {
	return (ELEM_TYPE*) malloc(size);
}

int main()
{
	MemoryPool<int> obj(myalloc<int>, free);
	auto p1 = obj.alloc(100);
	cout << p1<<endl;
	obj.free(p1);

	MemoryPool<double> obj2(myalloc<double>, free);
	auto p2 = obj2.alloc(200);
	cout << p2<<endl;
	obj2.free(p2);

	MemoryPool<float> obj3(myalloc<float>, free);
	auto p3 = obj3.alloc(100.23);
	cout << p3<<endl;
	obj3.free(p3);

	MemoryPool<long int> obj4(myalloc<long int>, free);
	auto p4 = obj4.alloc(301);
	cout << p4<<endl;
	obj4.free(p4);


	return 0;
}
