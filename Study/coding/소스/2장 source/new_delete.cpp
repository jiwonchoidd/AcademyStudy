/*
   new_delete.cpp
*/

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main(void)
{
	int size;
	cout<<"�Ҵ��ϰ��� �ϴ� �迭�� ũ��: ";
	cin>>size;

	int* arr=new int[size];  // �迭�� ���� �Ҵ�.

	for(int i=0; i<size; i++)
		arr[i]=i+10;

	for(int j=0; j<size; j++)
		cout<<"arr["<<j<<"]= "<<arr[j]<<endl;

    delete []arr;  // �Ҵ�� �޸� �Ҹ�.

	return 0;
}
