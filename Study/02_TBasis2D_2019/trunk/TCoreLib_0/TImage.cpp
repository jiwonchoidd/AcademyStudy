#include "TImage.h"
#include "TCoreSys.h"

bool	TImage::Init()
{
	return true;
}

bool	TImage::Frame(){
	return true;
}
bool	TImage::Render(){
	return true;
}
bool	TImage::Release(){
	SelectObject( m_hMemDC, m_hOldBitmap );
	DeleteObject( m_hBitmap );
	DeleteDC( m_hMemDC );
	return true;
}

bool TImage::Load(	HWND hWnd, 
					HDC hdc,
					TCHAR_STRING fileName )
{
	m_hMemDC = CreateCompatibleDC( hdc );
	m_hBitmap =(HBITMAP)LoadImage(g_hInstance,
		fileName.c_str(), 
		IMAGE_BITMAP,
		0, 0, 
		LR_DEFAULTSIZE | LR_LOADFROMFILE );

	m_hOldBitmap = (HBITMAP)SelectObject(
				m_hMemDC,
				m_hBitmap );
	return true;
}
TImage::TImage(void)
{
	m_strName = _T("");
}


TImage::~TImage(void)
{
}
