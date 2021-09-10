#include <iostream>
#include "lamp.h"
#include "coinbox.h"
#include "billbox.h"
#include "mixer.h"
using namespace std;

Mixer::Mixer() { 
  pLamp_ = NULL;
  pCoinBox_ = NULL;
  pBillBox_ = NULL;
  mixLevel_ = MAX_MIX_LEVEL;
  cupCnt_ = MAX_CUP_CNT;
}

void
Mixer::SetLamp(Lamp *pLamp) { pLamp_ = pLamp; }

void
Mixer::SetCoinBox(CoinBox *pCoinBox) { pCoinBox_ = pCoinBox; }

void
Mixer::SetBillBox(BillBox *pBillBox) { pBillBox_ = pBillBox; }

void 
Mixer::Refill(int addLevel) {
  int oldMixLevel = mixLevel_;
  mixLevel_ += addLevel;

  if (! IsValid()) return;

  if (oldMixLevel < MIN_MIX_LEVEL && 
      mixLevel_ >= MIN_MIX_LEVEL && cupCnt_ >= MIN_CUP_CNT) 
    pLamp_->TurnOnOffResourceLack(LAMP_OFF);
}

void 
Mixer::AddCup(int addCnt) { 
  int oldCupCnt = cupCnt_;
  cupCnt_ += addCnt;

  if (! IsValid()) return;

  if (mixLevel_ >= MIN_MIX_LEVEL && 
      oldCupCnt < MIN_CUP_CNT && cupCnt_ >= MIN_CUP_CNT) 
    pLamp_->TurnOnOffResourceLack(LAMP_OFF);
}

int 
Mixer::TakeOut(int wantedCupCnt) {
  if (! IsValid()) return 0;

  int servedCnt = 0;
  while (servedCnt < wantedCupCnt) {
    if (mixLevel_ < MIN_MIX_LEVEL || cupCnt_ < MIN_CUP_CNT) {
      pLamp_->TurnOnOffResourceLack(LAMP_ON);
      break;
    }
    mixLevel_--;
    cupCnt_--;
    servedCnt++;
  }

  return servedCnt;
}

void
Mixer::OutOfOrder() {
  pLamp_->TurnOnOffMixerOut(LAMP_ON);
  pCoinBox_->SetCoinBoxMode(COIN_REJECT_MODE);
  pBillBox_->SetBillBoxMode(BILL_REJECT_MODE);
}

void
Mixer::Repair() {
  pLamp_->TurnOnOffMixerOut(LAMP_OFF);
  pCoinBox_->SetCoinBoxMode(COIN_ACCEPT_MODE);
  pBillBox_->SetBillBoxMode(BILL_ACCEPT_MODE);
}

bool
Mixer::IsValid() {
  if (pLamp_ == NULL || pCoinBox_ == NULL || pBillBox_ == NULL)
    return false;
  else
    return true;
}
