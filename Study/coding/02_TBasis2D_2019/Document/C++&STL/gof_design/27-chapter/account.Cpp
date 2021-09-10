#include <math.h>
#include "account.h"
#include "reporter.h"
using namespace std;

#define lround(x)	((long)((double(x)) + 0.5))		// added in VC++

Account::Account(string n) { 
  customerName_ = n; 
  netMoney_ = 0;
}

list<int>
Account::GetHistory() { return history_; }

string
Account::GetCustomerName() { return customerName_; }

void
Account::Deposit(int inMoney) { 
  netMoney_ += inMoney;
  history_.push_back(inMoney);
}

void
Account::Withdraw(int outMoney) {
  netMoney_ -= outMoney;
  history_.push_back(-outMoney);
}

// ---------------------------------------
PassbookAccount::PassbookAccount(string n) : Account(n) {}

void
PassbookAccount::Accept(Reporter& rpt) {
  rpt.VisitPassbookAccount(this);
}

int
PassbookAccount::GetTotalSum() {
  return netMoney_; // -- 별도 이자 없음.
}

// ---------------------------------------
CheckingAccount::CheckingAccount(string n) : Account(n) {}

void
CheckingAccount::Accept(Reporter& rpt) {
  rpt.VisitCheckingAccount(this);
}

int
CheckingAccount::GetTotalSum() {
  return netMoney_ + lround(netMoney_ * 0.1); // -- 10% 이자
}
