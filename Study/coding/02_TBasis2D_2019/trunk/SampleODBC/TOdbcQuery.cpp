#include "TOdbcQuery.h"
bool TOdbcQuery::GetDiagRec()
{
	SQLRETURN ret;
	SQLSMALLINT		iCurrentRecord = 1, MsgLen;
	SQLTCHAR		SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER		NativeError;
	SQLLEN numRecs = 0;
	//����, ��� �� ���� ������ ���� �� ���� ������ ����
	//���ڵ��� �ʵ��� ���� ���� ��ȯ �Ѵ�.
	//	01004 (�����Ͱ� �߷Ƚ��ϴ�.)
	//	01S02(�ɼ� ���� ���� ��)
	//	HY008(�۾��� ��� ��)
	//	HYC00(������ ����� ���� ���� ����)
	//	HYT00(���� �ð� ���� ��)
	SQLGetDiagField(SQL_HANDLE_STMT, m_hStmt, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);

	while (iCurrentRecord <= numRecs)
	{
		if (ret = SQLGetDiagRec(SQL_HANDLE_STMT, m_hStmt, iCurrentRecord, SqlState, &NativeError,
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
bool TOdbcQuery::Init()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	// ���� �ڵ� -> �Ҵ� -> ����	
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	return true;
}
bool TOdbcQuery::Connect(
	int iType, const TCHAR* loadDB,
	const TCHAR* UID, const TCHAR* PWD)
{
	TCHAR Dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, Dir);

	TCHAR InCon[256] = { 0, };
	_stprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s"), Dir, loadDB);
	SQLSMALLINT cbOutLen;

	SQLRETURN ret;
	switch (iType)
	{
	case 1:
	{
		SQLRETURN ret = SQLDriverConnect(m_hDbc, NULL, InCon, _countof(InCon), NULL, 0, &cbOutLen, SQL_DRIVER_NOPROMPT);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			Check();
			return false;
		}
	}break;
	case 2:
	{
		wsprintf(InCon, _T("FileDsn=%s"), loadDB);
		ret = SQLDriverConnect(m_hDbc, NULL,
			InCon, _countof(InCon), NULL, 0, &cbOutLen, SQL_DRIVER_NOPROMPT);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			Check();
			return false;
		}
	}break;
	case 3:	
	{		
		ret = SQLConnect(m_hDbc,(SQLTCHAR*)loadDB, SQL_NTS,	(SQLTCHAR*)UID, SQL_NTS,	(SQLTCHAR*)PWD, SQL_NTS);	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			Check();
			return false;
		}
	}break;
	}
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	
	return true;
}
bool TOdbcQuery::Prepare(const TCHAR* sql)
{
	SQLRETURN ret;
	//PREPARE
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hReadStmt) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	//wsprintf(sql, L"select * from USERLIST where USERID=?", SQL_NTS);
	ret = SQLPrepare(m_hReadStmt, (SQLTCHAR*)sql, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}

	ZeroMemory(m_szSelectName, sizeof(m_szSelectName));
	m_iDataLength = sizeof(m_szSelectName);
	m_cbColumn = SQL_NTS;


	ret = SQLBindParameter(m_hReadStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR, m_iDataLength, 0, m_szSelectName, m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	return true;
}
SQLRETURN TOdbcQuery::GetDescribeCol(SQLHSTMT stmt)
{
	SQLLEN count=0;
	SQLSMALLINT colCount;
	//UPDATE, INSERT, DELETE ���� ��ȯ��.
	SQLRETURN ret = SQLRowCount(stmt, &count);
	if (ret != SQL_SUCCESS)
	{
		Check();
	}
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
bool TOdbcQuery::Exec(const TCHAR* sql)
{
	SQLRETURN ret;	
	ret = SQLExecDirect(m_hStmt,(SQLTCHAR*)sql, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	//�ʵ��� ������ ��´�.
	GetDescribeCol(m_hStmt);

	//  SQLGetData�Լ��� ���� ����� �� ����.
	// �Ʒ��� GetRecordData�Լ����� SQLGetData�� ����ϰ� �ִ�.

	/*if (m_ColumnList[0].pfSqlType == SQL_WCHAR ||
		m_ColumnList[0].pfSqlType == SQL_WVARCHAR ||
		m_ColumnList[0].pfSqlType == SQL_WLONGVARCHAR)
	{
		m_ColumnList[0].pfSqlType = SQL_UNICODE;
	}*/
	/*for (int iCol = 0; iCol < this->m_ColumnList.size(); iCol++)
	{
		ret = SQLBindCol(m_hStmt, iCol+1, SQL_WCHAR, m_ColumnList[iCol].bindData, 
			sizeof(m_ColumnList[iCol].bindData), &m_ColumnList[iCol].byteSize);
	}*/	
	
	// ���ڵ��� ����� ��� �����Ѵ�.
	GetRecordData(m_hStmt);
	return true;
}
bool TOdbcQuery::Exec(SQLHSTMT stmt)
{
	SQLRETURN ret;	
	ret = SQLExecute(stmt);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	if (GetRecordData(stmt))
	{
		return true;
	}
	return true;
}
// ���ڵ� ��ȯ ���� ����� Ȯ���Ѵ�.
bool TOdbcQuery::ExecReturn(const TCHAR* sql)
{
	SQLRETURN ret;
	ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	while (SQLMoreResults(m_hStmt) != SQL_NO_DATA);

	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_CLOSE);
	return true;
}
SQLRETURN TOdbcQuery::GetColData(SQLHSTMT stmt, int column, std::wstring& rt)
{
	SQLRETURN ret;
	if (m_ColumnList[column -1].pfSqlType != SQL_VARBINARY)
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
		if (m_bSaveImage)
		{
			rt.clear();
			SQLLEN  indicator;	
			char    smallbuffer[1000] = { 0, };
			ret = SQLGetData(stmt,	column,	SQL_C_BINARY,smallbuffer,sizeof(smallbuffer),&indicator);				
			
			if (ret == SQL_SUCCESS_WITH_INFO)
			{
				std::wstring filename = L"save";
				filename += std::to_wstring(m_RecordResultList.size()) + L"_";
				filename += std::to_wstring(column);// m_ColumnList[column - 1].bindData;
				filename += L".bmp";
				FILE* fp = _tfopen(filename.c_str(), L"w+b");
				if (fp != NULL)
				{
					do {

						rt += reinterpret_cast<TCHAR*>(smallbuffer);

						if (indicator > sizeof(smallbuffer)) // Full buffer
						{
							fwrite(smallbuffer, sizeof(char), sizeof(smallbuffer), fp);
						}
						else
						{
							// Partial buffer 
							fwrite(smallbuffer, sizeof(char), indicator, fp);
							break;
						}
						ret = SQLGetData(stmt, column, SQL_C_BINARY, smallbuffer, sizeof(smallbuffer), &indicator);
						if (ret == SQL_SUCCESS)
						{
							// Partial buffer 
							fwrite(smallbuffer, sizeof(char), indicator, fp);
						}
					} while (ret == SQL_SUCCESS_WITH_INFO);
				}
				fclose(fp);
			}
			return SQL_SUCCESS;
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
	}
	return ret;
}

bool TOdbcQuery::GetRecordData(SQLHSTMT	hStmt)
{
	GetDescribeCol(hStmt);

	SQLRETURN ret;
	while (1)
	{
		ret = SQLFetch(hStmt);
		if (ret == SQL_NO_DATA) break;
		if (ret != SQL_SUCCESS)
		{
			if (GetDiagRec() == false)
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
			if (ret	== SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
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

bool TOdbcQuery::Release()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_hReadStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLDisconnect(m_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	return true;
}

void TOdbcQuery::Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(this->m_hEnv, this->m_hDbc,
		this->m_hStmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}

TOdbcQuery::TOdbcQuery()
{
	m_bSaveImage = true;
}


TOdbcQuery::~TOdbcQuery()
{
}
