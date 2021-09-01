#include <iostream>
#include "mediator.h"
#include "billbox.h"
using namespace std;

BillBox::BillBox() {
  pMediator_ = NULL;
  billCnt_ = 0;
  mode_ = BILL_ACCEPT_MODE;
}

void 
BillBox::SetMediator(Mediator *pMediator) { pMediator_ = pMediator; }

void
BillBox::InsertBill(int wantedCupCnt) {
  if (! IsValid() || pMediator_->IsCoinLack() || 
      pMediator_->IsBillFull() || mode_ == BILL_REJECT_MODE) {
    RejectBill();
    return;
  }

  billCnt_++;

  if (wantedCupCnt > 10) wantedCupCnt = 10;
  int servedCnt = pMediator_->TakeOutCup(wantedCupCnt);
  pMediator_->PayOutCoin(10-servedCnt);

  if (wantedCupCnt > servedCnt)
    cout << "Sorry, No More Served" << endl;

  if (billCnt_ >= MAX_BILL_CNT)
    pMediator_->InformBillState(BILL_FULL);
}

void
BillBox::RejectBill() {
  cout << "Sorry, Can't Accept Bill now" << endl;
}

void
BillBox::TakeOutBill(int takeOutCnt) {
  if (! IsValid()) return;

  billCnt_ -= takeOutCnt;
  if (billCnt_ < MAX_BILL_CNT)
    pMediator_->InformBillState(BILL_NOT_FULL);
}

void
BillBox::SetBillBoxMode(int mode) { mode_ = mode; }

bool
BillBox::IsValid() {
  if (pMediator_ == NULL) return false;
  else return true;
}
