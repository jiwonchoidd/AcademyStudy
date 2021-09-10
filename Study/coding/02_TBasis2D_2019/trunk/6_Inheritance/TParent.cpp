#include "TParent.h"
TParent::TParent(void) {
	money = 1000000000;	// 십억 원
	cout << "Parent 생성자" << endl;
}
TParent::TParent(int money)
{
	this->money = money;
	cout << "Parent 생성자" << endl;
}

TParent::~TParent(void) {
	cout << "Parent 소멸자" << endl;
}
void TParent::Character(void) {
	cout << "차분한 성품" << endl;
}
void TParent::Appearance(void) {
	cout << "잘생긴 외모" << endl;
}
void TParent::Wealth(void) {
	cout << "재산: " << money << "원" << endl;
}
