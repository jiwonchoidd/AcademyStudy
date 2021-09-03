#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <tchar.h>
#include <vector>
#define SPA _T(" ")
using namespace::std;
#define ATYPE

class TToken;
namespace A_TYPE
{
	class TValue
	{
	private:
		TCHAR*		m_szValue;
		TValue*		m_pNext;
		friend class TToken;
	private:
		// �����ڵ� private�� �ϸ� TToken����
		// ����� �� �ִ� ���� Ŭ������ �ȴ�.
		TValue(TCHAR*);
	public:
		~TValue();
	};
}
namespace B_TYPE
{
	class TValue
	{
	public:
		TCHAR*		m_szValue;
		TValue*		m_pNext;
	public:
		TValue(TCHAR*);
		~TValue();
	};
}

#ifdef ATYPE
using namespace A_TYPE;
#else
using namespace B_TYPE;
#endif

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}


static std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}