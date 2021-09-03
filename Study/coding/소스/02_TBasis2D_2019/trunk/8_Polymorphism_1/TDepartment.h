#pragma once
#include "TPermanent.h"
#include "TTemporary.h"

#define MAX_ARRAY 3
class TDepartment
{
private:
	TEmployee* m_pList[MAX_ARRAY];
	int m_iIndex;
public:
	TDepartment();
	~TDepartment();
public:
	void AddEmployee(TEmployee*);
	void ShowList(); // 급여 리스트 출력.
	void Release();
};

