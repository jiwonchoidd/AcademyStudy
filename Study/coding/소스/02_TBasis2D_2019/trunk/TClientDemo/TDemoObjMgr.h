#pragma once
#include "TObject.h"

class TDemoObjMgr: public TSingleton<TDemoObjMgr>
{
private:
	friend class TSingleton<TDemoObjMgr>;
public:
	TObject		m_ObjModel_A;
	TObject		m_ObjModel_B;
	TObject		m_ObjEffect_A;
	// 유저 모델 리소스( A, B )
	std::vector<TObject>	m_ObjectList;
	std::vector<TObject>	m_EffectList;
	int			m_iIndex[4];
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	TDemoObjMgr(void);
	~TDemoObjMgr(void);
};
#define I_DemoObjMgr TDemoObjMgr::GetInstance()