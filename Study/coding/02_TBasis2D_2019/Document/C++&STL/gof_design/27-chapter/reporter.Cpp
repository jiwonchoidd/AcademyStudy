#include <iostream>
#include "reporter.h"
#include "account.h"
using namespace std;

void
TotalSumReporter::VisitPassbookAccount(PassbookAccount* pAccount) {
  cout << pAccount->GetCustomerName() 
        << "´ÔÀÇ PassbookAccount Total Sum : " 
        << pAccount->GetTotalSum() << endl;
}

void
TotalSumReporter::VisitCheckingAccount(CheckingAccount* pAccount) {
  cout << pAccount->GetCustomerName() 
        << "´ÔÀÇ CheckingAccount Total Sum : " 
        << pAccount->GetTotalSum() << endl;
}

// ----------------------------------------------------------------
void
HistoryReporter::VisitPassbookAccount(PassbookAccount* pAccount) {
  cout << pAccount->GetCustomerName() 
        << "´ÔÀÇ PassbookAccount ÀÔÃâ±Ý ³»¿ª : " << endl;
  list<int> inOutHist = pAccount->GetHistory();
  list<int>::iterator iter;
  for (iter=inOutHist.begin(); iter != inOutHist.end(); iter++) 
    cout << *iter << endl;
  cout << endl;
}

void
HistoryReporter::VisitCheckingAccount(CheckingAccount* pAccount) {
  cout << pAccount->GetCustomerName() 
        << "´ÔÀÇ CheckingAccount ÀÔÃâ±Ý ³»¿ª : " << endl;
  list<int> inOutHist = pAccount->GetHistory();
  list<int>::iterator iter;
  for (iter=inOutHist.begin(); iter != inOutHist.end(); iter++) 
    cout << *iter << endl;
  cout << endl;
}
