#include "TObjectMgr.h"
#include "TBasisSys.h"
#include "TSound.h"

bool TObjectMgr::Init()
{
	HDC hScreenDC = GetDC( g_hWnd );
	m_hBackScreen.InitImage(g_hWnd, hScreenDC, m_iWidth, m_iHeight );
	
		m_ObjModel_A.SetBackScreen(&m_hBackScreen, _T("../../data/bitmap1.bmp"), _T("../../data/bitmap2.bmp"));
		m_ObjModel_B.SetBackScreen(&m_hBackScreen, _T("../../data/bitmap1.bmp"), _T("../../data/bitmap2.bmp"));
		m_ObjEffect_A.SetBackScreen(&m_hBackScreen, _T("../../data/bitmap1.bmp"), _T("../../data/bitmap2.bmp"));
		m_ObjMap_A.SetBackScreen(&m_hBackScreen, _T("../../data/석양.bmp"),_T("NULL") );

	ReleaseDC( g_hWnd, hScreenDC );	
	
	// 배경
	RECT rt ;
	rt.left = 0;
	rt.top = 0;
	rt.right = m_rtClient.right;
	rt.bottom = m_rtClient.bottom; 
	m_ObjMap_A.SetRect(rt);
	m_ObjMap_A.Init();

	// 오브젝트
	rt.left = 90;
	rt.top = 1;
	rt.right = 42;
	rt.bottom = 60; 
	m_ObjModel_A.SetRect(rt);
	m_ObjModel_A.MoveObject( 400, 300 );
	m_ObjModel_A.Init();

	rt.left = 46;
	rt.top = 62;
	rt.right = 68;
	rt.bottom = 79; 
	m_ObjModel_B.SetRect(rt);
	m_ObjModel_B.MoveObject( 100, 300 );
	m_ObjModel_B.Init();

	rt.left = 119;
	rt.top = 142;
	rt.right = 35;
	rt.bottom = 45; 
	m_ObjEffect_A.SetRect(rt);
	m_ObjEffect_A.MoveObject( 0, 0 );
	m_ObjEffect_A.Init();

	m_ObjectList.push_back( m_ObjModel_A );
	m_ObjectList.push_back( m_ObjModel_B );
	
	m_EffectList.push_back( m_ObjEffect_A );

	//
	/*if( I_SoundMgr.Init() )
	{
		m_iIndex[0] = I_SoundMgr.Load("../../data/OnlyLove.mp3");		
		TSound* pSound = I_SoundMgr.GetPtr( m_iIndex[0] );
		pSound->SetMode();	
		pSound->Play(I_SoundMgr.m_pSystem );

		m_iIndex[1] = I_SoundMgr.Load("../../data/OnlyLove.mp3");
		m_iIndex[2] = I_SoundMgr.Load("../../data/Gun1.wav");
		m_iIndex[3] = I_SoundMgr.Load("../../data/Gun2.wav");
	}*/
	return true;
}
bool TObjectMgr::Frame()
{	
	GetCursorPos(&m_CursorPos);
	ScreenToClient(g_hWnd, &m_CursorPos);

	m_ObjectList[0].m_fAngle += 45.0f * g_fSecPerFrame;
	m_ObjectList[1].m_fAngle += 45.0f * g_fSecPerFrame;
	
	// 오브젝트 충돌 처리
	m_bCollision = false;
	if( Collision(m_ObjectList[0], m_ObjectList[1]) )
	{
		TSound* pSound = I_SoundMgr.GetPtr( m_iIndex[3] );
		pSound->Play(I_SoundMgr.m_pSystem );
		m_bCollision = true;
	}
	if( Collision( m_ObjectList[0], m_CursorPos) )
	{
		TSound* pSound = I_SoundMgr.GetPtr( m_iIndex[2] );
		pSound->Play(I_SoundMgr.m_pSystem );
		m_bCollision = true;
	}

	for( int iObj=0; iObj < m_ObjectList.size(); iObj++ )
	{
		m_ObjectList[iObj].Frame();
	}
	return true;
}
bool TObjectMgr::Render()
{
	if( !m_bCollision )	m_ObjMap_A.RenderMap();
		
		for( int iObj=0; iObj < m_ObjectList.size(); iObj++ )
		{
			if( m_bCollision && iObj == 2 )
			{
				m_ObjectList[iObj].MoveObject( m_ObjectList[0].m_rtCollision.left,
					m_ObjectList[0].m_rtCollision.top);
				m_ObjectList[iObj].RotateBlt();	
			}
			else
			{
				m_ObjectList[iObj].RotateBlt();
			}
		}		
		return true;
}
bool TObjectMgr::Release()
{
	for( int iObj=0; iObj < m_ObjectList.size(); iObj++ )
	{
		m_ObjectList[iObj].Release();
	}
	m_ObjMap_A.Release();
	m_hBackScreen.Release();
	return true;
}
bool TObjectMgr::Collision(TObject& pSrc, POINT ptPos)
{
	// 마우스 위치와 RECT
	if( pSrc.m_rtCollision.left <= ptPos.x &&
		pSrc.m_rtCollision.left+pSrc.m_rtCollision.right >= ptPos.x )
	{
		if( pSrc.m_rtCollision.top <= ptPos.y &&
			pSrc.m_rtCollision.top+pSrc.m_rtCollision.bottom >= ptPos.y )
		{
			return true;
		}
	}
	return false;
}
bool TObjectMgr::Collision(TObject& pSrc, TObject& pDesc)
{
	bool bColWidth = false;
	bool bColHeight= false;
	int iDistanceWidth;
	// pSrc가 오른쪽
	if( pSrc.m_rtCollision.left > pDesc.m_rtCollision.left )
	{
			iDistanceWidth = 
				(pSrc.m_rtCollision.left + pSrc.m_rtCollision.right )- 
				pDesc.m_rtCollision.left;

	}
	else
	{
		iDistanceWidth = 
			(pDesc.m_rtCollision.left + pDesc.m_rtCollision.right )- 
				pSrc.m_rtCollision.left;
	}

	int iMaxWidth = pSrc.m_rtCollision.right + 
					pDesc.m_rtCollision.right;
	if( iMaxWidth > iDistanceWidth )
	{
		bColWidth = true;
	}

	// 세로
	int iDistanceHeight;
	// pSrc가 밑에 위치
	if( pSrc.m_rtCollision.top > pDesc.m_rtCollision.top )
	{
			iDistanceHeight = 
				(pSrc.m_rtCollision.top + pSrc.m_rtCollision.bottom )- 
				pDesc.m_rtCollision.top;

	}
	else
	{
		iDistanceHeight = 
			iDistanceHeight = 
				(pDesc.m_rtCollision.top + pDesc.m_rtCollision.bottom )- 
				pSrc.m_rtCollision.top;
	}

	int iMaxHeight = pSrc.m_rtCollision.bottom + 
					 pDesc.m_rtCollision.bottom;
	if( iMaxHeight > iDistanceHeight )
	{
		bColHeight = true;
	}
	
	if( bColWidth && bColHeight )
	{
		return true;
	}
	
	return false;
}
TObjectMgr::TObjectMgr(void)
{
	m_bCollision	= false;
}

TObjectMgr::~TObjectMgr(void)
{
}
