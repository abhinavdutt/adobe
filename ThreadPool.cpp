/*#################################################################
#  ThreadPool.cpp                                               #
#                                                               #
#  Author: Abhinav Dutt                                         #
#                                                               #
#  ThreadPool.cpp                                               #
#                                                               #
#                                                               #
#                                                               #
#################################################################*/

#include <atomic>
#include <deque>
#include <mutex>
#include <unordered_map>
#include <iostream>

using namespace std;
#include "ThreadPool.hpp"

template <class ELEM_TYPE>
MemoryPool<ELEM_TYPE> ::MemoryPool(std::function<void*(size_t)> alloc_fun, std::function<void(void*)> free_fun)
{
	_alloc_fun = [&](size_t size){return (ELEM_TYPE*)alloc_fun(size);};
	_free_fun  = [&](ELEM_TYPE* p){free_fun(p);};
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

	ELEM_TYPE *ptr = nullptr;
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
void MemoryPool<ELEM_TYPE>::free(ELEM_TYPE* ptr)
{
	if (!ptr)
	{
		return;
	}

	std::unique_lock<std::mutex> pool_lock(_pool_mu);
	auto size = _ptr_map[ptr];
	_slot_map[size].push_front(ptr);
}
