/*
	oo1.cpp
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

int main(void)
{
	Account yoon={"1234", "2321", "yoon", 1000};
	cout<<"계좌번호 : "<<yoon.accID<<endl;
	cout<<"비밀번호 : "<<yoon.secID<<endl;
	cout<<"이    름 : "<<yoon.name<<endl;
	cout<<"잔    액 : "<<yoon.balance<<endl;

	return 0;
}