#include <iostream>
#include "lamp.h"
#include "mixer.h"
#include "coinbox.h"
#include "billbox.h"
#include "mediator.h"
using namespace std;

Mediator::Mediator(Lamp *pLamp, Mixer *pMixer, 
                    CoinBox *pCoinBox, BillBox *pBillBox)
  : pLamp_(pLamp), pMixer_(pMixer), pCoinBox_(pCoinBox), pBillBox_(pBillBox) {}

bool
Mediator::IsResourceLack() { return pLamp_->IsResourceLack(); }

bool
Mediator::IsCoinLack() { return pLamp_->IsCoinLack(); }

bool
Mediator::IsCoinFull() { return pLamp_->IsCoinFull(); }

bool
Mediator::IsBillFull() { return pLamp_->IsBillFull(); }

void
Mediator::InformMixerState(int state) {
  if (state == MIXER_OUT_OF_ORDER) {
    pLamp_->TurnOnOffMixerOut(LAMP_ON);
    pCoinBox_->SetCoinBoxMode(COIN_REJECT_MODE);
    pBillBox_->SetBillBoxMode(BILL_REJECT_MODE);
  }
  else if (state == MIXER_OK) {
    pLamp_->TurnOnOffMixerOut(LAMP_OFF);
    pCoinBox_->SetCoinBoxMode(COIN_ACCEPT_MODE);
    pBillBox_->SetBillBoxMode(BILL_ACCEPT_MODE);
  }
  else {}
}

void
Mediator::InformResourceState(int state) {
  if (state == RESOURCE_LACK)
    pLamp_->TurnOnOffResourceLack(LAMP_ON);
  else if (state == RESOURCE_OK)
    pLamp_->TurnOnOffResourceLack(LAMP_OFF);
  else {}
}

void
Mediator::InformCoinState(int state) {
  if (state == COIN_LACK)
    pLamp_->TurnOnOffCoinLack(LAMP_ON);
  else if (state == COIN_NOT_LACK)
    pLamp_->TurnOnOffCoinLack(LAMP_OFF);
  else if (state == COIN_FULL)
    pLamp_->TurnOnOffCoinFull(LAMP_ON);
  else if (state == COIN_NOT_FULL)
    pLamp_->TurnOnOffCoinFull(LAMP_OFF);
  else {}
}

void
Mediator::InformBillState(int state) {
  if (state == BILL_FULL)
    pLamp_->TurnOnOffBillFull(LAMP_ON);
  else if (state == BILL_NOT_FULL)
    pLamp_->TurnOnOffBillFull(LAMP_OFF);
  else {}
}

int
Mediator::TakeOutCup(int takeOutCnt) {
  return pMixer_->TakeOut(takeOutCnt);
}

void
Mediator::PayOutCoin(int payOutCnt) {
  pCoinBox_->PayOutCoin(payOutCnt);
}
