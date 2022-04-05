#include "KNode.h"

void KNode::AddObject(std::shared_ptr<KMapObject> obj)
{
	m_StaticObjectList.push_back(obj);
}

void KNode::AddDynamicObject(std::shared_ptr<KMapObject> obj)
{
	m_DynamicObjectList.push_back(obj);
}
