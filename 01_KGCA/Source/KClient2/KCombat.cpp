#include "KCombat.h"
//�� ���ϸ��� �����
//�÷��̾� ���ϸ� ������ �´� ������ 
//���ϸ� ������ ���� ���ͷ�
//�������� ����
bool KCombat::Init(ID3D11DeviceContext* context, KPlayer2D* player, int id = -1)
{
	m_pContext = context;
	m_pPlayer = player;
	PoketmonInfo mypoket=m_pPlayer->m_inven.poketmon_list.front();
	mypoket.mon_rect

	{
		m_Name = L"enemy";
		switch (id)
		{
			case 0:
			{

			}break;
			case 1:
			{

			}break;
			case 2:
			{

			}break;
			case 3:
			{

			}break;
		default:
			break;
		}
	}
	
	return true;
}

bool KCombat::Frame()
{
	return true;
}

bool KCombat::Render(ID3D11DeviceContext* pContext)
{
	return true;
}

bool KCombat::Release()
{
	return false;
}

void KCombat::ObjectOverlap(KCollider* pObj, DWORD dwState)
{
}

KCombat::KCombat()
{
	m_enemy_info.mon_level = 0; //����
	m_enemy_info.mon_height= 0.0f;//Ű
	m_enemy_info.mon_weight = 0.0f;//������
	m_enemy_info.type=0; //Ÿ��
	m_enemy_info.sex=false; //����
	m_enemy_info.skill.clear();
	m_pPlayer = nullptr;
}

KCombat::~KCombat()
{
	m_enemy_info.mon_level = 0; //����
	m_enemy_info.mon_height = 0.0f;//Ű
	m_enemy_info.mon_weight = 0.0f;//������
	m_enemy_info.type = 0; //Ÿ��
	m_enemy_info.sex = false; //����
	m_enemy_info.skill.clear();
	m_pPlayer = nullptr;
}
