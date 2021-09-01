// 11장 템플릿을 이용한 제네릭 코드 작성
//

#include "stdafx.h"
#include <iostream>  
using namespace std;
#define Partial 
#ifndef Partial
template <class Key, class Value> 
class Dictionary 
{
	Key* keys;
	Value* values;
	int size;
	int max_size;
public:
	Dictionary(int initial_size) : size(0) 
	{
		max_size = 1;
		while (initial_size >= max_size)
			max_size *= 2;
		keys = new Key[max_size];
		values = new Value[max_size];
	}
	void add(Key key, Value value) 
	{
		Key* tmpKey;
		Value* tmpVal;
		if (size + 1 >= max_size) 
		{
			max_size *= 2;
			tmpKey = new Key[max_size];
			tmpVal = new Value[max_size];
			for (int i = 0; i < size; i++) 
			{
				tmpKey[i] = keys[i];
				tmpVal[i] = values[i];
			}
			tmpKey[size] = key;
			tmpVal[size] = value;
			delete[] keys;
			delete[] values;
			keys = tmpKey;
			values = tmpVal;
		}
		else {
			keys[size] = key;
			values[size] = value;
		}
		size++;
	}

	void print() 
	{
		for (int i = 0; i < size; i++)
		{
			cout << "{" << keys[i] << ", " << values[i] << "}" << endl;
		}
	}
};

// Template partial specialization: Key is specified to be int.  
template <class Value>
class Dictionary<int, Value> 
{
	int* keys;
	Value* values;
	int size;
	int max_size;
public:
	Dictionary(int initial_size) : size(0) {
		max_size = 1;
		while (initial_size >= max_size)
			max_size *= 2;
		keys = new int[max_size];
		values = new Value[max_size];
	}
	void add(int key, Value value) 
	{
		int* tmpKey;
		Value* tmpVal;
		if (size + 1 >= max_size) 
		{
			max_size *= 2;
			tmpKey = new int[max_size];
			tmpVal = new Value[max_size];
			for (int i = 0; i < size; i++) 
			{
				tmpKey[i] = keys[i];
				tmpVal[i] = values[i];
			}
			tmpKey[size] = key;
			tmpVal[size] = value;
			delete[] keys;
			delete[] values;
			keys = tmpKey;
			values = tmpVal;
		}
		else {
			keys[size] = key;
			values[size] = value;
		}
		size++;
	}

	void sort() 
	{
		// Sort method is defined.  
		int smallest = 0;
		for (int i = 0; i < size - 1; i++) 
		{
			for (int j = i; j < size; j++) 
			{
				if (keys[j] < keys[smallest])
					smallest = j;
			}
			swap(keys[i], keys[smallest]);
			swap(values[i], values[smallest]);
		}
	}

	void print() 
	{
		for (int i = 0; i < size; i++)
		{
			cout << "{" << keys[i] << ", " << values[i] << "}" << endl;
		}
	}
};

int main() 
{
	Dictionary<char*, char*>* dict = new Dictionary<char*, char*>(10);
	dict->print();
	dict->add("apple", "fruit");
	dict->add("banana", "fruit");
	dict->add("dog", "animal");
	dict->print();

	Dictionary<int, char*>* dict_specialized = new Dictionary<int, char*>(10);
	dict_specialized->print();
	dict_specialized->add(100, "apple");
	dict_specialized->add(101, "banana");
	dict_specialized->add(103, "dog");
	dict_specialized->add(89, "cat");
	dict_specialized->print();
	dict_specialized->sort();
	cout << endl << "Sorted list:" << endl;
	dict_specialized->print();
}
#else
// Original template collection class.  
template <class T> 
class Bag 
{
	T* elem;
	int size;
	int max_size;

public:
	Bag() : elem(0), size(0), max_size(1) {}
	void add(T t)
	{
		T* tmp;
		if (size + 1 >= max_size) 
		{
			max_size *= 2;
			tmp = new T[max_size];
			for (int i = 0; i < size; i++)
				tmp[i] = elem[i];
			tmp[size++] = t;
			delete[] elem;
			elem = tmp;
		}
		else
			elem[size++] = t;
	}

	void print() 
	{
		for (int i = 0; i < size; i++)
			cout << elem[i] << " ";
		cout << endl;
	}
};

// 포인터 유형에 대한 템플릿 부분 특수화.
// NULL을 검사하도록 컬렉션이 수정 및 저장소 유형.
template <class T> 
class Bag<T*> 
{
	T* elem;
	int size;
	int max_size;

public:
	Bag() : elem(0), size(0), max_size(1) {}
	void add(T* t) 
	{
		T* tmp;
		if (t == NULL) 
		{   // Check for NULL  
			cout << "Null pointer!" << endl;
			return;
		}

		if (size + 1 >= max_size) {
			max_size *= 2;
			tmp = new T[max_size];
			for (int i = 0; i < size; i++)
				tmp[i] = elem[i];
			tmp[size++] = *t;  // Dereference  
			delete[] elem;
			elem = tmp;
		}
		else
			elem[size++] = *t; // Dereference  
	}

	void print() 
	{
		for (int i = 0; i < size; i++)
			cout << elem[i] << " ";
		cout << endl;
	}
};

int main() {
	Bag<int> xi;
	Bag<char> xc;
	Bag<int*> xp; // Uses partial specialization for pointer types.  

	xi.add(10);
	xi.add(9);
	xi.add(8);
	xi.print();

	xc.add('a');
	xc.add('b');
	xc.add('c');
	xc.print();

	int i = 3, j = 87, *p = new int[2];
	*p = 8;
	*(p + 1) = 100;
	xp.add(&i);
	xp.add(&j);
	xp.add(p);
	xp.add(p + 1);
	p = NULL;
	xp.add(p);
	xp.print();
}
#endif