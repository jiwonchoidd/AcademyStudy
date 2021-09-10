//mediator는 '중재자', '조정자', '중개인'이란 뜻이다.
//M:N의 관계를 M:1로 단순화 한다.
//객체지향 설계에서는 많은 객체가 존재하고 이들이 서로 관계를 맺으며 상호작용한다.
//이때 객체의 수가 너무 많아지면 서로 통신하느라 복잡해져 객체지향에서 
//가장 중요한 느슨한 결합의 특성을 해칠 수 있다.
//이를 해결하는 한 가지 방법은 중간에 이를 통제하고 지시할 수 있는 역할을 하는 중재자를 두는 것이다.
//중재자에게 모든 것을 요구하여 통신의 빈도수를 줄여 
//객체지향의 목표를 달성하게 해주는 것이 바로 mediator 패턴이다

#include <iostream>
#include "lamp.h"
#include "mixer.h"
#include "coinbox.h"
#include "billbox.h"
#include "mediator.h"
using namespace std;

void main()
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
