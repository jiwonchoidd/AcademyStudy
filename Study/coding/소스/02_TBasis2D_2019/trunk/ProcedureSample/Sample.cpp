#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <iostream>
#include <stdlib.h>
#include <sal.h>
SQLHENV g_hEnv;
SQLHDBC g_hDbc;
SQLHSTMT g_hStmt;
void Select();
bool Add();
bool Delete();
bool Update();
#define TRYODBC(h, ht, x)   {   RETCODE rc = x;\
                                if (rc != SQL_SUCCESS) \
                                { \
                                    HandleDiagnosticRecord (h, ht, rc); \
                                } \
                                if (rc == SQL_ERROR) \
                                { \
                                    fwprintf(stderr, L"Error in " L#x L"\n"); \
                                }  \
                            }
void MkDate(TIMESTAMP_STRUCT& ts, TCHAR* szBuffer)
{
	TCHAR szMonth[4], szDay[4], szYear[8];

	_stprintf(szMonth, _T("%u"), ts.month);
	_stprintf(szDay, _T("%u"), ts.day);
	_stprintf(szYear, _T("%u"), ts.year);
	_tcscpy(szBuffer, szMonth);
	_tcscat(szBuffer, _T("/"));
	_tcscat(szBuffer, szDay);
	_tcscat(szBuffer, _T("/"));
	_tcscat(szBuffer, szYear);

	return;
}
void ConvertTimeStampStructToString(TIMESTAMP_STRUCT tTimeStamp, CHAR *strTimeStamp, LONG lSizeTimeStamp)
{
	strcpy(strTimeStamp, "");
	sprintf(strTimeStamp, "%d-%d-%d %d:%d:%d", tTimeStamp.year, tTimeStamp.month, tTimeStamp.day, tTimeStamp.hour, tTimeStamp.minute, tTimeStamp.second);
}
void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000] = { 0, };
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1] = { 0, };


	if (RetCode == SQL_INVALID_HANDLE)
	{
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRec(hType,
		hHandle,
		++iRec,
		wszState,
		&iError,
		wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
		(SQLSMALLINT *)NULL) == SQL_SUCCESS)
	{
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5))
		{
			MessageBox(NULL, wszMessage, wszState, MB_OK);
		}
	}
}
static int print_error(SQLHENV    henv, SQLHDBC        hdbc, SQLHSTMT   hstmt)
{
	SQLTCHAR     buffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLTCHAR     sqlstate[SQL_SQLSTATE_SIZE + 1];
	SQLINTEGER  sqlcode;
	SQLSMALLINT length;

	while (SQLError(henv, hdbc, hstmt, sqlstate, &sqlcode, buffer,
		SQL_MAX_MESSAGE_LENGTH + 1, &length) == SQL_SUCCESS)
	{
		printf("\n **** ERROR *****\n");
		printf("         SQLSTATE: %s\n", sqlstate);
		printf("Native Error Code: %ld\n", sqlcode);
		printf("%s \n", buffer);
	};
	return 0;
}
/*
 Insert into tblCigar (name,price, korean) values ('aaa', 3000, 1)
*/
bool Add()
{
	TCHAR szSQL[256] = { 0, };// _T("Insert into tblCigar(name, price, korean) values('흡연', 500, 5)");
	SQLTCHAR Name[21] = _T("금연");
	SQLINTEGER Price = 30000;
	BOOL  Korean = FALSE;

	_stprintf((TCHAR*)szSQL, _T("Insert into tblCigar(name, price, korean) values('%s', %d, %d)"),
		Name, Price, Korean);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS)
		== SQL_SUCCESS)
	{
		SQLCloseCursor(g_hStmt);
		Select();
		return true;
	}
	SQLCloseCursor(g_hStmt);
	return false;
}
/*
   select ,,  from table
   select ,,  from table where ?=?;
*/
void Select()
{
	SQLTCHAR ID[20] = { 0, };
	SQLTCHAR Pass[20] = { 0, };
	TIMESTAMP_STRUCT tm;
	int iPrice, iKorean;
	SQLLEN lName, lPass, lPrice, lKorean;
	SQLBindCol(g_hStmt, 1, SQL_C_CHAR, ID, sizeof(ID), &lName);
	SQLBindCol(g_hStmt, 2, SQL_C_CHAR, Pass, sizeof(Pass), &lPass);
	SQLBindCol(g_hStmt, 3, SQL_C_TYPE_TIMESTAMP,
		&tm, sizeof(tm), &lKorean);
	//SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &iKorean, 0, &lKorean);
////	if (SQLExecDirect(g_hStmt,
//////		(SQLTCHAR*)_T("select name,price,korean from tblCigar where name='this'"), SQL_NTS) !=
////(SQLTCHAR*)_T("select USERID, USERPS from UserList"), SQL_NTS) !=
////		SQL_SUCCESS)
////	{
////		return;
////	}
	SWORD sReturn = 0;
	SQLLEN cbRetParam = SQL_NTS;
	SQLRETURN ret2 = SQLBindParameter(g_hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT,
		SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	SQLCHAR id[10] = "홍길동";
	SQLBindParameter(g_hStmt, 2, SQL_PARAM_INPUT,
		SQL_C_CHAR, SQL_VARCHAR, 10, 0, id, sizeof(id), NULL);

	SQLTCHAR sql[] = _T("{? = CALL dbo.usp_pass(?)}");
	ret2 = SQLPrepare(g_hStmt, sql, SQL_NTS);

	SQLRETURN ret = SQLExecute(g_hStmt);
	//SQLExecDirect(g_hStmt,(SQLTCHAR*)_T("{? = CALL usp_pass(?)}"), SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleDiagnosticRecord(g_hStmt, SQL_HANDLE_STMT, ret);
		print_error(g_hEnv, g_hDbc, g_hStmt);
		return;
	}
	while (1)
	{
		ret = SQLFetch(g_hStmt);
		if (ret == SQL_NO_DATA)
		{
			break;
		}
		if (ret == SQL_ERROR)
		{
			HandleDiagnosticRecord(g_hStmt, SQL_HANDLE_STMT, ret);
		}
		else
		{
			printf("\n%s:%s", ID, Pass);
		}
	}
	while ((ret = SQLMoreResults(g_hStmt)) != SQL_NO_DATA)
		printf("\nRet%d:", sReturn);
	SQLCloseCursor(g_hStmt);
}
void SelectOut()
{
	SWORD sReturn = 0;
	SQLLEN cbRetParam = SQL_NTS;
	SQLRETURN ret2 = SQLBindParameter(g_hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT,
		SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	SQLCHAR id[10] = "홍길동";
	SQLBindParameter(g_hStmt, 2, SQL_PARAM_INPUT,
		SQL_C_CHAR, SQL_VARCHAR, 10, 0, id, sizeof(id), NULL);

	SQLCHAR outPS[10] = { 0, };
	SQLBindParameter(g_hStmt, 3, SQL_PARAM_OUTPUT,
		SQL_C_CHAR, SQL_VARCHAR, 10, 0, outPS, sizeof(outPS), NULL);

	SQLTCHAR sql[] = _T("{? = CALL dbo.usp_passout(?,?)}");
	ret2 = SQLPrepare(g_hStmt, sql, SQL_NTS);

	SQLRETURN ret = SQLExecute(g_hStmt);
	//SQLExecDirect(g_hStmt,
	//(SQLTCHAR*)_T("{? = CALL usp_pass(?)}"), SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleDiagnosticRecord(g_hStmt, SQL_HANDLE_STMT, ret);
		print_error(g_hEnv, g_hDbc, g_hStmt);
		return;
	}
	while ((ret = SQLMoreResults(g_hStmt)) != SQL_NO_DATA)
		printf("\nRet%d:", sReturn);
	SQLCloseCursor(g_hStmt);
}
void SelectInfo()
{
	SQLTCHAR ID[20] = { 0, };
	SQLTCHAR Pass[20] = { 0, };
	TIMESTAMP_STRUCT tm;
	int iPrice, iKorean;
	SQLLEN lName, lPass, lPrice, lKorean;
	SQLBindCol(g_hStmt, 1, SQL_C_CHAR, ID, sizeof(ID), &lName);
	SQLBindCol(g_hStmt, 2, SQL_C_CHAR, Pass, sizeof(Pass), &lPass);
	SQLBindCol(g_hStmt, 3, SQL_C_TYPE_TIMESTAMP,
		&tm, sizeof(tm), &lKorean);
	//SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &iKorean, 0, &lKorean);
////	if (SQLExecDirect(g_hStmt,
//////		(SQLTCHAR*)_T("select name,price,korean from tblCigar where name='this'"), SQL_NTS) !=
////(SQLTCHAR*)_T("select USERID, USERPS from UserList"), SQL_NTS) !=
////		SQL_SUCCESS)
////	{
////		return;
////	}
	SWORD sReturn = 0;
	SQLLEN cbRetParam = SQL_NTS;
	SQLRETURN ret2 = SQLBindParameter(g_hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT,
		SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	SQLCHAR id[10] = "홍길동";
	SQLBindParameter(g_hStmt, 2, SQL_PARAM_INPUT,
		SQL_C_CHAR, SQL_VARCHAR, 10, 0, id, sizeof(id), NULL);

	SQLTCHAR sql[] = _T("{? = CALL dbo.usp_userinfo(?)}");
	ret2 = SQLPrepare(g_hStmt, sql, SQL_NTS);

	SQLRETURN ret = SQLExecute(g_hStmt);
	//SQLExecDirect(g_hStmt,(SQLTCHAR*)_T("{? = CALL usp_pass(?)}"), SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleDiagnosticRecord(g_hStmt, SQL_HANDLE_STMT, ret);
		print_error(g_hEnv, g_hDbc, g_hStmt);
		return;
	}
	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		// unicode
		//TCHAR szBuffer[255] = { 0, };
		//memset(szBuffer, 0, 255);
		//MkDate(tm, szBuffer);

		// multi-byte
		CHAR szBuffer2[255] = { 0, };
		memset(szBuffer2, 0, 255);
		ConvertTimeStampStructToString(tm, szBuffer2, sizeof(szBuffer2));
		printf("\n%s:%s", ID, Pass);
		printf(" %s", szBuffer2);
	}
	while ((ret = SQLMoreResults(g_hStmt)) != SQL_NO_DATA)
		printf("\nRet%d:", sReturn);
	SQLCloseCursor(g_hStmt);
}
bool Delete()
{
	TCHAR szSQL[256] = _T("Delete from tblCigar where name='금연'");
	SQLTCHAR Name[21] = _T("금연");
	SQLINTEGER Price = 30000;
	BOOL  Korean = FALSE;

	//_stprintf((TCHAR*)szSQL, _T("Delete from tblCigar where name='%s')"),
	//	Name);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS)
		== SQL_SUCCESS)
	{
		SQLCloseCursor(g_hStmt);
		Select();
		return true;
	}
	SQLCloseCursor(g_hStmt);
	return false;
}
bool Update()
{
	TCHAR szSQL[256] =
		_T("Update tblCigar set name='금연자' where name='금연'");
	SQLTCHAR Name[21] = _T("금연");
	SQLINTEGER Price = 30000;
	BOOL  Korean = FALSE;

	//_stprintf((TCHAR*)szSQL, _T("Delete from tblCigar where name='%s')"),
	//	Name);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS)
		!= SQL_SUCCESS)
	{
		return false;
	}

	SQLCloseCursor(g_hStmt);
	Select();
	return false;
}
void main()
{
	// 환경핸들
	if (SQLAllocHandle(SQL_HANDLE_ENV,
		SQL_NULL_HANDLE, &g_hEnv)
		!= SQL_SUCCESS)
	{
		return;
	}
	if (SQLSetEnvAttr(g_hEnv,
		SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3,
		SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return;
	}
	// 연결핸들

	SQLAllocHandle(SQL_HANDLE_DBC,
		g_hEnv, &g_hDbc);

	/*SQLTCHAR szInCon[256] = { 0, };
	_stprintf((TCHAR*)szInCon,
_T("DRIVER={Microsoft Access Driver (*.mdb)};")
_T("DBQ=%s\\cig.mdb;"),L"..\\..\\data");*/
//SQLTCHAR szInCon[256] =  _T("DSN=KGCASERVER");
	SQLTCHAR szOutCon[256] = { 0, };
	SQLSMALLINT cbCon;
	SQLTCHAR szInCon[256] = { 0, };
	_stprintf(szInCon, _T("%s"), _T("Driver={SQL Server};Server=shader.kr;Address=192.168.0.101,1433;Network=dbmssocn;Database=KGCAGAME;Uid=sa;Pwd=kgca!@34;"));
	//_stprintf(szInCon, _T("%s"), _T("Driver={SQL Server};Server=shader.kr;Address=112.216.123.139,1433;Network=dbmssocn;Database=WildPlanetDB;Uid=sa;Pwd=kgca!@34;"));

	SQLRETURN ret = SQLDriverConnect(g_hDbc,
		NULL,
		szInCon, _countof(szInCon),
		szOutCon, _countof(szOutCon),
		&cbCon, SQL_DRIVER_NOPROMPT);
	/*SQLRETURN ret = SQLConnect(g_hDbc,
		(SQLTCHAR*)L"dbo.USERLIST", SQL_NTS,
		(SQLTCHAR*)L"sa", SQL_NTS,
		(SQLTCHAR*)L"kgca!@34", SQL_NTS);*/
	if (ret != SQL_SUCCESS &&
		ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleDiagnosticRecord(g_hDbc, SQL_HANDLE_DBC, ret);
		/*SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
		SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
		SQLINTEGER sqlcode;
		SQLSMALLINT length;
		SQLError(g_hEnv, g_hDbc, g_hDbc, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length);
		MessageBox(NULL, buffer, sqlstate, MB_OK);*/
		return;
	}

	// 명령핸들

	SQLAllocHandle(SQL_HANDLE_STMT,
		g_hDbc, &g_hStmt);

	while (1)
	{
		int iSelect;
		printf("\n0:출력, 1:추가, 2:삭제, 3:수정");
		scanf_s("%d", &iSelect);
		switch (iSelect)
		{
		case 0:SelectInfo(); break;
		case 1:SelectOut(); break;
		case 2:Add(); break;
		case 3:Delete(); break;
		case 4:Update(); break;
		}
	}
	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLDisconnect(g_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}