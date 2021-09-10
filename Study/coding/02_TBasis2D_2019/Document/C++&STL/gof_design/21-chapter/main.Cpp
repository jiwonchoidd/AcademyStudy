#include <iostream>
#include "lamp.h"
#include "mixer.h"
#include "coinbox.h"
#include "billbox.h"
#include "mediator.h"
using namespace std;

void
main()
{
  Lamp lamp;
  Mixer mixer;
  CoinBox coinBox;
  BillBox billBox;

  Mediator mediator(&lamp, &mixer, &coinBox, &billBox);
  mixer.SetMediator(&mediator);
  coinBox.SetMediator(&mediator);
  billBox.SetMediator(&mediator);

  // -- 90 명이 서비스 받음
  for (int i = 0; i < 90; i++)
    coinBox.InsertCoin();

  // -- 고장 & 수리 
  mixer.OutOfOrder();
  coinBox.InsertCoin();
  billBox.InsertBill();
  mixer.Repair();

  billBox.InsertBill(8);
}
