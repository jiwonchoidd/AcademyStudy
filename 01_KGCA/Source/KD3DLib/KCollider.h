#pragma once

#include "KObject.h"
//Ŭ���� �������� �ߺ��̵Ǿ ����̾���
//��������� ����
//enum vs enum class
enum KCollisionType
{
	Block = 0,
	Overlap,
	Ignore,
};
enum KSelectType
{
	Select_Block = 0,
	Select_Overlap,
	Select_Ignore,
};
enum KSelectState // ���콺 ������ ���°�
{
	S_DEFAULT	= 0, // �⺻ ����, Ŀ���� ���� ������
	S_HOVER		= 1, // Ŀ���� ���� ������
	S_FOCUS		= 2, // S_ACTIVE ���¿��� �ٸ� ���� ��������
	S_ACTIVE	= 4, // ���콺 ���� ��ư ������ ������
	S_SELECTED	= 8, // S_ACTIVE ���Ŀ� ���� ������ ���� ��ư ��������
};
struct Rt_Size
{
	float width;
	float height;
};
class KCollider : public KObject
{
public:
	int					m_ID;
	DWORD				m_CollisonType;

public:
	bool				m_bSelect;
	int					m_SelectID;
	DWORD				m_SelectType;
	DWORD				m_SelectState;
public:
	KRect				m_rtColl;
	KVector2			m_pos;
	KVector2			m_dir;
	Rt_Size				m_rtSize; //������ ����ü
public:
	virtual void	ObjectOverlap(KCollider* pObj, DWORD dwState);
	virtual void	SelectOverlap(KCollider* pObj, DWORD dwState);
public:

	KCollider()
	{
		m_rtSize = { 0,0 };
		m_ID = -1;
		m_SelectID = -1;
		m_pos = {0,0};
		m_bSelect = false;
		m_CollisonType = Overlap;
		m_SelectType = Select_Overlap;
		m_SelectState= S_DEFAULT;
	}
};

