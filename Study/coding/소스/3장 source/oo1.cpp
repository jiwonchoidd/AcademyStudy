/*
	oo1.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

struct Account {
	char accID[20];    // ���� ��ȣ 
	char secID[20];    // ��� ��ȣ 
	char name[20];     // ��     ��
	int balance;       // ��     �� 
};

int main(void)
{
	Account yoon={"1234", "2321", "yoon", 1000};
	cout<<"���¹�ȣ : "<<yoon.accID<<endl;
	cout<<"��й�ȣ : "<<yoon.secID<<endl;
	cout<<"��    �� : "<<yoon.name<<endl;
	cout<<"��    �� : "<<yoon.balance<<endl;

	return 0;
}