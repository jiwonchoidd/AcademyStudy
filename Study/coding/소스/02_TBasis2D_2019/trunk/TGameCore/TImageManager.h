#pragma once

#include "TManager.h"
class TImage
{
public:
	std::wstring m_csName;
	TImage()
	{
		m_csName = std::to_wstring(-1);
	}
	TImage(std::wstring name)
	{
		m_csName = std::to_wstring(rand());
	}
};
template <class T>
class TImageManager : public TManager<T>
{
public:
	TImageManager();
	virtual ~TImageManager();
};
template <class T>
TImageManager<T>::TImageManager()
{

}
template <class T>
TImageManager<T>::~TImageManager()
{

}