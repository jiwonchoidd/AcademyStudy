//mediator�� '������', '������', '�߰���'�̶� ���̴�.
//M:N�� ���踦 M:1�� �ܼ�ȭ �Ѵ�.
//��ü���� ���迡���� ���� ��ü�� �����ϰ� �̵��� ���� ���踦 ������ ��ȣ�ۿ��Ѵ�.
//�̶� ��ü�� ���� �ʹ� �������� ���� ����ϴ��� �������� ��ü���⿡�� 
//���� �߿��� ������ ������ Ư���� ��ĥ �� �ִ�.
//�̸� �ذ��ϴ� �� ���� ����� �߰��� �̸� �����ϰ� ������ �� �ִ� ������ �ϴ� �����ڸ� �δ� ���̴�.
//�����ڿ��� ��� ���� �䱸�Ͽ� ����� �󵵼��� �ٿ� 
//��ü������ ��ǥ�� �޼��ϰ� ���ִ� ���� �ٷ� mediator �����̴�

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

	// -- 90 ���� ���� ����
	for (int i = 0; i < 90; i++)
		coinBox.InsertCoin();

	// -- ���� & ���� 
	mixer.OutOfOrder();
	coinBox.InsertCoin();
	billBox.InsertBill();
	mixer.Repair();

	billBox.InsertBill(8);
}
