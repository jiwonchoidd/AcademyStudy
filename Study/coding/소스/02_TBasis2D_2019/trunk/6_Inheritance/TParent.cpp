#include "TParent.h"
TParent::TParent(void) {
	money = 1000000000;	// �ʾ� ��
	cout << "Parent ������" << endl;
}
TParent::TParent(int money)
{
	this->money = money;
	cout << "Parent ������" << endl;
}

TParent::~TParent(void) {
	cout << "Parent �Ҹ���" << endl;
}
void TParent::Character(void) {
	cout << "������ ��ǰ" << endl;
}
void TParent::Appearance(void) {
	cout << "�߻��� �ܸ�" << endl;
}
void TParent::Wealth(void) {
	cout << "���: " << money << "��" << endl;
}
