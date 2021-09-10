#pragma once
#include <iostream>
//// const 
//int dataA = 1;
//int dataB = 2;
//int dataC = 3;
//int dataD = 4;
	// 상수 데이터, 비상수 포인터
	// a 포인터 주소에 값을 변경할 수 있다.
//////////////// 중요 ////////////////////
// 값(오른쪽에 const붙어 있으면) <- *(기준) -> 주소(왼쪽에 const붙어 있으면)
////////////////////////////////////
//const int* a = &dataA;
//// a 포인터 주소에 값을 변경할 수 있다.
//a = &dataB; // 가능
//*a = 9;   // 불가능
//const int const* a1 = &dataA;
//a1 = &dataC; // 가능
//*a1 = 9;     // 불가능
//int const* a2 = &dataA;
//a2 = &dataC; // 가능
//*a2 = 9;     // 불가능
//int* const const a3 = &dataA;
//a3 = &dataD; // 불가능
//*a3 = 9;     // 가능
//int* const a4 = &dataA;
//a4 = &dataD; // 불가능
//*a4 = 9;     // 가능
//int const * const a5 = &dataA;
//a5 = &dataD; // 불가능
//*a5 = 9;     // 불가능
// void a() const {} //  함수에서 멤버 변수 값 변경 불가능
// const 함수에서는 const 함수만 호출가능.


class TRegion
{
public:
	enum { POINT_POINT, POINT_SIZE };	// 시작점과 끝점, 시작점과 길이
	int m_iWidth;
	int m_iHeight;
private:
	/////////////////////////////////////////////////////////////
	// 사각형 영역을 저장하기 위한 멤버변수들
	int m_iLeft;				// 시작점의 x좌표
	int m_iTop;				// 시작점의 y좌표
	int m_iRight;				// 끝점의 x좌표
	int m_iBottom;				// 끝점의 y좌표
	/////////////////////////////////////////////////////////////
	// 출력방식을 지정하기 위한 멤버변수
	static int notation;	// POINT_POINT 또는 POINT_SIZE
public:
	const TRegion* Get() const { return this; }
	const TRegion& GetRef() const { return *this; }
	/////////////////////////////////////////////////////////////
	// 사각형 영역에 대한 정보를 얻는 함수들
	const int GetLeft() { return m_iLeft; }	
	const int GetTop()  { return m_iTop; }
	const int* GetRight() { return &m_iRight; }
	const int& GetBottom() { return m_iBottom; }

	int GetWidth() const;						// 가로길이 얻기	
	const int GetHeight() const;				// 세로길이 얻기
	void GetStartPoint(int &x, int &y) const;	// 시작점 얻기
	void GetEndPoint(int &x, int &y) const;		// 끝점 얻기
	void GetCenterPoint(int &x, int &y) const;	// 중심점 얻기
	bool IsPointInRegion(int x, int y) const;	// 점이 영역 안에 있는지
	/////////////////////////////////////////////////////////////
	// 사각형 영역을 설정하고 처리하는 함수들
	void SetRect(int l, int t, int r, int b);	// 영역 설정
	void Move(int x, int y);					// 위치 이동
	void Resize(int width, int height);			// 크기 변경
	void UnionRegion(const TRegion &r1, const TRegion &r2);		// 합집합
	bool IntersectRegion(const TRegion &r1, const TRegion &r2);	// 교집합
	/////////////////////////////////////////////////////////////
	// 사각형 영역을 표시하기 위한 함수들
	void Show(void);						// 영역 출력
	static void SetNotation(int notation);	// 출력 방식 지정
public:
	TRegion(void);			// 생성자
	TRegion(int l, int t, int r, int b);	// 생성자
};

