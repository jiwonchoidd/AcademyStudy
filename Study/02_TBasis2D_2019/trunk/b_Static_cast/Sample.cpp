#include <iostream>
using namespace std;
//static_cast �ǹ̾��� ����ȯ ����.
typedef unsigned char BYTE;

void f() {
	char ch;
	int i = 65;
	float f = 2.5;
	double dbl;

	ch = static_cast<char>(i);   // int to char
	dbl = static_cast<double>(f);   // float to double
	i = static_cast<BYTE>(ch);
}

class B {
public:
	virtual void Test() {}
};
class D : public B {};

void f(B* pb) {
	// �ݵ�� ���� ���̺��� �־�� dynamic_cast�� ��밡���ϴ�.
	D* pd1 = dynamic_cast<D*>(pb);
	D* pd2 = static_cast<D*>(pb);
	if (pd1 == NULL)
		cout << "pd1 == NULL" << endl;
	else
		cout << "pd1 != NULL" << endl;
	if (pd2 == NULL)
		cout << "pd1 == NULL" << endl;
	else
		cout << "pd1 != NULL" << endl;
}
void f2(B* pb, D* pd) {
	D* pd2 = static_cast<D*>(pb);   // �������� �ʴ�.
	B* pb2 = static_cast<B*>(pd);   // �����ϴ�.(Safe conversion)
}
/////////////////////////////////////////////////////////////////////////////////
class BClass
{
public:
	float m_fValue;
};
class AClass
{
public:
	int m_iValue;
	// BClass�� �ɽ��õǴ� ����� �˷��ش�.
	operator BClass();
};
AClass::operator BClass()
{
	// ����ʱ�ȭ
	BClass fRet = { static_cast<float>(m_iValue) };
	return fRet;
};

void main()
{
	f();
	B b;
	f(&b);	
	/////////////////////////////////////////////////////
	AClass a;
	a.m_iValue = 99;
	BClass c;
	c.m_fValue = -1;

	// ��Ӱ��谡 ��� �ɽ��� �ȴ�.(�Ҿ���)
	AClass * pa = (AClass*)&c;
	// ��Ӱ��谡 ������ ������ ������ �ȴ�.
	//pa = static_cast<AClass*>(&c); // 

	// static_cast �� ����ȯ ����� �˸� ��ȯ�Ѵ�.
	// �׷��� ����� �𸣸� �˷���� �Ѵ�.
	c = static_cast<BClass>(a);
	int k = c.m_fValue;

}