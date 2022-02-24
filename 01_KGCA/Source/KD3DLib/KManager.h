#pragma once
#include "KStd.h"
//�� �Ŵ����� �⺻���� �̸�, ������ ������ �����Ѵ�.
//�� �Ŵ����� ��Ӿ��� �Ŵ������� �ε����� ������
template <class T, class S>
class KManager : public  Singleton<S>
{
public:
	friend class  Singleton<KManager>;
public:
	int		m_iIndex;
	std::map<std::wstring, T* >  m_list;
public:
	std::wstring Splitpath(std::wstring path, std::wstring entry);
	T* CheckLoad(std::wstring name);
	virtual T* Load(std::wstring filename);
	T* GetPtr(std::wstring key);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	KManager();
public:
	~KManager();
};
template<class T, class S>
std::wstring KManager<T, S>::Splitpath(std::wstring path,
	std::wstring entry)
{
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };

	std::wstring fullpathname = path;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	std::wstring name = FileName;
	name += FileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	return name;
}
//�ߺ� �߻� ����
template<class T, class S>
T* KManager<T, S>::CheckLoad(std::wstring name)
{
	for (auto data : m_list)
	{
		if (data.second->m_csName == name)
		{
			return data.second;
		}
	}
	return nullptr;
}
template<class T, class S>
T* KManager<T, S>::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
template<class T, class S>
T* KManager<T, S>::Load(std::wstring filename)
{
	std::wstring name = Splitpath(filename, L"");
	T* pData = CheckLoad(name);
	//������ �����Ͱ� �ִٸ� return
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new T;
	if (pData->Load(filename) == false)
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
template<class T, class S>
bool	KManager<T, S>::Init()
{
	return true;
}
template<class T, class S>
bool	KManager<T, S>::Frame()
{
	return true;
}
template<class T, class S>
bool	KManager<T, S>::Render()
{
	return true;
}
template<class T, class S>
bool	KManager<T, S>::Release()
{
	for (auto data : m_list)
	{
		data.second->Release();
		delete data.second;
	}
	m_list.clear();
	return true;
}
template<class T, class S>
KManager<T, S>::KManager()
{
	m_iIndex = 0;
}
template<class T, class S>
KManager<T, S>::~KManager()
{
	Release();
}