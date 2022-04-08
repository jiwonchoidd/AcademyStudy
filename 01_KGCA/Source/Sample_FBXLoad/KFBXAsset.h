#pragma once
#include "KFbxLoader.h"
//KFBXOBJ을 모아서 트리 구조를 이루는 하나의 FBX 오브젝트
class KFBXAsset : public K3DAsset
{
public:
	KFbxLoader*		m_pLoader;
	KFbxLoader*		m_pAnimLoader;
	float			m_fAnimDir = 1.0f;
	float			m_fAnimTime = 0.0f;
	float			m_fAnimSpeed = 1.0f;
	KBoneWorld				m_matBoneArray;
	std::vector<KFBXAsset>	m_DrawList;
	//애니메이션 보간은 다른 애니메이션 변환시에만 사용함(프레임 저하)
public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render(ID3D11DeviceContext* pContext)override;
	virtual bool    Release() override;
};

