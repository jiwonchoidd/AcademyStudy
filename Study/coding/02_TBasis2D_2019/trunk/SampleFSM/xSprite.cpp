#include "xSprite.h"
#include "TBitmapMgr.h"
void		xSprite::SetLife(float fAnimTime)
{
	m_fLifeTime = fAnimTime;
	m_fSecPerRender = fAnimTime / m_iNumBitmap;
}
void xSprite::SetAttribute( float fAnimTime, int iWidth, int iWidthSize, int iHeight, int iHeightSize )
{
	RectSet.left	= iWidth;
	RectSet.right	= iWidthSize; // 텍스쳐 가로 셋 갯수 및 크기	
	RectSet.top		= iHeight;
	RectSet.bottom	= iHeightSize; // 텍스쳐 세로 셋 갯수 및 크기

	if( m_iNumBitmap > 0  )
	{
		m_fSecPerRender = fAnimTime / m_iNumBitmap;
		return;
	}
	else
	{
		m_fSecPerRender = fAnimTime / (iWidth*iHeight);
		m_iNumBitmap	=  iWidth *  iHeight;	
	}

	for(int iHeight=0;iHeight<RectSet.left;iHeight++)
	{
		for( int iWidth=0;iWidth<RectSet.top;iWidth++)
		{
			TRectUV  tRect;
			tRect.Rect.left		=iWidth		* RectSet.right;
			tRect.Rect.right	=(iWidth+1) * RectSet.right;
			tRect.Rect.top		=iHeight	* RectSet.bottom;
			tRect.Rect.bottom	=(iHeight+1)* RectSet.bottom;
			m_RectList.push_back( tRect );
		}
	}	
}
RECT xSprite::SetUV(RECT& Rect)
{	
	RECT vUV;
	float fOffSetX = 0.0f;
	if( Rect.left > 0 )
	{
		fOffSetX = (float)Rect.left/ (float)(RectSet.left*RectSet.right);
	}	
	float fOffSetY = 0.0f;
	if( Rect.top > 0 )
	{
		fOffSetY = (float)Rect.top/(float)(RectSet.top*RectSet.bottom);
	}
	
	vUV.left = fOffSetX;
	vUV.top= fOffSetY;

	fOffSetX = 1.0f;
	if( Rect.right > 0 )
	{
		fOffSetX = (float)Rect.right/(float)(RectSet.left*RectSet.right);
	}	
	fOffSetY = 1.0f;
	if( Rect.bottom > 0 )
	{
		fOffSetY = (float)Rect.bottom/(float)(RectSet.top*RectSet.bottom) ;
	}
	vUV.right = fOffSetX;
	vUV.bottom = fOffSetY;
	return vUV;
}
void xSprite::SetRectArray(vector<RECT> rtArray, float fLifeTime)
{	
	for (int iList = 0; iList < rtArray.size(); iList++)
	{			
		TRectUV rt;
		rt.Rect = rtArray[iList];
		m_RectList.push_back(rt);
	}
	m_fLifeTime = fLifeTime;
	m_fSecPerRender = m_fLifeTime / m_RectList.size();
	m_rtObj = m_RectList[0].Rect;
	m_iNumBitmap = m_RectList.size();
}
void xSprite::SetBitmapArray( TCHAR_STRING_VECTOR FileList )
{
	HRESULT hr=S_OK;
	for( int iList = 0; iList < FileList.size(); iList++ )
	{
		INT iIndex = I_BMgr.Add( const_cast<TCHAR*>(FileList[iList].c_str()) );
		if( iIndex <= 0 )
		{
			MessageBox( 0, _T("m_Texture.Load 실패"), _T("Fatal error"), MB_OK );
			return;
		}
		m_BitmapArray.push_back( iIndex );
	}	
	m_iNumBitmap = m_BitmapArray.size();
}
bool xSprite::Init()
{	
	return true;
}
bool		xSprite::Release()
{
	DeleteObject(m_hBrush);
	SelectObject(m_pBitmap->m_hScreenDC, m_hOldBrush);
	return true;
};
void xSprite::SetPos(tPOINT pos)
{
	m_ObjPos = pos;
	RECT p;
	if (m_RectList.size())
	{
		p = m_RectList[m_iApplyIndex].Rect;
		m_rtStart.x = m_ObjPos.x - p.right / 2.0f;
		m_rtStart.y = m_ObjPos.y - p.bottom / 2.0f;
	}
	else
	{
		// 여러장의 비트맵
		m_rtStart.x = m_ObjPos.x;
		m_rtStart.y = m_ObjPos.y;
	}
}
// 스프라이트 타입 단위로 변경
void xSprite::Updata(tPOINT pos, float& pfCurrentTimer, int& iApplyIndex, float fGlobalTime, float fElapsedTime)
{	
	pfCurrentTimer += fElapsedTime;
	if( pfCurrentTimer >=  m_fSecPerRender )
	{
		if(++iApplyIndex>=m_iNumBitmap)
			iApplyIndex=0;		
		
		pfCurrentTimer = 0.0f;
	}	
	AnimationFrame(iApplyIndex);
	SetPos(pos);
}
void xSprite::AnimationFrame( int iApplyIndex)
{
	// 텍스처 에니메이션
	if( m_BitmapArray.size() )
	{
		if (m_bMask != false)
		{
			m_pBitmapMask = I_BMgr.GetPtr(m_BitmapArray[iApplyIndex]);
		}
		m_pBitmap = I_BMgr.GetPtr(m_BitmapArray[iApplyIndex]);
		m_rtCurrent.right = m_pBitmap->m_BmpInfo.bmWidth;
		m_rtCurrent.bottom = m_pBitmap->m_BmpInfo.bmHeight;
		m_rtCurrent.left = m_rtCurrent.top = 0;
	}
	else // UV 텍스처 에니메이션
	{
		if( m_RectList.size() )
		{
			m_rtCurrent = m_RectList[iApplyIndex].Rect;
		}
	}		
}
bool xSprite::Frame( float fGlobalTime, float fElapsedTime )
{	
	Updata(m_ObjPos, m_fTime, m_iApplyIndex, fGlobalTime, fElapsedTime);
	AnimationFrame( m_iApplyIndex);	
	return true;
}

bool		xSprite::Render()
{
	if (m_bMask == false)
	{
		m_pBitmap->Draw(m_hOffScreenDC,
			m_rtStart,
			m_rtCurrent,
			m_dwReverse,
			SRCCOPY);
		return true;
	}
	if (m_bMask)
	{
		m_pBitmapMask->Draw(m_hOffScreenDC,
			m_rtStart,
			m_rtCurrent,
			m_dwReverse,
			SRCAND);
		m_pBitmap->Draw(m_hOffScreenDC,
			m_rtStart,
			m_rtCurrent,
			m_dwReverse,
			SRCINVERT);
		m_pBitmapMask->Draw(m_hOffScreenDC,
			m_rtStart,
			m_rtCurrent,
			m_dwReverse,
			SRCINVERT);
	}


	return true;
};


xSprite::xSprite(void)
{
	m_fLifeTime		= 0.0f;
	m_fTime			= 0.0f;
	m_iApplyIndex	= 0;
	m_fSecPerRender = 0.2f;
	m_iNumBitmap	= 0;
	RectSet.left	= 4;
	RectSet.right	= 64; // 텍스쳐 가로 셋 갯수 및 크기	
	RectSet.top		= 4;
	RectSet.bottom	= 64; // 텍스쳐 세로 셋 갯수 및 크기
}

xSprite::~xSprite(void)
{
}
