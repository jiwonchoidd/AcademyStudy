/*
	oo3.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

struct Account {
	char accID[20];    // °èÁÂ ¹øÈ£ 
	char secID[20];    // ºñ¹Ð ¹øÈ£ 
	char name[20];     // ÀÌ     ¸§
	int balance;       // ÀÜ     ¾× 

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
	cout<<"ÀÜ    ¾× : "<<yoon.balance<<endl;

	yoon.Withdraw(200);
	cout<<"ÀÜ    ¾× : "<<yoon.balance<<endl;

	return 0;
}