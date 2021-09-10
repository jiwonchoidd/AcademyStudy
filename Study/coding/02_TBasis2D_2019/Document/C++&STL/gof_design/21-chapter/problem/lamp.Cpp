#include <iostream>			// added in VC++
#include "lamp.h"
using namespace std;

Lamp::Lamp() { 
  mixerOutLamp_ = resourceLackLamp_ = coinLackLamp_ = coinFullLamp_ = LAMP_OFF;
}

void 
Lamp::TurnOnOffMixerOut(int onOff) { mixerOutLamp_ = onOff; }

void
Lamp::TurnOnOffResourceLack(int onOff) { resourceLackLamp_ = onOff; }

void 
Lamp::TurnOnOffCoinLack(int onOff) { coinLackLamp_ = onOff; }

void 
Lamp::TurnOnOffCoinFull(int onOff) { coinFullLamp_ = onOff; }

void 
Lamp::TurnOnOffBillFull(int onOff) { billFullLamp_ = onOff; }

bool
Lamp::IsResourceLack() { 
  if (resourceLackLamp_ == LAMP_ON) return true;
  else return false; 
}

bool
Lamp::IsCoinLack() { 
  if (coinLackLamp_ == LAMP_ON) return true; 
  else return false; 
}

bool
Lamp::IsCoinFull() { 
  if (coinFullLamp_ == LAMP_ON) return true; 
  else return false; 
}

bool
Lamp::IsBillFull() { 
  if (billFullLamp_ == LAMP_ON) return true; 
  else return false; 
}
