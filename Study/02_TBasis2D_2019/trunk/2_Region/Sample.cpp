#include "TRegion.h"
int main(void)
{
	TRegion::SetNotation(TRegion::POINT_POINT);	// 출력 형식 지정
	TRegion r1(10, 10, 100, 100);		// 영역 r1 선언
	TRegion r2(50, 50, 120, 120);		// 영역 r2 선언
	
	////////////////////  상수 객체 반환 ////////////////////////
	const TRegion* x1 = r1.Get();
	const TRegion& x2 = r2.GetRef();
	//TRegion* x1 = r1.Get();		// 오류!  비상수 포인터
	//TRegion& x2 = r2.GetRef();	// 오류!  비상수 참조
	int iX = x1->GetWidth();		// 정상!  비 상수 반환 & 상수 함수 호출
	int iT = x1->GetHeight();		// 정상!  상수 반환 & 상수 함수 호출
	const int iC = x1->GetWidth();	// 정상!  비 상수 반환 & 상수 함수 호출
	const int iD = x1->GetHeight();	// 정상!  상수 반환 & 상수 함수 호출
	//int iValue = x1->GetLeft();	// 오류!  비상수 함수 호출
	int iXref = x2.GetWidth();		// 정상!  상수 함수 호출
	//int iValueref = x2.GetLeft(); // 오류!  비상수 함수 호출
	///////////////////  상수 데이터 반환 ///////////////////////	
	int iY = r1.GetLeft();			// 정상! 비 상수 반환 함수 호출
	int iZ = r1.GetTop();			// 정상! 상수 반환 함수 호출
	const int iG = r1.GetLeft();	// 정상! 비 상수 반환 함수 호출
	const int iJ = r1.GetTop();		// 정상! 상수 반환 함수 호출
	//int* iP = r1.GetRight();		// 오류! 상수 반환 함수 호출
	const int* iQ = r1.GetRight();	// 정상! 상수 반환 함수 호출	
	//int& i6 = r1.GetBottom();     // 오류! 상수 반환 함수 호출
	const int& i7 = r1.GetBottom(); // 정상! 상수 반환 함수 호출

	///////////////////  영역 관리 함수 ///////////////////////	
	TRegion r3;
	// r1과 r2의 교집합을 r3에 저장
	r3.IntersectRegion(r1, r2);	r3.Show();
	// r1과 r2의 합집합을 r3에 저장
	r3.UnionRegion(r1, r2);		r3.Show();
	// r3의 위치 이동
	r3.Move(100, 100);			r3.Show();
	// r3의 크기 변경
	r3.Resize(200, 200);		r3.Show();

	TRegion::SetNotation(TRegion::POINT_SIZE);	// 출력 형식 변경
	r1.Show();
	r2.Show();
	r3.Show();
	return 0;
}
