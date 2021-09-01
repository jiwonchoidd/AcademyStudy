#pragma once
#include <windows.h>
#include <iostream>
#include <assert.h>
template<class T>
class ObjectPool
{
public:
	static int g_iIndex;
	enum
	{
		POOL_MAX_SIZE = 1024, // must be power of 2
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1
	};
	static void AllFree()
	{
		for (int i = 0; i < POOL_MAX_SIZE; ++i)
		{
			void* prevVal = InterlockedExchangePointer(&mPool[i], nullptr);
			if (prevVal != nullptr)
			{
				_aligned_free(prevVal);
			}
		}
	}
	//memory pre allocation is just optional
	static void prepareAllocation()
	{
		for (int i = 0; i < POOL_MAX_SIZE; ++i)
		{
			//malloc의 경우 가상 메모리 상으로는 데이터의 배열이 순차적이지만 
			//실제 물리 메모리상에서는 순서가 불규칙하다. 이때 _aligned_malloct을 사용하면
			//실제 물리메모리까지 순차적으로 할당된다.		
			mPool[i] = _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);

			InterlockedAdd(&mTailPos, POOL_MAX_SIZE);
		}
	}

	static void* operator new(size_t objSize)
	{
		long long popPos = InterlockedIncrement64(&mHeadPos) - 1;

		void* popVal = InterlockedExchangePointer(&mPool[popPos & POOL_SIZE_MASK], nullptr);

		if (popVal != nullptr)
		{
			return popVal;
		}
		g_iIndex = popPos & POOL_SIZE_MASK;
		return _aligned_malloc(objSize, MEMORY_ALLOCATION_ALIGNMENT);
	}

	static void operator delete(void* obj)
	{
		long long insPos = InterlockedIncrement64(&mTailPos) - 1;

		void* prevVal = InterlockedExchangePointer(&mPool[insPos & POOL_SIZE_MASK], obj);

		if (prevVal != nullptr)
		{
			_aligned_free(prevVal);
		}
	}

private:

	static void* volatile mPool[POOL_MAX_SIZE];
	static long long volatile mHeadPos;
	static long long volatile mTailPos;

	static_assert((POOL_MAX_SIZE & POOL_SIZE_MASK) == 0x0, "pool's size must be power of 2");
};
template <typename T>
int ObjectPool<T>::g_iIndex = 0;
template <typename T>
void* volatile ObjectPool<T>::mPool[POOL_MAX_SIZE] = {};

template <typename T>
long long volatile ObjectPool<T>::mHeadPos(0);

template<typename T>
long long volatile ObjectPool<T>::mTailPos(0);
