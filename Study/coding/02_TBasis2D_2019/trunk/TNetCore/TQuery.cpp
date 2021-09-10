#include "TQuery.h"
bool TQuery::GetDiagRec(SQLHANDLE handle, SQLSMALLINT     fHandleType)
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
	SQLGetDiagField(fHandleType, handle, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);

	while (iCurrentRecord <= numRecs)
	{
		if (ret = SQLGetDiagRec(fHandleType, handle, iCurrentRecord, SqlState, &NativeError,
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
void TQuery::CreatePrepare()
{
	SQLRETURN ret;
	// TODO : Login sql
	{
		TCHAR sql[] = L"select * from DemoGame where userid=?";
		ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hLoginStmt);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hLoginStmt);
			return;
		}
		if (SQLPrepare(m_hLoginStmt, sql, SQL_NTS) != SQL_SUCCESS)
		{
			Check(m_hLoginStmt);
			return;
		}
		m_iNameLength = sizeof(m_szUserID);
		m_cbColumn = SQL_NTS;
		ret = SQLBindParameter(m_hLoginStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE,
			SQL_CHAR, m_iNameLength, 0, m_szUserID, m_iNameLength, &m_cbColumn);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hLoginStmt);
			return;
		}
	}

	// TODO : MSSQL 저장프로시져 호출 
	{
		/*TCHAR sql[] = L"{ ?=call LoginUser(?,?)}";
		ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hLoginStmtCall);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hLoginStmtCall);
			return;
		}
		if (SQLPrepare(m_hLoginStmtCall, sql, SQL_NTS) != SQL_SUCCESS)
		{
			Check(m_hLoginStmtCall);
			return;
		}
		m_iNameLength = sizeof(m_szUserID);
		m_cbColumn = SQL_NTS;

		SQLLEN cbRetRaram;
		ret = SQLBindParameter(m_hLoginStmtCall, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT,
			SQL_INTEGER, 0, 0, &m_iRetRaram, 0, &cbRetRaram);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hLoginStmtCall);
			return;
		}

		ret = SQLBindParameter(m_hLoginStmtCall, 2, SQL_PARAM_INPUT, SQL_UNICODE,
			SQL_WCHAR, m_iNameLength, 0, m_szUserID, 0, &m_cbColumn);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hLoginStmtCall);
			return;
		}
		ret = SQLBindParameter(m_hLoginStmtCall, 3, SQL_PARAM_INPUT, SQL_UNICODE,
			SQL_WCHAR, m_iNameLength, 0, m_szUserPS, 0, &m_cbColumn);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hLoginStmtCall);
			return;
		}*/
	}

	// TODO : Account sql
	{
		TCHAR sqlAccount[] = L"insert into DemoGame (userid, userps) VALUES (?,?)";;
		ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hAccountStmt);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hAccountStmt);
			return;
		}
		if (SQLPrepare(m_hAccountStmt, sqlAccount, SQL_NTS) != SQL_SUCCESS)
		{
			Check(m_hAccountStmt);
			return;
		}
		m_iNameLength = sizeof(m_szUserID);
		m_cbColumnAccount[0] = SQL_NTS;
		m_cbColumnAccount[1] = SQL_NTS;
		ret = SQLBindParameter(m_hAccountStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, m_iNameLength, 0, m_szUserID, 0, &m_cbColumnAccount[0]);
		ret = SQLBindParameter(m_hAccountStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, m_iNameLength, 0, m_szUserPS, 0, &m_cbColumnAccount[1]);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hAccountStmt);
			return;
		}
	}

	// TODO : userlist prepare
	{
		TCHAR sqlUserlist[] = L"select * from DemoGame";
		//TCHAR sqlUserlist[] = L"{ call AllUserList()}";
		ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hUserListStmt);
		if (ret != SQL_SUCCESS)
		{
			Check(m_hUserListStmt);
			return;
		}
		if (SQLPrepare(m_hUserListStmt, sqlUserlist, SQL_NTS) != SQL_SUCCESS)
		{
			Check(m_hUserListStmt);
			return;
		}
	}
}
bool TQuery::ExecPrepare(SQLHSTMT stmt, bool bReturn)
{
	SQLRETURN ret = SQLExecute(stmt);
	if (ret != SQL_SUCCESS)
	{
		Check(stmt);
		return false;
	}
	if (bReturn)
	{
		if (GetRecordData(stmt) == false)
		{
			Check(stmt);
			return false;
		}
	}
	else
	{
		SQLLEN iRowCount = 0;
		ret = SQLRowCount(stmt, &iRowCount);
		if (ret != SQL_SUCCESS)
		{
			Check(stmt);
			return false;
		}
	}
	return true;
}
bool TQuery::ExecLogin(std::wstring szID, std::wstring szPass)
{
	_tcscpy_s(m_szUserID, szID.c_str());
	_tcscpy_s(m_szUserPS, szPass.c_str());
	SQLRETURN ret = SQLExecute(m_hLoginStmt);
	if (ret != SQL_SUCCESS)
	{
		Check(m_hLoginStmt);
		return false;
	}
	if (GetRecordData(m_hLoginStmt) == false)
	{
		Check(m_hLoginStmt);
		return false;
	}
	return true;
}
bool TQuery::ExecLogin(std::wstring szID)
{
	_tcscpy_s(m_szUserID, szID.c_str());
	SQLRETURN ret = SQLExecute(m_hLoginStmt);
	if (ret != SQL_SUCCESS)
	{
		Check(m_hLoginStmt);
		return false;
	}
	if (GetRecordData(m_hLoginStmt) == false)
	{
		Check(m_hLoginStmt);
		return false;
	}
	return true;
}

bool TQuery::ExecAccount(std::wstring szID, std::wstring szPS)
{
	_tcscpy_s(m_szUserID, szID.c_str());
	_tcscpy_s(m_szUserPS, szPS.c_str());
	SQLRETURN ret = SQLExecute(m_hAccountStmt);
	if (ret != SQL_SUCCESS)
	{
		Check(m_hAccountStmt);
		return false;
	}
	//UPDATE, INSERT, DELETE 문만 반환됨.
	SQLLEN iRowCount = 0;
	ret = SQLRowCount(m_hAccountStmt, &iRowCount);
	if (ret != SQL_SUCCESS)
	{
		Check(m_hAccountStmt);
		return false;
	}	
	return true;
}
void TQuery::Check(SQLHSTMT stmt)
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(m_hEnv, m_hDbc,
		stmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}
bool TQuery::Connect(int iType)
{
	SQLRETURN ret;
	SQLWCHAR  InCon[255] = { 0, };
	SQLWCHAR  Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	SQLWCHAR OutConnStr[255] = { 0, };
	SQLSMALLINT OutConnStrLen = _countof(OutConnStr);

	SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);


	GetCurrentDirectory(MAX_PATH, Dir);
	std::wstring szDBPath = Dir;
	szDBPath += L"/../../data/db/";
	
	switch (iType)
	{
		case 0:
		{
			wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), szDBPath.c_str(), L"DemoGame.accdb");

			ret = SQLDriverConnect(m_hDbc, NULL, InCon, _countof(InCon), OutConnStr, (SQLSMALLINT)_countof(OutConnStr), &OutConnStrLen, SQL_DRIVER_NOPROMPT);
			if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO))
			{
				GetDiagRec(m_hDbc, SQL_HANDLE_DBC);
				return false;
			}
		}break;
		case 1: // dsn
		{
			// 텍스트 파일 생성 후 확장자 변경 ->xxx.udl
			wsprintf(InCon, _T("FileDsn=%s%s"), szDBPath.c_str(), _T("DemoGameSQL.dsn"));
			ret = SQLDriverConnect(m_hDbc, NULL, 
				InCon, _countof(InCon), OutConnStr, 
				(SQLSMALLINT)_countof(OutConnStr), 
				&OutConnStrLen, SQL_DRIVER_NOPROMPT);
			if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO))
			{
				GetDiagRec(m_hDbc, SQL_HANDLE_DBC);
				return false;
			}
		}break;
	
	}

	ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		return false;
	}

	/// TODO : CreatePrepare
	CreatePrepare();
	return true;
}
bool TQuery::Exec(const TCHAR * szSql)
{
	if (SQLExecDirect(m_hStmt, (SQLTCHAR*)szSql, SQL_NTS) != SQL_SUCCESS)
	{
		return false;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(m_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(m_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(m_hStmt);
	return true;
	//SQLRETURN ret;
	//SQLTCHAR  userid[30] = { 0, };
	//SQLTCHAR  userps[30] = { 0, };
	//SQLINTEGER id;
	//SQLLEN  lID, lUserID, lUserPS;
	//SQLBindCol(m_hStmt, 1, SQL_INTEGER, &id, 0, &lID);
	//SQLBindCol(m_hStmt, 2, SQL_UNICODE, (SQLPOINTER)userid, sizeof(userid), &lUserID);
	//SQLBindCol(m_hStmt, 3, SQL_UNICODE, (SQLPOINTER)userps, sizeof(userps), &lUserPS);
	//TCHAR sql[] = L"select * from DemoGame";
	//ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	//SQLLEN iNumRow;
	//SQLSMALLINT iNumCol;
	//SQLRowCount(m_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	//SQLNumResultCols(m_hStmt, &iNumCol); // 반환 컬럼 수

	//while (SQLFetch(m_hStmt) != SQL_NO_DATA)
	//{
	//	//std::wcout << "id=" << id << " " << userid << " " << userps << std::endl;
	//}
	//SQLCloseCursor(m_hStmt);
	return true;
}
bool TQuery::Init()
{
	SQLRETURN ret;
	// 2)핸들 할당 -> 설정
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		ret = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3_80, 0);
		if (!( ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO ))
		{
			GetDiagRec(m_hDbc, SQL_HANDLE_DBC);
			SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
			return false;
		}
		ret = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
		if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO))
		{
			SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
			return false;
		}
	}
	return true;
}

bool TQuery::Release()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_hLoginStmtCall);	
	SQLFreeHandle(SQL_HANDLE_STMT, m_hUserListStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hLoginStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hAccountStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	return true;
}
SQLRETURN TQuery::GetDescribeCol(SQLHSTMT stmt)
{
	SQLLEN count = 0;

	//UPDATE, INSERT, DELETE 문만 반환됨.
	SQLRETURN ret = SQLRowCount(stmt, &count);
	if (ret != SQL_SUCCESS)
	{
		Check(stmt);
	}
	SQLSMALLINT colCount;
	SQLNumResultCols(stmt, &colCount);

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
			//Check(stmt);
			break;
		}
		m_ColumnList.push_back(col);
		col.icol++;
	}
	return ret;
}
SQLRETURN TQuery::GetColData(SQLHSTMT stmt, int column, std::wstring& rt)
{
	SQLRETURN ret;
	if (m_ColumnList[column - 1].pfSqlType != SQL_VARBINARY)
	{
		if (ret = SQLGetData(stmt,
			m_ColumnList[column - 1].icol,
			SQL_WCHAR,
			m_ColumnList[column - 1].bindData,
			sizeof(m_ColumnList[column - 1].bindData),
			NULL) == SQL_SUCCESS)
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
bool TQuery::GetRecordData(SQLHSTMT	hStmt)
{
	GetDescribeCol(hStmt);
	// SQL데이터 형에 따른 바인딩으로 처리 할 수 있다.
	SQLRETURN ret;
	while (m_ColumnList.size() > 0)
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
TQuery::TQuery()
{
}

TQuery::~TQuery()
{
}

// sample code
bool TQuery::ExecSelect(const TCHAR * sql, std::wstring& szID, std::wstring& szPS)
{
	SQLRETURN ret;
	SQLTCHAR  userid[30] = { 0, };
	SQLTCHAR  userps[30] = { 0, };
	SQLINTEGER id;
	SQLLEN  lID, lUserID, lUserPS;
	SQLBindCol(m_hStmt, 1, SQL_INTEGER, &id, 0, &lID);
	SQLBindCol(m_hStmt, 2, SQL_UNICODE, (SQLPOINTER)userid, sizeof(userid), &lUserID);
	SQLBindCol(m_hStmt, 3, SQL_UNICODE, (SQLPOINTER)userps, sizeof(userps), &lUserPS);
	ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(m_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(m_hStmt, &iNumCol); // 반환 컬럼 수

	if (SQLFetch(m_hStmt) != SQL_NO_DATA)
	{
		szID = userid;
		szPS = userps;
	}
	SQLCloseCursor(m_hStmt);
	return true;
}
bool TQuery::ExecUpdate(const TCHAR * szSql)
{
	//TCHAR sql[] =
	//	L"update DemoGame set userps='3333' where userid='코로나'";
	if (SQLExecDirect(m_hStmt, (SQLTCHAR*)szSql, SQL_NTS) != SQL_SUCCESS)
	{
		return false;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(m_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(m_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(m_hStmt);
	return true;
}
bool TQuery::ExecDelete(const TCHAR * szSql)
{
	TCHAR sql[] =
		L"delete from DemoGame where userid='홍길동'";
	if (SQLExecDirect(m_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return false;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(m_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(m_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(m_hStmt);
	return true;
}
bool TQuery::ExecInsert(const TCHAR * szSql)
{
	//TCHAR sql[] =
	//	L"insert into DemoGame (userid, userps) VALUES ('5555','1111')";
	if (SQLExecDirect(m_hStmt, (SQLTCHAR*)szSql, SQL_NTS) != SQL_SUCCESS)
	{
		return false;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(m_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(m_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(m_hStmt);
	return true;
}