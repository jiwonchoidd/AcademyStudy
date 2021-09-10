#include "TObjectPool.h"
#include <cassert>
#include <iostream>

int main() 
{
	ObjectPool<UsedObject, int> pool(0, 2, 99);
	std::cout << "Initialized object pool with max size of " << pool.max_size() << std::endl;
	std::cout << "Size of the pool: " << pool.size() << std::endl;
	std::cout << "Available objects: " << pool.free() << std::endl;

	auto inst = pool.get();
	inst->increment();
	inst->print();

	std::cout << "Available objects: " << pool.free() << std::endl;
	std::cout << "Size of the pool: " << pool.size() << std::endl;
	{
		auto inst2 = pool.get();
		inst->increment();
		inst->print();
		std::cout << "Instance 2 pointer is " << inst2.get() << std::endl;
		std::cout << "Available objects: " << pool.free() << std::endl;
		std::cout << "Size of the pool: " << pool.size() << std::endl;
	}

	std::cout << "Available objects: " << pool.free() << std::endl;
	std::cout << "Size of the pool: " << pool.size() << std::endl;
	auto inst2 = pool.get();
	std::cout << "Instance 2 pointer is " << inst2.get() << std::endl;

	auto inst3 = pool.get();
	assert(inst3 == nullptr);
	assert(pool.empty());
	std::cout << "Pool is empty, all objects are in use" << std::endl;
}