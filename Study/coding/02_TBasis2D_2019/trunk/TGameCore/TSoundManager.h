#pragma once
#include <windows.h>
#include "TManager.h"
class TSound
{
public:
	std::wstring m_csName;
	TSound()
	{
		m_csName = std::to_wstring(-1);
	}
	TSound(std::wstring name)
	{
		m_csName = std::to_wstring(rand());
	}
};
template <class T>
class TSoundManager : public TManager<T>
{
public:
	TSoundManager();
	virtual ~TSoundManager();
};
template <class T>
TSoundManager<T>::TSoundManager()
{

}
template <class T>
TSoundManager<T>::~TSoundManager()
{

}