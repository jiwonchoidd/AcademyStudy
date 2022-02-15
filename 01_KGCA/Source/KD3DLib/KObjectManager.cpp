#include "KObjectManager.h"

void KObjectManager::AddCollisionExecute(KCollider* owner, CollisionFunction func)
{
	owner->m_ID = m_iExcueteCollisionID++;
	m_ObjectList.insert(std::make_pair(owner->m_ID, owner));
	m_fnCollisionExecute.insert(std::make_pair(owner->m_ID, func));
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
		//ignore 형태의 콜라이더를 무시한다.
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
