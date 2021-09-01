/*
	oo2.cpp
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

void Deposit(Account &acc, int money)  // �Ա�
{
	acc.balance+=money;
}

void Withdraw(Account &acc, int money) // ��� 
{
	acc.balance-=money;
}

int main(void)
{
	Account yoon={"1234", "2321", "yoon", 1000};

	Deposit(yoon, 100);
	cout<<"��    �� : "<<yoon.balance<<endl;

	Withdraw(yoon, 200);
	cout<<"��    �� : "<<yoon.balance<<endl;

	return 0;
}