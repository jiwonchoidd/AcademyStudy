#include "TControlMgr.h"
#include <new>

using std::bad_alloc;

class ExceptA
{
public:
	void What()
	{
		cout << "ExceptA" << endl;
	}
};


void main()
{
	TControlMgr g_CtlMgr;
	g_CtlMgr.InputData();		
	g_CtlMgr.Sort();
	g_CtlMgr.ShowData();
	g_CtlMgr.ShowData(g_CtlMgr.Find(8));
	//g_CtlMgr.ShowData(mgr.Find(_T("CYXY")));
	// 10�� �ֱ� ������ 11�� �˻����� ���Ѵ�.
	TStudent* pFind = g_CtlMgr.Find(11);
	try{			
		wcout << L"�̸�:" << pFind->GetName() << endl;
	}
	catch( int ex )
	{
		wcout << L"�ش� ��ȣ �л��� ã�� �� �����ϴ�."<<endl;		
	}	
	
	g_CtlMgr.Release();
	cout << "����";	
}