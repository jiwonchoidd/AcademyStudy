#include "Sample.h"


bool		Sample::Init()
{
	m_Bitmap.Init();
	m_Bitmap.Set(m_hScreenDC, IDB_BITMAP2);
	HBRUSH hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(
		this->m_hOffScreenDC, hBrush);
	return true;
};
bool		Sample::Render() 
{
	//HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	////// 현재 적용 상태 저장
	//HBRUSH hOldBrush = (HBRUSH)SelectObject(
	//	m_hOffScreenDC, hBrush);
	//// 사용
	//Ellipse(m_hOffScreenDC, 100, 100, 200, 200);
	//// 복원
	//SelectObject(m_hOffScreenDC, hOldBrush);

	RECT rt = {0, 0, 100, 100};
	rt.right = m_rtClient.right;
	rt.bottom = m_rtClient.bottom;
	m_Bitmap.Draw( m_hOffScreenDC, rt);
	return true;
};
bool		Sample::Release() 
{	
	m_Bitmap.Release();
	return true;
};

Sample::Sample()
{
}


Sample::~Sample()
{
}
