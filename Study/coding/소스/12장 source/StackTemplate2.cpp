/*
   StackTemplate2.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

template <typename T>
class Stack {
private:
    int topIdx;   // ������ �Էµ� ��ġ�� �ε���.
   T* stackPtr;  // ���� ������.
public:
	Stack(int s=10);
	~Stack();
	void Push(const T& pushValue);
	T Pop();
};

template <typename T>
Stack<T>::Stack(int len){
	topIdx=-1;		// ���� �ε��� �ʱ�ȭ.
	stackPtr=new T[len];	// ������ ���� ���� �迭 ����.
}

template <typename T>
Stack<T>::~Stack(){
	delete[] stackPtr;
}

template <typename T>
void Stack<T>::Push(const T& pushValue){ // ���ÿ� ������ �Է�.
	stackPtr[++topIdx]=pushValue;
}

template <typename T>
T Stack<T>::Pop(){ // ���ÿ��� ������ ����.
	return stackPtr[topIdx--];
}

int main()
{
	Stack<char> stack1(10);
	stack1.Push('A');
	stack1.Push('B');
	stack1.Push('C');

	for(int i=0; i<3; i++){
		cout<<stack1.Pop()<<endl;
	}

	Stack<int> stack2(10);
	stack2.Push(10);
	stack2.Push(20);
	stack2.Push(30);

	for(int j=0; j<3; j++){
		cout<<stack2.Pop()<<endl;
	}

	return 0;
}
