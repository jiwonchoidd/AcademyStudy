#include <iostream>
#include "mediator.h"
#include "coinbox.h"
using namespace std;

CoinBox::CoinBox() {
  pMediator_ = NULL;
  coinCnt_ = 10; 
  mode_ = COIN_ACCEPT_MODE;
}

void
CoinBox::SetMediator(Mediator *pMediator) { pMediator_ = pMediator; }

void 
CoinBox::InsertCoin() {
  if (! IsValid() || pMediator_->IsResourceLack() || 
      pMediator_->IsCoinFull() || mode_ == COIN_REJECT_MODE) {
    RejectCoin();
    return;
  }

  coinCnt_++;
  pMediator_->TakeOutCup();

  if (coinCnt_ == MIN_COIN_CNT) 
    pMediator_->InformCoinState(COIN_NOT_LACK);
  if (coinCnt_ >= MAX_COIN_CNT) 
    pMediator_->InformCoinState(COIN_FULL);
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
    pMediator_->InformCoinState(COIN_LACK);
  if (coinCnt_ + payOutCnt >= MAX_COIN_CNT) 
    pMediator_->InformCoinState(COIN_NOT_FULL);
}

void
CoinBox::SetCoinBoxMode(int mode) { mode_ = mode; }

bool
CoinBox::IsValid() {
  if (pMediator_ == NULL) return false;
  else return true;
}
