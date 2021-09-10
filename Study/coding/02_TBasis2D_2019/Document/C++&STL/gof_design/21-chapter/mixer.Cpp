#include <iostream>
#include "mediator.h"
#include "mixer.h"
using namespace std;

Mixer::Mixer() { 
  pMediator_ = NULL;
  mixLevel_ = MAX_MIX_LEVEL;
  cupCnt_ = MAX_CUP_CNT;
}

void
Mixer::SetMediator(Mediator *pMediator) { pMediator_ = pMediator; }

void 
Mixer::Refill(int addLevel) {
  int oldMixLevel = mixLevel_;
  mixLevel_ += addLevel;

  if (! IsValid()) return;

  if (oldMixLevel < MIN_MIX_LEVEL && 
      mixLevel_ >= MIN_MIX_LEVEL && cupCnt_ >= MIN_CUP_CNT) 
    pMediator_->InformResourceState(RESOURCE_LACK);
}

void 
Mixer::AddCup(int addCnt) { 
  int oldCupCnt = cupCnt_;
  cupCnt_ += addCnt;

  if (! IsValid()) return;

  if (mixLevel_ >= MIN_MIX_LEVEL && 
      oldCupCnt < MIN_CUP_CNT && cupCnt_ >= MIN_CUP_CNT) 
    pMediator_->InformResourceState(RESOURCE_OK);
}

int 
Mixer::TakeOut(int wantedCupCnt) {
  if (! IsValid()) return 0;

  int servedCnt = 0;
  while (servedCnt < wantedCupCnt) {
    if (mixLevel_ < MIN_MIX_LEVEL || cupCnt_ < MIN_CUP_CNT) {
      pMediator_->InformResourceState(RESOURCE_LACK);
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
  pMediator_->InformMixerState(MIXER_OUT_OF_ORDER);
}

void
Mixer::Repair() {
  pMediator_->InformMixerState(MIXER_OK);
}

bool
Mixer::IsValid() {
  if (pMediator_ == NULL) return false;
  else return true;
}
