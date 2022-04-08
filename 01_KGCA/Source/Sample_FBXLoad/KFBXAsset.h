#pragma once
#include "KFbxLoader.h"
//KFBXOBJ�� ��Ƽ� Ʈ�� ������ �̷�� �ϳ��� FBX ������Ʈ
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
	//�ִϸ��̼� ������ �ٸ� �ִϸ��̼� ��ȯ�ÿ��� �����(������ ����)
public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render(ID3D11DeviceContext* pContext)override;
	virtual bool    Release() override;
};

