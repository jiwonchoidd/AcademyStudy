#include "TDepartment.h"
void TDepartment::AddEmployee(TEmployee* pEmp)
{
	if (m_iIndex >= MAX_ARRAY)
	{
		if( pEmp != NULL)	delete pEmp;
		return;
	}
	m_pList[m_iIndex++] = pEmp;
}
void TDepartment::ShowList() // 급여 리스트 출력.
{
	for (int i = 0; i< m_iIndex; i++)
	{
		cout << "name: " << m_pList[i]->GetName() << endl;
		cout << "salary: " << m_pList[i]->GetPay() << endl;
		cout << endl;
	}
}
void TDepartment::Release()
{
	for (int i = 0; i < m_iIndex; i++)
	{
		if (m_pList[i] != NULL)
		{
			delete m_pList[i];
			m_pList[i] = NULL;
		}
	}
}
TDepartment::TDepartment() : m_iIndex(0)
{
	for (int i = 0; i < MAX_ARRAY; i++)
	{
		m_pList[i] = NULL;
	}
};
TDepartment::~TDepartment()
{
	Release();
}