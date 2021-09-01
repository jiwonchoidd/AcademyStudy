/*
   StackTemplate1.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class Stack {
private:
   int topIdx;   // ������ �Էµ� ��ġ�� �ε���.
   char* stackPtr;  // ���� ������.
public:
	Stack(int s=10);
	~Stack();
	void Push(const char& pushValue);
	char Pop();
};

Stack::Stack(int len){
	topIdx=-1;		// ���� �ε��� �ʱ�ȭ.
	stackPtr=new char[len];	// ������ ���� ���� �迭 ����.
}
Stack::~Stack(){
	delete[] stackPtr;
} 
void Stack::Push(const char& pushValue){ // ���ÿ� ������ �Է�.
	stackPtr[++topIdx]=pushValue;
}
char Stack::Pop(){ // ���ÿ��� ������ ����.
	return stackPtr[topIdx--];
}

int main()
{
	Stack stack(10);
	stack.Push('A');
	stack.Push('B');
	stack.Push('C');

	for(int i=0; i<3; i++){
		cout<<stack.Pop()<<endl;
	}

	return 0;
}
