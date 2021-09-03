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
	// explicit�� ������ �տ� �ٿ��� �������� ȣ���� �����Ѵ�.
	// �̴� ������� ȣ�⸸ ������� �Ѵ�.
	explicit TClassExplicit(int iValue) : m_iValue(iValue)
	{
	}
	 // explicit�� ���� �����ڿ��� ����ϸ� 
	 // ���� �����ڸ� ���Ͽ� ��ü�� ���� �� �ʱ�ȭ�� �� �� ����.
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
	//mutable �� const �Լ����� ���� ������ ����.
	// �̴� const�� ���ܸ� �δ� ����� �ȴ�.
	mutable int m_iData;
	static int m_iCnt;
public:
	// �Լ��� const������ mutable�� ���� ������ ���������� ����Ѵ�.
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
	// ������ ��ȯ�� �߻����� ���Ѵ�.
	//TClassExplicit tE=3; // Error

	// ���� �����ڸ� ���Ͽ� ��ü�� ���� �� �ʱ�ȭ�� �� �� ����.
	//TClassExplicit tF = tD; // Error
	TClassExplicit pColp(tD); // Ok

	TClassMutable tG(3);	
	cout << tG.Get(5);
}