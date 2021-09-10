#include "TDemoObjMgr.h"
#include "TSoundMgr.h"

bool TDemoObjMgr::Init()
{
	m_ObjModel_A.Load(_T("../../data/bitmap1.bmp"), _T("../../data/bitmap2.bmp"));
	m_ObjModel_B.Load(_T("../../data/bitmap1.bmp")	_T("../../data/bitmap2.bmp"));
	m_ObjEffect_A.Load(_T("../../data/bitmap1.bmp"),_T("../../data/bitmap2.bmp"));
	// 배경	
	RECT rt;
	// 오브젝트
	rt.left = 90;
	rt.top = 1;
	rt.right = 42;
	rt.bottom = 60; 
	m_ObjModel_A.SetSourceRect(400, 300, rt);
	m_ObjModel_A.Init();

	rt.left = 46;
	rt.top = 62;
	rt.right = 68;
	rt.bottom = 79; 
	m_ObjModel_B.SetSourceRect(100, 300, rt);
	m_ObjModel_B.Init();

	rt.left = 119;
	rt.top = 142;
	rt.right = 35;
	rt.bottom = 45; 
	m_ObjEffect_A.SetSourceRect(400, 400, rt);
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
bool TDemoObjMgr::Frame()
{		
	return true;
}
bool TDemoObjMgr::Render()
{
	return true;
}
bool TDemoObjMgr::Release()
{
	for( int iObj=0; iObj < m_ObjectList.size(); iObj++ )
	{
		m_ObjectList[iObj].Release();
	}
	return true;
}

TDemoObjMgr::TDemoObjMgr(void)
{
}

TDemoObjMgr::~TDemoObjMgr(void)
{
}
