// 14장. C++연산자오버로딩
#include "TPoint.h"
void (TPoint::*fp) ();
void Execute(TPoint* the)
{
	if (fp != NULL)
	{
		(the->*fp)();
	}
}
int main(void)
{
	TPoint p1(0, 0), p2, p3, p4;
	p2 = ++p1;	// 전위형. operator++() 호출
	p1.Show();
	p2.Show();

	p1->Show();
	(*p1).Show();
	TPoint* pData = new TPoint();
	pData->Show();

	p3 = p1++;	// 후위형. operator++(int) 호출
	p1.Show();
	p3.Show();

	p4 = p1 + p2;
	p4.Show();
	
	TPoint p5(10, 20), p6, p7, p8;
	p6 = p5 * 2;		// 멤버함수 호출 
	p6.Show();
	p6 = 2 * p5;		// friend 함수 호출
	p6.Show();

	p7 = p5 / 2;		// 멤버함수 호출 
	p7.Show();
	p8 = 2 / p5;		// friend 함수 호출
	p8.Show();

	p5 += p4;		 
	p5.Show();
	p6 -= p5;		
	p6.Show();

	p5 *= 2;
	p5.Show();
	p6 /= 2;
	p6.Show();

	p5 *= p4;
	p5.Show();
	p6 /= p4;
	p6.Show();

	if (p5 == p6) cout << "같다.";
	if (p5 != p6) cout << "다르다.";

	cout << p6[0] << endl;		// x좌표
	cout << p6[1] << endl;		// y좌표
	cout << p6(0) << endl;		// x좌표
	cout << p6(1) << endl;		// y좌표


	TPoint a, b, c;
	cout << "첫번째  입력: ";
	cin >> a ;					// >> 연산자 호출
	cout << "두번째  입력: ";
	cin >> b;					// >> 연산자 호출

	cout << "a = " << a << endl;		// << 연산자 호출
	cout << "b = " << b << endl;		// << 연산자 호출
	cout << "a+b = " << a + b << endl;	// + 연산자 호출
	cout << "a-b = " << a - b << endl;	// - 연산자 호출
	cout << "a*b = " << a * b << endl;	// * 연산자 호출
	cout << "a/b = " << a / b << endl;	// / 연산자 호출

	TPoint* d = new TPoint();
	d->SetPosition(10, 10);
	cout << *d;	

	// 전역 함수포인터
	fp = &TPoint::Show;
	Execute(d); // 함수로 실행

	//클래스멤버함수 함수포인터
	d->m_fp = &TPoint::Show;
	(*d) > &a; // 연산자 오버로딩으로 실행
	d->Execute(); // 클래스함수로 실행
	delete d;
	return 0;
}
