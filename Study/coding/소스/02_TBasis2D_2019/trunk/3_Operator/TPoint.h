#pragma once
#include <iostream>
using namespace std;
// const char& operator []() const {}
// char& operator [] () 두 함수는 별개의 함수가 된다.

// 연산자 재정의 불가
//1)선택 연산자 : .
//2)포인터 선택 연산자 : .*
//3)영역 연산자 : ::
//4)조건 연산자 : ? :
//5)크기 연산자 : sizeof

class TPoint
{
public:
	int x, y;
	void SetPosition(int _x, int _y);
	void Move(int _x, int _y);
	void Show(void);
public:
	TPoint* operator->()  {
		return this;
	}
	TPoint& operator*()  {
		return *this;
	}
	TPoint operator++(void); // ++p1
	TPoint operator++(int);  // p1--;
	// 이항연산자 p3=p1 + p2
	TPoint operator+(const TPoint &pt) const;
	TPoint operator-(const TPoint &pt) const;
	TPoint operator*(const TPoint &c);
	TPoint operator/(const TPoint &c);

	TPoint operator*(int mag) const; // p2=p1*2;
	TPoint operator/(int div) const; // p2=p1/2;	

	friend TPoint operator*(int mag, const TPoint &pt); // p2= 2 * p1;
	friend TPoint operator/(int div, const TPoint &pt); // p2= 2 / p1;

	TPoint &operator=(const TPoint &pt);
	TPoint &operator+=(const TPoint &pt);
	TPoint &operator-=(const TPoint &pt);
	TPoint &operator*=(const TPoint &pt);
	TPoint &operator/=(const TPoint &pt);
	TPoint &operator*=(int mag);
	TPoint &operator/=(int div);
	
	bool operator==(const TPoint &pt) const;
	bool operator!=(const TPoint &pt) const;
	// 인덱싱
	int &operator[](int index); //p6[0]
	int &operator()(int index); //p6(0)

	void *operator new(size_t size);
	void operator delete(void *p);

	//std::cout << *pFindA << *pFindB;
	friend ostream &operator<<(ostream &os, const TPoint &pt);
	friend istream &operator>>(istream &is, TPoint &pt);

	void (TPoint::*m_fp) ();
	void Execute() {
		if (m_fp != NULL)
		{
			(this->*m_fp)();
		}
	}
	void operator > (TPoint* the)
	{
		if (m_fp != NULL)
		{
			(the->*m_fp)();
		}
	}
public:
	TPoint();
	TPoint(int x, int y);
	~TPoint();
};

