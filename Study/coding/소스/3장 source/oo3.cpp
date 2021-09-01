/*
	oo3.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

struct Account {
	char accID[20];    // ���� ��ȣ 
	char secID[20];    // ��� ��ȣ 
	char name[20];     // ��     ��
	int balance;       // ��     �� 

	void Deposit(int money){
		balance+=money;
	}
	void Withdraw(int money){
		balance-=money;
	}

};

int main(void)
{
	Account yoon={"1234", "2321", "yoon", 1000};

	yoon.Deposit(100);
	cout<<"��    �� : "<<yoon.balance<<endl;

	yoon.Withdraw(200);
	cout<<"��    �� : "<<yoon.balance<<endl;

	return 0;
}