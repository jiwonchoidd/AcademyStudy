#pragma once
#include "TSpriteMgr.h"
#define MAX_PARTICLE	512

class TParticle 
{
public:
	tPOINT		m_vPos;				// ��ƼŬ ��ġ
	tPOINT		m_vAdd;				// ��ƼŬ ��ġ�� ������ ���� �������� ��
	float			m_fTimer;			// ����ú��� �����Ǵ� �ð�
	float			m_fLifeTime;		// �����ֱ�
	int				m_iApplyIndex;		// �ؽ�ó �ε���
	INT				m_iSpriteIndex;		// ��������Ʈ Ÿ�� �ε���
public:
	TParticle();
	~TParticle();
};