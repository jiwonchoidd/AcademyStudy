#include <iostream>
#include <list>
#include <string>
#include "account.h"
#include "reporter.h"
using namespace std;

void DoReport(list<Account *>& accountList, Reporter& rpt) {
  list<Account *>::iterator iter;
  for (iter=accountList.begin(); iter != accountList.end(); iter++)
    (*iter)->Accept(rpt);
}

void 
main()
{
  PassbookAccount a("¿µÈñ"), b("Ã¶¼ö");
  CheckingAccount c("¾Æºü"), d("¾ö¸¶");

  list<Account *> accountList;
  accountList.push_front(&a);
  accountList.push_front(&b);
  accountList.push_front(&c);
  accountList.push_front(&d);

  TotalSumReporter sumRpt;
  HistoryReporter  histRpt;

  a.Deposit(1000); 
  b.Deposit(2000);
  c.Deposit(3000);
  d.Deposit(4000);

  DoReport(accountList, sumRpt);
  cout << endl;

  a.Withdraw(500);
  c.Withdraw(1000);

  DoReport(accountList, histRpt);
  cout << endl;
  DoReport(accountList, sumRpt);
}
