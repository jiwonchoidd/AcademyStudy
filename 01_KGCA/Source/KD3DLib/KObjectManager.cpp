#include "KObjectManager.h"

void KObjectManager::AddCollisionExecute(KCollider* owner, CollisionFunction func)
{
	KCollider* temp = (KCollider*)owner;
	temp->m_ID = m_iExcueteCollisionID++;
	//���̵� ��ü 
	m_ObjectList.insert(std::make_pair(temp->m_ID, temp));
	//���̵� �Լ�
	m_fnCollisionExecute.insert(std::make_pair(temp->m_ID, func));
}

void KObjectManager::DeleteExecute(KCollider* owner, CollisionFunction func)
{
}

bool KObjectManager::Init()
{
	return true;
}

bool KObjectManager::Frame()
{
	for (auto src : m_ObjectList)
	{
		KCollider* pObjSrc = (KCollider*)src.second;
		//ignore ������ �ݶ��̴��� �����Ѵ�.
		if (pObjSrc->m_CollisonType == KCollisionType::Ignore) continue;
		DWORD dwState = KCollisionType::Overlap;
		for (auto dest : m_ObjectList)
		{
			KCollider* pObjDest = (KCollider*)dest.second;
			if (pObjSrc == pObjDest) continue;
			if (KCollision::ToRect(pObjSrc->m_rtColl,(pObjDest->m_rtColl)))
			{
				FuncionIterator colliter = m_fnCollisionExecute.find(pObjSrc->m_ID);
				if (colliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjDest, dwState);
				}
			}
		}
	}
	return true;
}

bool KObjectManager::Release()
{
	return true;
}
