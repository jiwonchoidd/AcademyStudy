/*
   Debug_new.cpp
*/
#include <iostream>

//#define DEBUG 1;
#define DEBUG  0;

using std::cin;
using std::cout;
using std::endl;

int main(void)
{
	int size;
	cout<<"�Ҵ��ϰ��� �ϴ� �迭�� ũ��: ";
	cin>>size;

	int* arr=new int[size];  // �迭�� ���� �Ҵ�.

#if DEBUG==1 
	cout<<"����� ��� �Դϴ�"<<endl;
	if(arr==NULL)
	{
		cout<<"�޸� �Ҵ� ����"<<endl;
		return -1; //���α׷� ����.
	}
#endif

	for(int i=0; i<size; i++)
		arr[i]=i+10;

	for(int j=0; j<size; j++)
		cout<<"arr["<<j<<"]= "<<arr[j]<<endl;

    delete []arr;  // �Ҵ�� �޸� �Ҹ�.

	return 0;
}
