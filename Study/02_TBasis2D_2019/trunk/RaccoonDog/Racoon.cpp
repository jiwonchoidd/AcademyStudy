#include "Racoon.h"

CRacoon::CRacoon(void)
{
}

CRacoon::~CRacoon(void)
{
}
void CRacoon::Init()
{
	state = 1;			//너구리는 정면을 보고 있다 
	x = 775;			//너구리의 시작 위치 
	y = 578;			//너구리의 시작 위치 
	speedx = 5;			//너구리 이동 속도 
	speedy = 5;			//너구리 이동 속도 
}