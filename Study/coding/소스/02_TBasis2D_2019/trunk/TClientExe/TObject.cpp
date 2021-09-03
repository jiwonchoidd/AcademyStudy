#include "TObject.h"
#include "TInput.h"
#include "TBasisSys.h"
#include "TImageMgr.h"
#include "TSound.h"

bool TObject::Init()
{		
	// Left
	m_rtLeftAnim[0].left = 46;
	m_rtLeftAnim[0].top = 62;
	m_rtLeftAnim[0].right = 68;
	m_rtLeftAnim[0].bottom = 79; 
	//Right
	m_rtRightAnim[0].left = 46;
	m_rtRightAnim[0].top = 62;
	m_rtRightAnim[0].right = 68;
	m_rtRightAnim[0].bottom = 79; 

	m_rtCollision.left = m_fPosX;
	m_rtCollision.top  = m_fPosY;
	m_rtCollision.right = m_rtLeftAnim[0].right;
	m_rtCollision.bottom = m_rtLeftAnim[0].bottom;

	if( m_hBackScreen == NULL ) 
		return false;

	int iIndex = I_ImageMgr.Load( g_hWnd, m_hBackScreen->hDC, m_strColor );
	m_phColor  = I_ImageMgr.GetPtr( iIndex );

	if( m_strMask != _T("NULL") )
	{
		iIndex = I_ImageMgr.Load( g_hWnd, m_hBackScreen->hDC, m_strMask );
		m_phMask  = I_ImageMgr.GetPtr( iIndex );		
	}

	
	return true;
}
bool TObject::RenderMap()
{
	m_phColor->Draw( m_hBackScreen, m_fPosX, m_fPosY, m_Rect.right, m_Rect.bottom, 
							       m_Rect.left, m_Rect.top );
	return true;
}

bool TObject::RotateBlt()
{		
	// 원본 이미지의 가로 및 세로 영역의 최대값을 사용하여
	// 오프스크린 DC를 만든다.( 회전시 짤리는 영역이 없도록 )
	int iWidth		=	m_Rect.right;
	int iHeight		=	m_Rect.bottom;
	if(iWidth>iHeight) iHeight = iWidth;
	else		       iWidth = iHeight;

	HBITMAP hMaskBitmap = m_phMask->GetRotatedBitmap( m_fAngle, iWidth, iHeight, m_Rect);
	HBITMAP hColorBitmap = m_phColor->GetRotatedBitmap( m_fAngle, iWidth, iHeight, m_Rect);

	// 제2의 오프스크린을 제1의 오프스크린으로 드로우
	HDC m_hMaskDC = CreateCompatibleDC( m_hBackScreen->hDC);
	HDC m_hColorDC = CreateCompatibleDC( m_hBackScreen->hDC);
	HBITMAP hOldMaskBitmap = (HBITMAP)SelectObject( m_hMaskDC, hMaskBitmap );
	HBITMAP hOldColorBitmap = (HBITMAP)SelectObject( m_hColorDC, hColorBitmap );
	
			BitBlt( m_hBackScreen->hDC,m_fPosX, m_fPosY,iWidth, iHeight, m_hMaskDC, 0, 0, SRCAND );
			BitBlt( m_hBackScreen->hDC,m_fPosX, m_fPosY,iWidth, iHeight, m_hColorDC, 0, 0, SRCINVERT );
			BitBlt( m_hBackScreen->hDC,m_fPosX, m_fPosY,iWidth, iHeight, m_hMaskDC, 0, 0, SRCINVERT );
	
	SelectObject( m_hMaskDC, hOldMaskBitmap );
	SelectObject( m_hColorDC, hOldColorBitmap );
	DeleteDC( m_hMaskDC );
	DeleteDC( m_hColorDC );	

	DeleteObject( hMaskBitmap );
	DeleteObject( hColorBitmap );
	return true;
}
bool TObject::Render()
{
	static int iCount = 0;
	RECT rt = m_Rect;

	/*if( iCount % 50 == 0 )
		 rt = m_RectAnim;
	else
		rt = m_Rect;
	iCount++;*/
	// S & D
	// s = 0, s = 1
	// d = 0, d = 1
	// s = 1111 &  d( 1110 ) = 1110
	// s = 0000 &  d( 1110 ) = 0000
	m_phMask->Draw( m_hBackScreen, m_fPosX, m_fPosY, 
								rt.right, rt.bottom, 
							    rt.left, rt.top, 
								SRCAND );
	// S ^ D
	// d(0000)  , d(1011 )
	// s( ???? ), s(1111)
	// s( 1010 ) ^ d(0001) = 1010
	// s( 1110 ) ^ d(1011) = 0101

	m_phColor->Draw( m_hBackScreen, m_fPosX, m_fPosY, 
								rt.right, rt.bottom, 
							    rt.left, rt.top,
								SRCINVERT);
	// s( 0000 ) ^ d(0001) = 0001
	// s( 0000 ) ^ d(1110) = 1110
	// s( 0000 ) ^ d(1010) = 1010

	// s( 1111 ) ^ d(1001 ) = 0110
	// s( 1111 ) ^ d(0101 ) = 1010

	m_phMask->Draw( m_hBackScreen, m_fPosX, m_fPosY, 
								  rt.right, rt.bottom, 
							    rt.left, rt.top,
									SRCINVERT);
	

	return true;
}

bool TObject::Release()
{
	return true;
}
void TObject::MoveLeft()
{
	m_fPosX -= 100.0f * g_fSecPerFrame;
}
void TObject::MoveRight()
{
	m_fPosX += 100.0f * g_fSecPerFrame;
}
void TObject::MoveUp()
{
	m_fPosY -= 100.0f * g_fSecPerFrame;
}
void TObject::MoveDown()
{
	m_fPosY += 100.0f * g_fSecPerFrame;
}
bool TObject::Frame()
{	
	m_rtCollision.left = m_fPosX;
	m_rtCollision.top  = m_fPosY;
	m_rtCollision.right = m_Rect.right;
	m_rtCollision.bottom = m_Rect.bottom;
	return true;
}
TObject::TObject(void)
{
	m_hBackScreen = NULL;
	m_fPosX = m_fPosY = 0;
	m_fAngle = 0.0f;
}

TObject::~TObject(void)
{
}
