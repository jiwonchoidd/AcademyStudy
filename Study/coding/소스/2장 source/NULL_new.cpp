/*
   NULL_new.cpp
*/
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main(void)
{
	int size;
	cout<<"할당하고자 하는 배열의 크기: ";
	cin>>size;

	int* arr=new int[size];  // 배열의 동적 할당.
	if(arr==NULL)
	{
		cout<<"메모리 할당 실패"<<endl;
		return -1; //프로그램 종료.
	}

	for(int i=0; i<size; i++)
		arr[i]=i+10;

	for(int j=0; j<size; j++)
		cout<<"arr["<<j<<"]= "<<arr[j]<<endl;

    delete []arr;  // 할당된 메모리 소멸.

	return 0;
}
