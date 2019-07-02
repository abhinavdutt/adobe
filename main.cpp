/*#################################################################
#  main.cpp                                                     #
#                                                               #
#  Author: Abhinav Dutt                                         #
#                                                               #
#  main.cpp                                                     #
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
#include "ThreadPool.cpp"

int main()
{

	//Integer data type
	MemoryPool<int> obj1(malloc, free);
	auto p1 = obj1.alloc(214);
	cout<<p1<<endl;
	obj1.free(p1);

	// Double data type
	MemoryPool<double> obj2(malloc, free);
	auto p2 = obj2.alloc(100);
	cout<<p2<<endl;
	obj2.free(p2);

	//Float data type
	MemoryPool<float> obj3(malloc, free);
	auto p3 = obj3.alloc(100.234567);
	cout<<p3<<endl;
	obj3.free(p3);



	return 0;
}
