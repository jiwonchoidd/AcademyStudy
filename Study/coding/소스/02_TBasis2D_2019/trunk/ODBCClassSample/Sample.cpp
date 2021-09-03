#define EXAMPLE_CLASS
#ifdef EXAMPLE_CLASS
#include "TOdbcQuery.h"
#include "conio.h"
TOdbcQuery Example;
int main()
{
	setlocale(LC_ALL, "korean");

	SQLRETURN rc = SQL_SUCCESS;
	//SQLTCHAR DBName[] = L"TestBlob";// "testodbc";
	//SQLTCHAR sql[255] = L"SELECT * FROM USERBLOBDATA";
	SQLTCHAR DBName[] = L"DemoGame.accdb";// "testodbc";
	SQLTCHAR sql[255] = L"SELECT * FROM DemoGame";

	if (Example.Init())
	{
		//if (Example.Connect(3, DBName, L"sa", L"kgca!@34")==false)
		if (Example.Connect(1, DBName, L"sa", L"kgca!@34") == false)
		{
			return -1;
		}		
		rc = SQLExecDirect(Example.m_hStmt, sql, SQL_NTS);
		if (rc == SQL_SUCCESS)
		{
			if (Example.GetRecordData(Example.m_hStmt))
			{
				for (int iColumn = 0; iColumn < Example.m_RecordResultList.size(); iColumn++)
				{
					std::wcout << Example.m_RecordResultList[iColumn][1].c_str() << std::endl;
				}
			}
		}
		else
		{
			Example.Check();
		}

		//Example.Prepare(L"select * from USERBLOBDATA where BLOBName=?");		
		Example.Prepare(L"select * from DemoGame where userid=?");
		/////////////////////////////// Prepare 사용 //////////////////////////////////////////////
		//_tcscpy(Example.m_szSelectName, L"First test file");
		_tcscpy(Example.m_szSelectName, L"kgca");
		Example.Exec(Example.m_hReadStmt);
		for (int iColumn = 0; iColumn < Example.m_RecordResultList.size(); iColumn++)
		{
			std::wcout << Example.m_RecordResultList[iColumn][1].c_str() << std::endl;
		}
		//////////////////////////////  재사용 ///////////////////////////////////////
		//_tcscpy(Example.m_szSelectName, L"하늘이");
		_tcscpy(Example.m_szSelectName, L"5555");
		Example.Exec(Example.m_hReadStmt);
		for (int iColumn = 0; iColumn < Example.m_RecordResultList.size(); iColumn++)
		{
			std::wcout << Example.m_RecordResultList[iColumn][1].c_str() << std::endl;
		}
	}
	Example.Release();
	
	return 0;
}
#else
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <vector>
#pragma comment(lib,"odbc32.lib")

// 1)핸들선언
SQLHENV  g_hEnv = SQL_NULL_HENV; // 환경핸들
SQLHENV  g_hDbc = SQL_NULL_HDBC; // 연결핸들
SQLHENV  g_hStmt = SQL_NULL_HSTMT;// 명령핸들

TCHAR		g_szInputUserID[32] = { 0, };
SQLHENV		g_hUserIDStmt = SQL_NULL_HSTMT;// 명령핸들
SQLTCHAR	g_szUserID[32] = { 0, };
SQLTCHAR	g_szUserPS[32] = { 0, };
SQLINTEGER  g_iID;
SQLLEN		lID, lUserID, lUserPS;

int  iNameLength = 0;
SQLLEN cbColumn = SQL_NTS;
typedef std::vector< std::wstring> RECORD;

struct ColDescription
{
	SQLUSMALLINT       icol;
	SQLWCHAR		   szColName[80];
	SQLSMALLINT        cchColNameMax;
	SQLSMALLINT        pcchColName;
	SQLSMALLINT        pfSqlType;
	SQLULEN            pcbColDef;
	SQLSMALLINT        pibScale;
	SQLSMALLINT        pfNullable;
	SQLWCHAR		   bindData[80];
	//TargetValuePtr 버퍼에서 리턴할 수 있는 바이트 수를 표시하는 값(또는 값 배열)의 포인터
	SQLLEN			   byteSize; // bite
};
std::vector< ColDescription> m_ColumnList;
RECORD					m_ColumnResultList;
std::vector< RECORD >	m_RecordResultList;

SQLRETURN ret;
void Check(SQLHSTMT stmt)
{
	if (ret == SQL_SUCCESS) return;
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(g_hEnv, g_hDbc,
		stmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}
bool GetDiagRec(SQLHSTMT stmt)
{
	SQLRETURN ret;
	SQLSMALLINT		iCurrentRecord = 1, MsgLen;
	SQLTCHAR		SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER		NativeError;
	SQLLEN numRecs = 0;
	//오류, 경고 및 상태 정보가 포함 된 진단 데이터 구조
	//레코드의 필드의 현재 값을 반환 한다.
	//	01004 (데이터가 잘렸습니다.)
	//	01S02(옵션 값이 변경 됨)
	//	HY008(작업이 취소 됨)
	//	HYC00(선택적 기능이 구현 되지 않음)
	//	HYT00(제한 시간 만료 됨)
	SQLGetDiagField(SQL_HANDLE_STMT, stmt, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);

	while (iCurrentRecord <= numRecs)
	{
		if (ret = SQLGetDiagRec(SQL_HANDLE_STMT, stmt, iCurrentRecord, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
		{
			if (_tcscmp(SqlState, L"01004") == 0)
			{
				return true;
			}
			MessageBox(NULL, Msg, SqlState, MB_OK);
			iCurrentRecord++;
		}
	}
	return false;
}
void CreatePrepare()
{
	SQLRETURN ret;
	TCHAR sql[] = L"select * from DemoGame where userid=?";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hUserIDStmt);
	if (ret != SQL_SUCCESS)
	{
		return;
	}
	if (SQLPrepare(g_hUserIDStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}

	iNameLength = sizeof(g_szInputUserID);
	ZeroMemory(g_szInputUserID, iNameLength);
	cbColumn = SQL_NTS;

	ret = SQLBindParameter(g_hUserIDStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		iNameLength, 0, g_szInputUserID, iNameLength, &cbColumn);
	if (ret != SQL_SUCCESS)
	{
		Check(g_hUserIDStmt);
		return;
	}

	ret = SQLBindCol(g_hUserIDStmt, 1, SQL_INTEGER, &g_iID, 0, &lID);
	ret = SQLBindCol(g_hUserIDStmt, 2, SQL_UNICODE, (SQLPOINTER)g_szUserID, sizeof(g_szUserID), &lUserID);
	ret = SQLBindCol(g_hUserIDStmt, 3, SQL_UNICODE, (SQLPOINTER)g_szUserPS, sizeof(g_szUserPS), &lUserPS);
}
SQLRETURN GetDescribeCol(SQLHSTMT stmt)
{
	SQLLEN count = 0;

	//UPDATE, INSERT, DELETE 문만 반환됨.
	SQLRETURN ret = SQLRowCount(stmt, &count);
	if (ret != SQL_SUCCESS)
	{
		Check(stmt);
	}
	//SQLSMALLINT colCount;
	//SQLNumResultCols(stmt, &colCount);

	m_ColumnList.clear();
	m_ColumnResultList.clear();
	m_RecordResultList.clear();

	ColDescription col;
	col.icol = 1;
	while (1)
	{
		ret = SQLDescribeCol(stmt,
			col.icol,
			col.szColName,
			sizeof(col.szColName),
			&col.pcchColName,
			&col.pfSqlType,
			&col.pcbColDef,
			&col.pibScale,
			&col.pfNullable);

		if (ret != SQL_SUCCESS)
		{
			//Check();
			break;
		}
		m_ColumnList.push_back(col);
		col.icol++;
	}
	return ret;
}
SQLRETURN GetColData(SQLHSTMT stmt, int column, std::wstring& rt)
{
	SQLRETURN ret;
	if (m_ColumnList[column - 1].pfSqlType != SQL_VARBINARY)
	{
		if (ret = SQLGetData(stmt, m_ColumnList[column - 1].icol, SQL_WCHAR,
			m_ColumnList[column - 1].bindData,
			sizeof(m_ColumnList[column - 1].bindData), NULL) == SQL_SUCCESS)
		{
			rt = m_ColumnList[column - 1].bindData;
		}
	}
	else
	{
		TCHAR buf[1000] = { 0, };
		SQLLEN cb;
		do {
			ZeroMemory(buf, sizeof(TCHAR) * 1000);
			ret = SQLGetData(stmt, m_ColumnList[column - 1].icol, SQL_WCHAR, buf, sizeof(buf), &cb);
			if (ret == SQL_NULL_DATA) break;
			rt += buf;
		} while (ret == SQL_SUCCESS_WITH_INFO);

	}
	return ret;
}
bool GetRecordData(SQLHSTMT	hStmt)
{
	GetDescribeCol(hStmt);

	SQLRETURN ret;
	while (1)
	{
		ret = SQLFetch(hStmt);
		if (ret == SQL_NO_DATA) break;
		if (ret != SQL_SUCCESS)
		{
			if (GetDiagRec(hStmt) == false)
			{
				SQLCloseCursor(hStmt);
				return false;
			}
		}

		RECORD record;
		for (int iCol = 0; iCol < m_ColumnList.size(); iCol++)
		{
			std::wstring retString;
			ret = GetColData(hStmt, m_ColumnList[iCol].icol, retString);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				record.push_back(retString);
			}
		}
		m_RecordResultList.push_back(record);
	}
	while (ret = SQLMoreResults(hStmt) != SQL_NO_DATA);

	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_CLOSE);
	return true;
}

void ExecPrepare(SQLHSTMT stmt)
{
	_tcscpy_s(g_szInputUserID, L"kgca");
	ret = SQLExecute(stmt);
	if (ret != SQL_SUCCESS)
	{
		Check(stmt);
		return;
	}
	if (GetRecordData(stmt) == false)
	{
		Check(stmt);
		return;
	}


	//while (1)
	//{
	//	ret = SQLFetch(g_hUserIDStmt);
	//	GetDiagRec();
	//	if (ret == SQL_NO_DATA)
	//	{
	//		break;
	//	}
	//	std::wcout << "g_iID=" << g_iID << " " << g_szUserID << " " << g_szUserPS << std::endl;
	//}
	//while (ret = SQLMoreResults(g_hUserIDStmt) != SQL_NO_DATA);
	//SQLCloseCursor(g_hUserIDStmt);
	//SQLFreeStmt(g_hUserIDStmt, SQL_CLOSE);
}
// 3)핸들 사용 ->조회
void SelectQuery()
{
	SQLTCHAR  szUserID[30] = { 0, };
	SQLTCHAR  szUserPS[30] = { 0, };
	SQLINTEGER iID;
	SQLLEN  lID, lUserID, lUserPS;
	SQLBindCol(g_hStmt, 1, SQL_INTEGER, &iID, 0, &lID);
	SQLBindCol(g_hStmt, 2, SQL_UNICODE, (SQLPOINTER)szUserID, sizeof(szUserID), &lUserID);
	SQLBindCol(g_hStmt, 3, SQL_UNICODE, (SQLPOINTER)szUserPS, sizeof(szUserPS), &lUserPS);
	TCHAR sql[] = L"select * from DemoGame";
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(g_hStmt, &iNumCol); // 반환 컬럼 수

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << "g_iID=" << iID << " " << szUserID << " " << szUserPS << std::endl;
	}
	SQLCloseCursor(g_hStmt);
}
// 3)핸들 사용 ->추가
void InsertQuery()
{
	TCHAR sql[] =
		L"insert into DemoGame (g_szUserID, g_szUserPS) VALUES ('5555','1111')";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(g_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(g_hStmt);
}
////// 3)핸들 사용 ->삭제
void DeleteQuery()
{
	TCHAR sql[] = L"delete from DemoGame where g_szUserID='홍길동'";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(g_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(g_hStmt);
}
// 수정
void UpdateQuery()
{
	TCHAR sql[] =
		L"update DemoGame set g_szUserPS='3333' where g_szUserID='코로나'";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(g_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(g_hStmt);
}
void main()
{
	setlocale(LC_ALL, "KOREAN");
	// 2)핸들 할당 -> 설정
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv);
	ret = SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3_80, SQL_IS_INTEGER);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);

	TCHAR  InCon[256] = { 0, };
	TCHAR  Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	GetCurrentDirectory(MAX_PATH, Dir);
	wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, L"DemoGame.accdb");
	ret = SQLDriverConnect(g_hDbc, NULL, InCon, _countof(InCon), NULL, 0,
		&cbCon, SQL_DRIVER_NOPROMPT);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	if (ret != SQL_SUCCESS)
	{
		Check(g_hStmt);
		return;
	}

	CreatePrepare();

	_tcscpy_s(g_szInputUserID, L"kgca");
	ExecPrepare(g_hUserIDStmt);
	std::wcout << "id=" << g_iID << " userid=" << g_szUserID << " userps=" << g_szUserPS << std::endl;


	{
		_tcscpy_s(g_szInputUserID, L"kgca");
		ret = SQLExecute(g_hUserIDStmt);
		while (1)
		{
			ret = SQLFetch(g_hUserIDStmt);
			GetDiagRec(g_hUserIDStmt);
			if (ret == SQL_NO_DATA)
			{
				break;
			}
			std::wcout << "id=" << g_iID << " userid=" << g_szUserID << " userps=" << g_szUserPS << std::endl;
		}
		while (ret = SQLMoreResults(g_hUserIDStmt) != SQL_NO_DATA);
		SQLCloseCursor(g_hUserIDStmt);
		SQLFreeStmt(g_hUserIDStmt, SQL_CLOSE);
	}

	SelectQuery();
	InsertQuery();
	DeleteQuery();
	UpdateQuery();

	SQLFreeHandle(SQL_HANDLE_STMT, g_hUserIDStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);

}

#endif