#include <iostream>
using namespace std;
//static_cast 의미없는 형변환 방지.
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
	// 반드시 가상 테이블이 있어야 dynamic_cast은 사용가능하다.
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
	D* pd2 = static_cast<D*>(pb);   // 안전하지 않다.
	B* pb2 = static_cast<B*>(pd);   // 안전하다.(Safe conversion)
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
	// BClass로 케스팅되는 방법을 알려준다.
	operator BClass();
};
AClass::operator BClass()
{
	// 멤버초기화
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

	// 상속관계가 없어도 케스팅 된다.(불안전)
	AClass * pa = (AClass*)&c;
	// 상속관계가 없으면 컴파일 오류가 된다.
	//pa = static_cast<AClass*>(&c); // 

	// static_cast 는 형변환 방법을 알면 변환한다.
	// 그러나 방법을 모르면 알려줘야 한다.
	c = static_cast<BClass>(a);
	int k = c.m_fValue;

}