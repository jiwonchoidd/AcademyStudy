#pragma once
#include "TObject.h"

class TObjectMgr: public TSingleton<TObjectMgr>
{
private:
	friend class TSingleton<TObjectMgr>;
public:
	TObject		m_ObjModel_A;
	TObject		m_ObjModel_B;
	TObject		m_ObjEffect_A;
	TObject		m_ObjMap_A;
	TImage		m_hBackScreen;

	int			m_iWidth;
	int			m_iHeight;
	RECT		m_rtClient;
	POINT		m_CursorPos;


	// 유저 모델 리소스( A, B )
	std::vector<TObject>	m_ObjectList;
	std::vector<TObject>	m_EffectList;

	int			m_iIndex[4];
	bool		m_bCollision;	
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

	bool Collision(TObject& pSrc, TObject& pDesc);
	bool Collision(TObject& pSrc, POINT ptPos);

public:
	TObjectMgr(void);
	~TObjectMgr(void);
};
#define I_ObjectMgr TObjectMgr::GetInstance()