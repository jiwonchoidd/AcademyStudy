#include "Racoon.h"

CRacoon::CRacoon(void)
{
}

CRacoon::~CRacoon(void)
{
}
void CRacoon::Init()
{
	state = 1;			//�ʱ����� ������ ���� �ִ� 
	x = 775;			//�ʱ����� ���� ��ġ 
	y = 578;			//�ʱ����� ���� ��ġ 
	speedx = 5;			//�ʱ��� �̵� �ӵ� 
	speedy = 5;			//�ʱ��� �̵� �ӵ� 
}