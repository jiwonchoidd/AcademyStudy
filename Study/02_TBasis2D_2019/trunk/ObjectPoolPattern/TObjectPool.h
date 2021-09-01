//https://hackernoon.com/the-object-pool-pattern-e424507b996a
#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <tuple>

// Base class for all poolable objects
class PoolableObject {
public:
	virtual ~PoolableObject() = default;
	virtual void reset() = 0;

protected:
	PoolableObject() = default;
};

// Example of an poolable object
class UsedObject : public PoolableObject {
public:
	UsedObject(int init_num = 0) : _num{ init_num }, _init_num{ init_num } {};
	void increment() { ++_num; }

	void print() { std::cout << "My number is: " << _num << std::endl; }
	void reset() override { _num = _init_num; }

private:
	int _num;
	int _init_num;
};

// The Object Pool
template <typename T, typename... Args> class ObjectPool {
public:
	template <typename P> using pointer_type = std::unique_ptr<P, std::function<void(P*)>>;

	ObjectPool(std::size_t init_size = 0, std::size_t max_size = 10, Args&&... args)
		: _max_size{ max_size }, _available{ max_size }, _size{ 0 }, _args{ args... }
	{
		static_assert(std::is_base_of<PoolableObject, T>::value, "Must be poolable object");
		initialize(init_size);
	}

	pointer_type<T> get() {
		if (_pool.empty()) {
			if (_available == 0) {
				return nullptr;
			}
			add();
		}
		--_available;
		auto inst = std::move(_pool.top());
		_pool.pop();
		return std::move(inst);
	}

	std::size_t free() { return _available; }
	std::size_t max_size() { return _max_size; }
	std::size_t size() { return _size; }
	bool empty() { return _pool.empty(); }

private:
	// Adds a new object to the pool
	void add(T* ptr = nullptr) {
		if (ptr == nullptr) {
			ptr = create_with_params(std::index_sequence_for<Args...>());
			++_size;
		}
		else {
			ptr->reset();
			++_available;
		}

		pointer_type<T> inst(ptr, [this](T* ptr) {
			// This is the custom deleter of the unique_ptr.
			// When the object is deleted in the callers context, it will be
			// returned back to the pool by utilizing the add function
			add(ptr);
		});

		_pool.push(std::move(inst));
	}

	template <std::size_t... Is> T* create_with_params(const std::index_sequence<Is...>&) {
		return new T(std::get<Is>(_args)...);
	}

	// Initializes the pool
	void initialize(std::size_t init_size) {
		for (std::size_t i = 0; i < init_size; ++i) {
			add();
		}
	}

	std::size_t _max_size;
	std::size_t _available;
	std::size_t _size;
	std::stack<pointer_type<T>> _pool;
	std::tuple<Args...> _args;
};

