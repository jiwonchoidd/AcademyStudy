#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <list>
#include <string>
using namespace std;

class Reporter;

class Account {
  public :
    Account(string n);
    virtual void Accept(Reporter& rpt)=0;
    virtual int GetTotalSum() = 0;
    list<int> GetHistory();
    string GetCustomerName();
    void Deposit(int inMoney);
    void Withdraw(int outMoney);
  protected :
    int netMoney_;
    list<int> history_; // -- 양수는 입금, 음수는 출금을 의미
    string customerName_;
};

class PassbookAccount : public Account {
  public :
    PassbookAccount(string n);
    void Accept(Reporter& rpt);
    int GetTotalSum();
};

class CheckingAccount : public Account {
  public :
    CheckingAccount(string n);
    void Accept(Reporter& rpt);
    int GetTotalSum();
};

#endif 
