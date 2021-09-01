/*
   StackTemplate1.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class Stack {
private:
   int topIdx;   // 마지막 입력된 위치의 인덱스.
   char* stackPtr;  // 스택 포인터.
public:
	Stack(int s=10);
	~Stack();
	void Push(const char& pushValue);
	char Pop();
};

Stack::Stack(int len){
	topIdx=-1;		// 스택 인덱스 초기화.
	stackPtr=new char[len];	// 데이터 저장 위한 배열 선언.
}
Stack::~Stack(){
	delete[] stackPtr;
} 
void Stack::Push(const char& pushValue){ // 스택에 데이터 입력.
	stackPtr[++topIdx]=pushValue;
}
char Stack::Pop(){ // 스택에서 데이터 꺼냄.
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
