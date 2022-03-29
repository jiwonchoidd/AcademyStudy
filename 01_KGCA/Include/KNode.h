#pragma once
#include "K3DAsset.h"
struct KMapObject
{
	std::wstring	obj_name;
	KMatrix			obj_matWorld;//������Ʈ���� ��ġ�� �ٸ�
	KVector3		obj_pos;
	KBox			obj_box; // ������Ʈ ��ġ OBB AABB ����
	K3DAsset*		obj_pObject;
	KVector3		m_vRight;
	KVector3		m_vUp;
	KVector3		m_vLook;
	void		UpdateData()
	{
		m_vRight.x = obj_matWorld._11;
		m_vRight.y = obj_matWorld._12;
		m_vRight.z = obj_matWorld._13;
		m_vUp.x = obj_matWorld._21;
		m_vUp.y = obj_matWorld._22;
		m_vUp.z = obj_matWorld._23;
		m_vLook.x = obj_matWorld._31;
		m_vLook.y = obj_matWorld._32;
		m_vLook.z = obj_matWorld._33;

		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Normalize(&m_vLook, &m_vLook);
	}
	void		UpdateCollision()
	{
		obj_box.Axis[0] = m_vRight;
		obj_box.Axis[1] = m_vUp;
		obj_box.Axis[2] = m_vLook;

		// GenAABB();
		obj_box.min = KVector3(100000, 100000, 100000);
		obj_box.max = KVector3(-100000, -100000, -100000);
		for (int iV = 0; iV < 8; iV++)
		{
			KVector3 pos;
			D3DXVec3TransformCoord(&pos, &obj_box.List[iV], &obj_matWorld);
			if (obj_box.min.x > pos.x)
			{
				obj_box.min.x = pos.x;
			}
			if (obj_box.min.y > pos.y)
			{
				obj_box.min.y = pos.y;
			}
			if (obj_box.min.z > pos.z)
			{
				obj_box.min.z = pos.z;
			}

			if (obj_box.max.x < pos.x)
			{
				obj_box.max.x = pos.x;
			}
			if (obj_box.max.y < pos.y)
			{
				obj_box.max.y = pos.y;
			}
			if (obj_box.max.z < pos.z)
			{
				obj_box.max.z = pos.z;
			}
		}

		KVector3 vHalf = obj_box.max - obj_box.middle;
		obj_box.size.x = fabs(D3DXVec3Dot(&obj_box.Axis[0], &vHalf));
		obj_box.size.y = fabs(D3DXVec3Dot(&obj_box.Axis[1], &vHalf));
		obj_box.size.z = fabs(D3DXVec3Dot(&obj_box.Axis[2], &vHalf));
		obj_box.middle = (obj_box.min + obj_box.max);
		obj_box.middle /= 2.0f;
	}
};
class KNode
{
public:
	KNode*  m_pParent;
	KRect	m_rect;//����
	KBox    m_node_box;//�ݸ���
	int		m_index;
	int		m_data;
	int		m_depth;
public:
	KNode*					m_pChildlist[4]; //�ڽ� ���
	KNode*					m_pNeighborlist[4];// �̿� ���
	std::vector<DWORD>		m_CornerList;// ����� �ڳ�
	KVector3				m_Center;
	UINT					m_LodLevel;
	POINT					m_Element;				// ��ġ ����Ʈ
	bool					m_bLeaf;
	std::list<KMapObject*>  m_StaticObjectList;
	std::list<KMapObject*>  m_DynamicObjectList;
public:
	std::vector <DWORD>		    m_IndexList;
	std::vector <PNCT_VERTEX>	m_VertexList;
	wrl::ComPtr<ID3D11Buffer>	m_pVertexBuffer;
public:
	void   AddObject(KMapObject* obj);
	void   AddDynamicObject(KMapObject* obj);
public:
	bool isRect(KVector2 pos)
	{
		if (this->m_rect.min.x <= pos.x &&
			this->m_rect.max.x >= pos.x &&
			this->m_rect.min.y <= pos.y &&
			this->m_rect.max.y >= pos.y)
		{
			return true;
		}
		return false;
	}
	void SetRect(float x, float y, float z, float w, float h)
	{
		this->m_rect.size = KVector2(w, h);
		this->m_rect.min = KVector2(x, z);
		this->m_rect.max.x = this->m_rect.min.x + w;
		this->m_rect.max.y = this->m_rect.min.y - h;
		m_Center.x = x + (w / 2.0f);
		m_Center.y = y;
		m_Center.z = z - (h / 2.0f);
	}
	void SetBox(float x, float y, float z, float w, float h)
	{
		this->m_node_box.min.x = x;
		this->m_node_box.min.z = z;
		this->m_node_box.max.x = x+w;
		this->m_node_box.max.z = z;
		this->m_node_box.min.y = y;
		this->m_node_box.max.y = x;
		this->m_node_box.Axis[0] = KVector3(1, 0, 0);
		this->m_node_box.Axis[1] = KVector3(0, 1, 0);
		this->m_node_box.Axis[2] = KVector3(0, 0, 1);
		this->m_node_box.size.x = (this->m_node_box.max.x - this->m_node_box.min.x) / 2.0f;
		this->m_node_box.size.y = (this->m_node_box.max.y - this->m_node_box.min.y) / 2.0f;
		this->m_node_box.size.z = (this->m_node_box.max.z - this->m_node_box.min.z) / 2.0f;
		this->m_node_box.middle = (this->m_node_box.max + this->m_node_box.min);
		this->m_node_box.middle /= 2.0f;
	}
public:
	KNode()
	{
		m_pParent = nullptr;
		m_index = 0;
		m_data = 0;
		m_depth = 0;
		m_Element = { 0,0 };				// ��ġ ����Ʈ
		m_bLeaf = false;
	}
	KNode(float x, float y, float w, float h)
	{
		m_pParent = nullptr; m_Element = { 0,0 };
		m_bLeaf = false; m_data = 0;
		m_index = 0; m_depth = 0;
		m_CornerList.push_back(x);
		m_CornerList.push_back(y);
		m_CornerList.push_back(w);
		m_CornerList.push_back(h);
	}
	~KNode()
	{
		for (int iChild = 0; iChild < 4; iChild++)
		{
			if (m_pChildlist[iChild] != nullptr)
			{
				delete m_pChildlist[iChild];
				m_pChildlist[iChild] = nullptr;
			}
		}
		for (auto ol : m_StaticObjectList)
		{
			if (ol != nullptr)
			{
				ol->obj_pObject->Release();
				delete ol;
			}
		}
		for (auto ol_d : m_DynamicObjectList)
		{
			if (ol_d != nullptr)
			{
				ol_d->obj_pObject->Release();
				delete ol_d;
			}
		}

	}
};