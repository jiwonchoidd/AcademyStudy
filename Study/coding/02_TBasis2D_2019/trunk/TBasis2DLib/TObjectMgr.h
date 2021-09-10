#pragma once
#include "TEffect.h"
#include <functional>
struct cmp {
	bool operator()(int i, int j)
	{
		if (i > j) return true;
		else return false;
	}
};
class TObjectComposite : public TObject
{
public:
	std::deque<TObject*> m_Components;
public:
	TObject* Clone();
};

using CollisionFunction = std::function<void(TObject*, DWORD)>;
using SelectFunction = std::function<void(POINT, DWORD)>;

class TObjectManager : public TSingleton<TObjectManager>
{
public:
	friend  class TSingleton<TObjectManager>;
private:
	std::vector<TObjAttribute>  m_ObjAttribute;
	std::vector<TSpriteInfo>	m_rtSpriteList;
private:
	std::map<wstring, TObject*>				m_List;
	std::map<wstring, TObject*>::iterator	m_iter;
	std::wstring				m_szDefaultPath;
private:
	int		m_iExecuteSelectID = 0;
	int		m_iExecuteCollisionID = 0;
	std::map<int ,TObject*>	      m_CollisionObjectList;
	std::map<int, TObject*>	      m_SelectObjectList;

	typedef std::map<int, SelectFunction>::iterator SelectFuncIterator;
	std::map<int, SelectFunction>    m_fnSelectExecute;

	typedef std::map<int, CollisionFunction>::iterator CollisionFuncIterator;
	std::map<int, CollisionFunction>    m_fnCollisionExecute;
public:
	void		AddSelectExecute(TObject* ownder, SelectFunction func);
	void		AddCollisionExecute(TObject* ownder, CollisionFunction func);
	void		DeleteExecute(TObject* owner);
public:
	TObject*	Add(TObject* pAddObject);
	bool		Load(const TCHAR* filename);
	TObject*	GetPtr(wstring szName);
	TObject*	Clone(wstring szName);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	bool  LoadObjectFile(T_STR szFileName, std::vector<TObjAttribute>&);
	bool  LoadEffectFile(T_STR szFileName, std::vector<TSpriteInfo>&);
	void  CreateEffect(std::vector<TSpriteInfo>&	list);
	void  CreateObject(std::vector<TObjAttribute>&	list);
	void  GetBitmapLoad(FILE* fp, wstring& ret);
private:
	TObjectManager();
public:
	~TObjectManager();
};
#define g_ObjectMgr TObjectManager::GetInstance()

