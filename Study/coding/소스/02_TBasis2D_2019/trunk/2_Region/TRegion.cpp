#include "TRegion.h"

int TRegion::notation = TRegion::POINT_POINT;

TRegion::TRegion(void)
{
	SetRect(0, 0, 0, 0);	// 영역을 0으로 초기화
}
TRegion::TRegion(int l, int t, int r, int b)
{
	SetRect(l, t, r, b);	// 영역을 주어진 값으로 초기화
}

void TRegion::SetRect(int l, int t, int r, int b)
{
	m_iLeft = l;			// 시작점의 x좌표 설정
	m_iTop = t;			// 시작점의 y좌표 설정
	m_iRight = r;			// 끝점의 x좌표 설정
	m_iBottom = b;			// 끝점의 y좌표 설정
}
int TRegion::GetWidth() const
{
	return m_iRight - m_iLeft;
}
const int TRegion::GetHeight() const
{
	return m_iBottom - m_iTop;
}
void TRegion::GetStartPoint(int &x, int &y) const
{
	x = m_iLeft;		// 시작점의 x 좌표 얻기
	y = m_iTop;		// 시작점의 y좌표 얻기
}
void TRegion::GetEndPoint(int &x, int &y) const
{
	x = m_iRight;		// 끝점의 x 좌표 얻기
	y = m_iBottom;		// 끝점의 y좌표 얻기
}
void TRegion::GetCenterPoint(int &x, int &y) const
{
	x = (m_iLeft + m_iRight) / 2;	// 중심점의 x 좌표 얻기
	y = (m_iTop + m_iBottom) / 2;	// 중심점의 y좌표 얻기
}

bool TRegion::IsPointInRegion(int x, int y) const
{
	return (x >= m_iLeft && x <= m_iRight && y >= m_iTop && y <= m_iBottom);
}
void TRegion::Move(int x, int y) {
	m_iLeft += x;			// 시작점의 x좌표 이동
	m_iRight += x;			// 끝점의 x좌표 이동
	m_iTop += y;			// 시작점의 y좌표 이동
	m_iBottom += y;		// 끝점의 y좌표 이동
}
void TRegion::Resize(int width, int height)
{
	int x, y;
	GetCenterPoint(x, y);	// 중심점 좌표 얻기
	m_iLeft = x - width / 2;	// 중심으로부터 가로길이의 절반만큼 이동
	m_iTop = y - height / 2;	// 중심으로부터 세로길이의 절반만큼 이동
	m_iRight = m_iLeft + width;	// 가로길이가 width가 되도록 조절
	m_iBottom = m_iTop + height;	// 세로길이가 height가 되도록 조절
}
void TRegion::UnionRegion(const TRegion &r1, const TRegion &r2)
{
	m_iLeft = r1.m_iLeft < r2.m_iLeft ? r1.m_iLeft : r2.m_iLeft;
	m_iRight = r1.m_iRight > r2.m_iRight ? r1.m_iRight : r2.m_iRight;
	m_iTop = r1.m_iTop < r2.m_iTop ? r1.m_iTop : r2.m_iTop;
	m_iBottom = r1.m_iBottom > r2.m_iBottom ? r1.m_iBottom : r2.m_iBottom;	
}
bool TRegion::IntersectRegion(const TRegion &r1, const TRegion &r2)
{
	m_iLeft = r1.m_iLeft > r2.m_iLeft ? r1.m_iLeft : r2.m_iLeft;
	m_iRight = r1.m_iRight < r2.m_iRight ? r1.m_iRight : r2.m_iRight;
	m_iTop = r1.m_iTop > r2.m_iTop ? r1.m_iTop : r2.m_iTop;
	m_iBottom = r1.m_iBottom < r2.m_iBottom ? r1.m_iBottom : r2.m_iBottom;
	// 한점 또는 라인이 중첩되었을 때 교차로 판정하지 않았다. 
	// 교집합이 존재하지 않는 경우
	if (m_iLeft >= m_iRight || m_iTop >= m_iBottom)	  
	{
		m_iLeft = m_iTop = m_iRight = m_iBottom = 0;
		return false;
	}
	return true;	
}
void TRegion::SetNotation(int n)
{
	notation = n;
}

void TRegion::Show(void)
{
	if (notation == POINT_POINT)	// 시작점/끝점 형식
	{
		printf("(%d, %d), (%d, %d)\n", m_iLeft, m_iTop, m_iRight, m_iBottom);
	}
	else					// 시작점/가로, 세로/길이 형식
	{
		printf("(%d, %d), [%d x %d]\n", m_iLeft, m_iTop, GetWidth(), GetHeight());
	}
}
