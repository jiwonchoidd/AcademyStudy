#include "TObject.h"
#include "TCoreSys.h"
#include "TImageMgr.h"

HBITMAP TObject::RotateBitmap( HDC hdc, float fAngle, int iSizeWidth, int iSizeHeight)
{
	// 회전 될 DC 및 비트맵 생성
	HDC BackScreenDC = CreateCompatibleDC(hdc);
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc,iSizeWidth,iSizeHeight);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject( BackScreenDC, hbmResult );

	// 화면 체움
	HBRUSH hbrBack = CreateSolidBrush( RGB( 255,255,255 ));
	HBRUSH hbrOld  = (HBRUSH)SelectObject( BackScreenDC, hbrBack );
		PatBlt(BackScreenDC, 0, 0, iSizeWidth, iSizeHeight, PATCOPY);
	DeleteObject( SelectObject(BackScreenDC, hbrOld ) );

	// 월드 좌표계 변환
	SetGraphicsMode( BackScreenDC, GM_ADVANCED );
	float fRadian = fAngle * 3.141592  / 180.0f;
	float fCosine = cos( fRadian );
	float fSine = sin( fRadian );

	XFORM xform;
	xform.eM11 = fCosine;  //  10  0 
	xform.eM12 = fSine;    //  0  10
	xform.eM21 = -fSine;
	xform.eM22 = fCosine;
	xform.eDx = (iSizeWidth / 2.0f);
	xform.eDy = (iSizeHeight / 2.0f);
	SetWorldTransform( BackScreenDC, &xform );
						
	BitBlt( BackScreenDC,  
		//0,0,
		-(m_DeskRect.right/2), 
		-(m_DeskRect.bottom/2), 		
		m_DeskRect.right, m_DeskRect.bottom, 
							hdc, 
							m_SrcRect.left, 
							m_SrcRect.top, SRCCOPY);

	// 소멸
	SelectObject(BackScreenDC, hbmOldDest );
	DeleteObject(BackScreenDC);
	return hbmResult;
}
bool TObject::Init()
{
	return true;
}
void TObject::SetPos( float iX, float iY )
{
	m_fPosition[0] = iX;
	m_fPosition[1] = iY;

	int xStep = m_SrcRect.right / 2;
	int yStep = m_SrcRect.bottom / 2;

	int left = m_fPosition[0]-xStep;
	int right = m_fPosition[0]+xStep;
	int top = m_fPosition[1]-yStep;
	int bottom = m_fPosition[1]+yStep;

	//if( m_bCollide == false )
	{
		m_DeskRect.left = left;
		m_DeskRect.top  = top; 
		m_DeskRect.right  = m_SrcRect.right;
		m_DeskRect.bottom = m_SrcRect.bottom;
	}

	m_rtCollision.left   =  m_DeskRect.left;
	m_rtCollision.top    =  m_DeskRect.top;
	m_rtCollision.right  =	right;
	m_rtCollision.bottom = 	bottom;
}
void TObject::SetObjDC(HDC hdc)
{
	m_hDrawDC = hdc;
	//m_Image.SetDC( hdc );
}
void TObject::CreateOffScreenDC(HDC hdc, int iWidth, int iHeight)
{
	m_hDrawDC = CreateCompatibleDC(hdc);
	HBITMAP hbit = CreateCompatibleBitmap( hdc, 
		iWidth,
		iHeight);
	m_hOldBitmap = (HBITMAP)SelectObject( m_hDrawDC, hbit );
	
	//m_Image.SetDC( hdc );
}
bool TObject::Load( TCHAR* strLoadFile, 
				    TCHAR* strMaskLoadFile)
{
	if( strLoadFile != NULL )
	{
		m_iIndex = I_ImageMgr.Load(g_hWnd,
			m_hDrawDC, strLoadFile);
		if( m_iIndex <= 0  ) return false;
		m_pImage = I_ImageMgr.GetPtr(m_iIndex);
	}
	if( strMaskLoadFile != NULL )
	{
		m_iIndexMask = I_ImageMgr.Load(g_hWnd,
			m_hDrawDC, strMaskLoadFile);
		if( m_iIndexMask <= 0 ) return false;
		m_pImageMask = I_ImageMgr.GetPtr(m_iIndexMask);
	}
	return true;	
}
bool TObject::Frame()
{
	this->m_AlphaBlend.BlendOp = AC_SRC_OVER;
    this->m_AlphaBlend.BlendFlags = 0;
	this->m_AlphaBlend.SourceConstantAlpha = m_iAlpha;
    this->m_AlphaBlend.AlphaFormat = AC_SRC_OVER;

	if( m_bRotation )
	{
		m_fAngle += (timeGetTime() % 100 ) * g_fSecPerFrame;
	}
	return true;
}
bool TObject::Render()
{
	if( m_bRotation )
	{
		int iWidth = m_DeskRect.right;
		int iHeight = m_DeskRect.bottom;
		if( iWidth > iHeight ) iHeight = iWidth;
		else                   iWidth = iHeight;

		HBITMAP hMaskBimap = RotateBitmap(m_pImageMask->m_hMemDC, m_fAngle, iWidth, iHeight );
		HBITMAP hBimap = RotateBitmap(m_pImage->m_hMemDC,m_fAngle, iWidth, iHeight);

		DrawBitmap(m_hDrawDC, 
								m_DeskRect.left,  m_DeskRect.top,
								iWidth, iHeight,
								0, 0,
								hMaskBimap,SRCAND);	
		DrawBitmap(m_hDrawDC,   m_DeskRect.left,  m_DeskRect.top,
								iWidth, iHeight,							
								0, 0, hBimap,SRCINVERT);
		DrawBitmap(m_hDrawDC, m_DeskRect.left,  m_DeskRect.top,
								iWidth, iHeight,
								0, 0,
								hMaskBimap,	SRCINVERT);


		DeleteObject(hMaskBimap);
		DeleteObject(hBimap);
		return true;
	}
	
	if( m_pImageMask == NULL || m_pImageMask->m_hBitmap == NULL )
	{
		DrawBitmap(m_hDrawDC, 0, 0,
		                 800, 600,
						 0, 0,
						 m_pImage->m_hBitmap );
	}
	else
	{
		Draw(m_hDrawDC, 
							m_DeskRect.left,  m_DeskRect.top,
							m_DeskRect.right, m_DeskRect.bottom,
							m_pImageMask->m_hMemDC,m_SrcRect.left, m_SrcRect.top,
							SRCAND);	
		Draw(m_hDrawDC, m_DeskRect.left,  m_DeskRect.top,
		                    m_DeskRect.right, m_DeskRect.bottom,
							m_pImage->m_hMemDC,m_SrcRect.left, m_SrcRect.top,
								SRCINVERT);
		Draw(m_hDrawDC, m_DeskRect.left,  m_DeskRect.top,
		                    m_DeskRect.right, m_DeskRect.bottom,
							m_pImageMask->m_hMemDC,m_SrcRect.left, m_SrcRect.top,
								SRCINVERT);	
		//R2_XORPEN    //XOR 연산으로 겹치는 부분만 반전처리
		//R2_NOTXORPEN //XOR 결과의 반대값   
		//SetROP2(m_hDrawDC, R2_NOTXORPEN);    
		//Rectangle(m_hDrawDC, m_rtCollision.left, m_rtCollision.top, 
        //            m_rtCollision.right, m_rtCollision.bottom); 	
	}
	return true;
}
bool TObject::Release()
{
	ReleaseDC( g_hWnd, m_hDrawDC );
	return true;
}
//void TObject::DrawBitmap(	int iX, int iY,
//							int iWidth, int iHeight,
//							int iStartX, int iStartY,
//							HBITMAP bitmap,
//							DWORD rop)
//{
//	// 화면 DC와 호환가능한 DC를 사용
//	// 메모리 DC
//	HDC MemDC = CreateCompatibleDC(hdc);
//		// 저장
//		HBITMAP OldBitmap = (HBITMAP)SelectObject( MemDC, bitmap );
//			
//			BitBlt( m_hDrawDC, iX, iY, iWidth, iHeight,// B로
//					MemDC, iStartX, iStartY, rop ); //A에서
//		// 복원
//		SelectObject( MemDC, OldBitmap);
//	// 소멸
//	DeleteDC( MemDC );	
//}
void TObject::Draw(	HDC hdc, 
				int iX, int iY,
				int iWidth, int iHeight,
				HDC srchdc,
				int iStartX, int iStartY,						
				DWORD rop)
{
	//BitBlt( hdc, iX, iY, iWidth, iHeight,// B로
	//		srchdc, iStartX, iStartY, rop ); //A에서
	StretchBlt(hdc, iX, iY, iWidth, iHeight,
		       srchdc, iStartX, iStartY,iWidth, iHeight, rop);
	/*TransparentBlt(
		hdc, iX, iY, iWidth, iHeight,
		srchdc, iStartX, iStartY,iWidth, iHeight, 
		RGB(255,255,255));*/
	/*AlphaBlend(hdc, iX, iY, iWidth, iHeight,
		srchdc, iStartX, iStartY,iWidth, iHeight,
		m_AlphaBlend);*/

}
void TObject::DrawBitmap(	HDC hdc, 
							int iX, int iY,
							int iWidth, int iHeight,
							int iStartX, int iStartY,
							HBITMAP bitmap,
							DWORD rop)
{
	// 화면 DC와 호환가능한 DC를 사용
	// 메모리 DC
	HDC MemDC = CreateCompatibleDC(hdc);
		// 저장
		HBITMAP OldBitmap = (HBITMAP)SelectObject( MemDC, bitmap );
			
			BitBlt( hdc, iX, iY, iWidth, iHeight,// B로
					MemDC, iStartX, iStartY, rop ); //A에서
		// 복원
		SelectObject( MemDC, OldBitmap);
	// 소멸
	DeleteDC( MemDC );	
}
TObject::TObject(void)
{
	m_pImageMask = NULL;
	m_pImage	 = NULL;
	m_iAlpha	 = 255;
	m_fPosition[0] = 0.0f;
	m_fPosition[1] = 0.0f;
	m_hDrawDC		= NULL;
	m_hOldBitmap= NULL;
	SetRect( &m_SrcRect, 0, 0, 0, 0 );
	SetRect( &m_DeskRect, 0, 0, 0, 0 );
	m_bCollide	= false;
	m_bRotation = false;
	m_fAngle	= 0.0f;
}


TObject::~TObject(void)
{
}
