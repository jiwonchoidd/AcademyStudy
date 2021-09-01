#include <iostream>
using namespace std;

class TClass
{
public:
	int m_iValue;
	int* m_pPointer;
public:
	TClass& operator=(TClass &copy)
	{
		m_iValue = copy.m_iValue;
		return *this;
	}
	TClass(int iValue) : m_iValue(iValue)
	{
	}
	TClass(int* p) : m_pPointer(p)
	{
	}
	TClass(TClass& copy) //: m_iValue(copy.m_iValue)
	{
		m_iValue = copy.m_iValue;
	}
	~TClass(){}
};
class TClassExplicit
{
private:
	int m_iValue;
public:
	// explicit는 생성자 앞에 붙여서 묵시적인 호출을 방지한다.
	// 이는 명시적인 호출만 가능토록 한다.
	explicit TClassExplicit(int iValue) : m_iValue(iValue)
	{
	}
	 // explicit을 복사 생성자에서 사용하면 
	 // 대입 연산자를 통하여 객체의 생성 및 초기화를 할 수 없다.
	 explicit TClassExplicit(TClassExplicit& copy);
	~TClassExplicit() {}
};
TClassExplicit::TClassExplicit(TClassExplicit& copy) : m_iValue(copy.m_iValue)
{
	//m_iValue = copy.m_iValue;
}
class TClassMutable
{
private:
	int m_iValue;
	//mutable 은 const 함수에서 변경 가능해 진다.
	// 이는 const의 예외를 두는 결과가 된다.
	mutable int m_iData;
	static int m_iCnt;
public:
	// 함수가 const이지만 mutable는 값의 변경을 예외적으로 허용한다.
	int Get(int iValue) const
	{
		m_iData = iValue;
		m_iCnt++;
		return m_iData;
	}
	explicit TClassMutable(int iValue) : m_iValue(iValue)
	{
	}
	explicit TClassMutable(TClassMutable& copy) : m_iValue(copy.m_iValue)
	{}
	~TClassMutable() {}
};
int TClassMutable::m_iCnt = 0;

void CopyFun(TClass a)
{
	std::cout << a.m_iValue;
}
void main()
{
	int gValue = 4;
	TClass tX(nullptr);
	TClass tA(3);
	CopyFun(tA);
	TClass tB =3;
	TClass tC = tA;
	tC = tA;
	TClassExplicit tD(3);
	// 묵시적 변환이 발생하지 못한다.
	//TClassExplicit tE=3; // Error

	// 대입 연산자를 통하여 객체의 생성 및 초기화를 할 수 없다.
	//TClassExplicit tF = tD; // Error
	TClassExplicit pColp(tD); // Ok

	TClassMutable tG(3);	
	cout << tG.Get(5);
}