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
	// 10명만 있기 때문에 11은 검색되지 못한다.
	TStudent* pFind = g_CtlMgr.Find(11);
	try{			
		wcout << L"이름:" << pFind->GetName() << endl;
	}
	catch( int ex )
	{
		wcout << L"해당 번호 학생을 찾을 수 없습니다."<<endl;		
	}	
	
	g_CtlMgr.Release();
	cout << "종료";	
}