#include <iostream>
#include "lamp.h"
#include "mixer.h"
#include "billbox.h"
#include "coinbox.h"
using namespace std;

CoinBox::CoinBox() {
  pLamp_ = NULL;
  pMixer_ = NULL;
  pBillBox_ = NULL;
  coinCnt_ = 10; 
  mode_ = COIN_ACCEPT_MODE;
}

void
CoinBox::SetLamp(Lamp *pLamp) { pLamp_ = pLamp; }

void
CoinBox::SetMixer(Mixer *pMixer) { pMixer_ = pMixer; }

void 
CoinBox::SetBillBox(BillBox *pBillBox) { pBillBox_ = pBillBox; }

void 
CoinBox::InsertCoin() {
  if (! IsValid() || pLamp_->IsResourceLack() || 
      pLamp_->IsCoinFull() || mode_ == COIN_REJECT_MODE) {
    RejectCoin();
    return;
  }

  coinCnt_++;
  pMixer_->TakeOut();

  if (coinCnt_ == MIN_COIN_CNT) 
    pLamp_->TurnOnOffCoinLack(LAMP_OFF);
  if (coinCnt_ >= MAX_COIN_CNT) 
    pLamp_->TurnOnOffCoinFull(LAMP_ON);
}

void 
CoinBox::RejectCoin() {
  cout << "Sorry, Can't Accept Coin now" << endl;
}

void 
CoinBox::PayOutCoin(int payOutCnt) {
  if (!IsValid()) return;

  coinCnt_ -= payOutCnt;
  if (coinCnt_ < MIN_COIN_CNT) 
    pLamp_->TurnOnOffCoinLack(LAMP_ON);
  if (coinCnt_ + payOutCnt >= MAX_COIN_CNT) 
    pLamp_->TurnOnOffCoinFull(LAMP_OFF);
}

void
CoinBox::SetCoinBoxMode(int mode) { mode_ = mode; }

bool
CoinBox::IsValid() {
  if (pLamp_ == NULL || pMixer_ == NULL)
    return false;
  else
    return true;
}
