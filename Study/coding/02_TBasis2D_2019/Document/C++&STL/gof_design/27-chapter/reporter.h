#ifndef REPORTER_H
#define REPORTER_H

class PassbookAccount;
class CheckingAccount;

class Reporter {
  public :
    virtual void VisitPassbookAccount(PassbookAccount* pAccount) = 0;
    virtual void VisitCheckingAccount(CheckingAccount* pAccount) = 0;
};

class TotalSumReporter : public Reporter {
  public :
    void VisitPassbookAccount(PassbookAccount* pAccount);
    void VisitCheckingAccount(CheckingAccount* pAccount);
};

class HistoryReporter : public Reporter {
  public :
    void VisitPassbookAccount(PassbookAccount* pAccount);
    void VisitCheckingAccount(CheckingAccount* pAccount);
};

#endif
