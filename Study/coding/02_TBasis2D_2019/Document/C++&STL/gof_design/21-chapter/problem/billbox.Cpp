#include <iostream>
#include "lamp.h"
#include "mixer.h"
#include "coinbox.h"
#include "billbox.h"
using namespace std;

BillBox::BillBox() {
  pLamp_ = NULL;
  pMixer_ = NULL;
  pCoinBox_ = NULL;
  billCnt_ = 0;
  mode_ = BILL_ACCEPT_MODE;
}

void 
BillBox::SetLamp(Lamp *pLamp) { pLamp_ = pLamp; }

void 
BillBox::SetMixer(Mixer *pMixer) { pMixer_ = pMixer; }

void
BillBox::SetCoinBox(CoinBox *pCoinBox) { pCoinBox_ = pCoinBox; }

void
BillBox::InsertBill(int wantedCupCnt) {
  if (! IsValid() || pLamp_->IsCoinLack() || 
      pLamp_->IsBillFull() || mode_ == BILL_REJECT_MODE) {
    RejectBill();
    return;
  }

  billCnt_++;

  if (wantedCupCnt > 10) wantedCupCnt = 10;
  int servedCnt = pMixer_->TakeOut(wantedCupCnt);
  pCoinBox_->PayOutCoin(10-servedCnt);

  if (wantedCupCnt > servedCnt)
    cout << "Sorry, No More Served" << endl;

  if (billCnt_ >= MAX_BILL_CNT)
    pLamp_->TurnOnOffBillFull(LAMP_ON);
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
    pLamp_->TurnOnOffBillFull(LAMP_OFF);
}

void
BillBox::SetBillBoxMode(int mode) { mode_ = mode; }

bool
BillBox::IsValid() {
  if (pLamp_ == NULL || pMixer_ == NULL || pCoinBox_ == NULL)
    return false;
  else 
    return true;
}
