#pragma once
#include <windows.h>
#include <map>
#include <string>
template <class T>
class TManager
{
private:
	std::map<std::wstring, T> m_Map;
	typedef typename std::map<std::wstring, T>::iterator ITER;
public:
	bool    Add(std::wstring);
	T*		GetPtr(std::wstring);
public:
	TManager();
	virtual ~TManager();
};
template <class T>
bool    TManager<T>::Add(std::wstring key)
{
	T data(key);
	//m_Map[key] = data;
	/*std::pair<std::wstring, TImage> addData;
	addData.first = key;
	addData.second = data;
	m_Map.insert(addData);*/
	std::pair<ITER, bool> ret;
	ret = m_Map.insert(std::make_pair(key, data));
	return ret.second;
}
template <class T>
T* TManager<T>::GetPtr(std::wstring key)
{	
	ITER iter = m_Map.find(key);
	if (iter != m_Map.end())
	{
		T* pData = (T*)&iter->second;
		return pData;
	}
	return nullptr;
}
template <class T>
TManager<T>::TManager()
{

}
template <class T>
TManager<T>::~TManager()
{

}

