/*
   SepciFuncTemplate2.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

template <typename T> // �Լ� ���ø� ����
int SizeOf(T a) 
{
	return sizeof(a);
}

template<> // �Լ� ���ø� ����
int SizeOf(char* a) 
{
	return strlen(a);
}

int main(void)
{
	int i=10;
	double e=7.7;
	char* str="Good morning!";

	cout<<SizeOf(i)<<endl;
	cout<<SizeOf(e)<<endl;
	cout<<SizeOf(str)<<endl;

	return 0;
}
