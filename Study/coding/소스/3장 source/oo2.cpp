/*
	oo2.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

struct Account {
	char accID[20];    // 계좌 번호 
	char secID[20];    // 비밀 번호 
	char name[20];     // 이     름
	int balance;       // 잔     액 
};

void Deposit(Account &acc, int money)  // 입금
{
	acc.balance+=money;
}

void Withdraw(Account &acc, int money) // 출금 
{
	acc.balance-=money;
}

int main(void)
{
	Account yoon={"1234", "2321", "yoon", 1000};

	Deposit(yoon, 100);
	cout<<"잔    액 : "<<yoon.balance<<endl;

	Withdraw(yoon, 200);
	cout<<"잔    액 : "<<yoon.balance<<endl;

	return 0;
}