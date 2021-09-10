#include "Sample.h"
#include "TBasisSys.h"
#include "TDebugString.h"

bool Sample::Init()
{	
	I_Server.Init();	
	I_DebugStr.Init();
	return true;
}
bool Sample::Frame()
{
	I_DebugStr.Frame();
	return true;
}
bool Sample::Release()
{
	I_Server.Release();
	I_DebugStr.Release();
	return true;
}
Sample::Sample(void)
{
}

Sample::~Sample(void)
{
}
INT WINAPI wWinMain(   HINSTANCE hInst, 
								HINSTANCE hPreInst, 
								LPWSTR strCmdLine, 
								INT nShow )
{
	Sample sWin;
	sWin.SetWindow(hInst, 800, 600, L"SampleServer2D_0");
	return 0;
}
//
//#include <windows.h>
//#include <stdio.h>
//#include <conio.h>
//#include <tchar.h>
////make sure you include the namespace
//using namespace ODBC;
//
//int main(void)
//{
//	MSSQLConnection link;
//	if (link.Connect())
//	{
//		ODBCStmt Stmt(link);
//		SQLExecDirect(Stmt, (SQLTCHAR*)_T("USE NorthWind"), SQL_NTS);
//		int pos = 1;
//		TCHAR strQuery[256] = _T("SELECT * FROM [Customers]");
//		int nRet = Stmt.Query(strQuery);
//		printf("%5d:\tColumns\t%d\r\n\r\n", Stmt.GetColumnCount(), 0);
//		while (Stmt.Fetch())
//		{
//			ODBCRecord rec(Stmt);
//			printf("Row %5d\r\n", pos);
//			INT i = 0;
//			while (i<Stmt.GetColumnCount())
//			{
//				TCHAR Desc[512] = _T(""); SQLINTEGER cbDesc = 0;
//				rec.GetData(i + 1, Desc, sizeof(Desc), &cbDesc);
//				TCHAR Name[256] = _T("");
//				rec.GetColumnName(i + 1, Name, sizeof(Name));
//				i++;
//				printf("\t%15s\t\\>\t%25s\r\n", Name, Desc);
//			}
//			pos++;
//		};
//	}
//	link.Disconnect();
//	getch();
//	return 0;
//}
//
//#include <windows.h>
//#include <stdio.h>
//#include <conio.h>
//#include <tchar.h>
////make sure you include the namespace
//using namespace ODBC;
//
//int main(void)
//{
//	MSSQLConnection link;
//	if (link.Connect())
//	{
//		ODBCStmt Stmt(link);
//		SQLExecDirect(Stmt, (SQLTCHAR*)_T("USE NorthWind"), SQL_NTS);
//		int pos = 1;
//		TCHAR strQuery[256] = _T("SELECT * FROM [Customers]");
//		int nRet = Stmt.Query(strQuery);
//		printf("%5d:\tColumns\t%d\r\n\r\n", Stmt.GetColumnCount(), 0);
//		Stmt.FetchLast();
//		do
//		{
//			ODBCRecord rec(Stmt);
//			printf("Row %5d\r\n", pos);
//			INT i = 0;
//			while (i<Stmt.GetColumnCount())
//			{
//				TCHAR Desc[512] = _T(""); SQLINTEGER cbDesc = 0;
//				rec.GetData(i + 1, Desc, sizeof(Desc), &cbDesc);
//				TCHAR Name[256] = _T("");
//				rec.GetColumnName(i + 1, Name, sizeof(Name));
//				i++;
//				printf("\t%15s\t\\>\t%25s\r\n", Name, Desc);
//			}
//			pos++;
//		} while (Stmt.FetchPrevious());
//	}
//	link.Disconnect();
//	getch();
//	return 0;
//}