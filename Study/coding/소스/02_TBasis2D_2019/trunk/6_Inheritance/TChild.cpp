#include "TChild.h"


void TChild::Humanity(void)
{
	cout << "넘치는 인간미" << endl;
}
void TChild::Character(void)
{
	cout << "불 같은 성품" << endl;
}

void TChild::Appearance(void)
{
	TParent::Appearance();		// 기반 클래스에서 정의한 기능
	cout << "훤칠한 키" << endl; 	// 파생 클래스에서 확장된 기능
}

//TChild::TChild()
//{
//	cout << "Child 생성자" << endl;
//}
TChild::TChild(void) : TParent(1000)
{
	cout << "Child 생성자" << endl;
}


TChild::~TChild()
{
	cout << "Child 소멸자" << endl;
}
