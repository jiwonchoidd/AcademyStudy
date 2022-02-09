#include "KODBC.h"

bool KODBC::Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(handle_env, handle_dbc,
		handle_stmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
	return true;
}
bool KODBC::Init()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &handle_env) != SQL_SUCCESS)
	{
		return false;
	}
	//환경 설정 환경 버젼 선택
	if (SQLSetEnvAttr(handle_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)//버젼처리
	{
		return false;
	}
	//환경 핸들 갖고 접속핸들 만듬 (환경핸들이 부모)
	if (SQLAllocHandle(SQL_HANDLE_DBC, handle_env, &handle_dbc) != SQL_SUCCESS)
	{
		return false;
	}

	return true;
}
//커넥팅 타입에 따라 접속하는 방법이 달라진다.
bool KODBC::Connect(int type, const TCHAR* dsn_dir)
{
	TCHAR inCon[256] = { 0, };
	TCHAR outCon[256] = { 0, };
	SQLSMALLINT cbOut_Len;
	SQLRETURN ret = 0;
	switch (type)
	{
		//다이렉트로 접속
		case 0:
		{
			_stprintf(inCon, _T("%s"),
				_T("Driver={SQL Server};Address=121.137.252.85,1433;Network=dbmssocn;Database=USERTBL;Uid=sa;Pwd=1q2w3e4r!;"));
			ret = SQLDriverConnect(handle_dbc, NULL,
				inCon, _countof(inCon),
				outCon, _countof(outCon),
				&cbOut_Len, SQL_DRIVER_NOPROMPT);
		}break;
		//파일 dsn
		case 1:
		{
			_stprintf(inCon, _T("Dsn=%s"), dsn_dir);
			ret = SQLConnect(handle_dbc,
				(SQLTCHAR*)dsn_dir, SQL_NTS,
				(SQLTCHAR*)L"sa", SQL_NTS,
				(SQLTCHAR*)L"1q2w3e4r!", SQL_NTS);
		}break;
		//시스템 dsn
		case 2:
		{
			_stprintf(inCon, _T("FileDsn=%s"), dsn_dir);
			ret = SQLDriverConnect(handle_dbc, NULL,
				inCon, _countof(inCon),
				outCon, _countof(outCon),
				&cbOut_Len, SQL_DRIVER_NOPROMPT);
		}break;
		case 3://ms access 대화상자 버젼
		{
			HWND hWnd = GetDesktopWindow();
			SQLSMALLINT len;
			ret = SQLDriverConnect(handle_dbc, hWnd,
				(SQLWCHAR*)L"Driver={Microsoft Access Driver (*.mdb, *.accdb)}", SQL_NTS,
				(SQLWCHAR*)inCon, _countof(inCon),
				&len, SQL_DRIVER_PROMPT);
		}break;
		//공통 대화상자
		case 4: // //SQL Sever 대화상자 버젼
		{
			HWND hWnd = GetDesktopWindow();
			SQLSMALLINT len;
			ret = SQLDriverConnect(handle_dbc, hWnd,
				(SQLWCHAR*)L"Driver={SQL Server Native Client 11.0}", SQL_NTS,
				(SQLWCHAR*)inCon, _countof(inCon),
				&len, SQL_DRIVER_PROMPT);
		}break;
	}

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return false;
	}

	//연결에 성공한 후에 명령 핸들
	if (SQLAllocHandle(SQL_HANDLE_STMT, handle_dbc, &handle_stmt) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	return true;
}


//초기에 테이블을 모두 조회한 후 변수를 바인딩하는 작업
bool KODBC::Execute_TableSet(const TCHAR* tablename)
{
	Table_Info table;
	table.table_name = tablename;
	std::wstring statement = L"select * from ";
	statement += tablename;

	SQLRETURN ret = SQLExecDirect(handle_stmt, (SQLTCHAR*)statement.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}

	//결과집합의 콜 열수를 반환함
	SQLNumResultCols(handle_stmt, &table.table_col_num);
	//데이터 베이스는 행이 0이 아닌 1부터 시작함
	for (int icol = 1; icol <= table.table_col_num; icol++)
	{
		//열 정보 구조체 생성
		Table_ColumnInfo column;
		//행 수 
		column.col_num = icol;
		//열 이름 만큼 사이즈
		int col_size = _countof(column.col_name);

		/*SQLDescribeCol 결과 집합의 한 열에 대해 결과 설명자-
		열 이름, 형식, 열 크기, 10 진수 숫자 및 null 허용 여부를 반환 합니다. */
		SQLDescribeCol(handle_stmt,
			icol,
			column.col_name,
			col_size,
			&column.col_name_ptr,
			&column.col_data_type,// 데이터형
			&column.col_size_ptr,
			&column.col_decimal_ptr, // 10진수 자리수
			&column.col_nullable);// NULL 허용여부
		table.table_data.push_back(column);
	}
		//변수 타입 바인딩
		SQLLEN lTemp = 0;
		TCHAR szData[100][21] = { 0, };
		int   iData[100];
		Table_Record record_data;
		//모든 열만큼 돌면서 변수 바인딩
		for (int iBind = 0; iBind < table.table_data.size(); iBind++)
		{
			switch (table.table_data[iBind].col_data_type)
			{
			case SQL_TYPE_TIMESTAMP://시간 타입일때
			{
				Table_Field data;
				data.field_data_type = SQL_UNICODE;
				ret = SQLBindCol(handle_stmt, iBind + 1,
					SQL_TYPE_TIMESTAMP,
					&szData[iBind],
					0,
					&lTemp);
				if (ret != SQL_SUCCESS)
				{
					Check();
					return false;
				}
				record_data.record.push_back(data);
			}break;
			case SQL_WCHAR://문자열 타입일때,
			case SQL_WVARCHAR: {
				Table_Field data;
				data.field_data_type = SQL_UNICODE;
				ret = SQLBindCol(handle_stmt, iBind + 1,
					SQL_UNICODE,
					szData[iBind],
					sizeof(szData[iBind]),
					&lTemp);
				if (ret != SQL_SUCCESS)
				{
					Check();
					return false;
				}
				record_data.record.push_back(data);
			}break;
			case SQL_INTEGER: {
				Table_Field data;
				data.field_data_type = SQL_INTEGER;
				ret = SQLBindCol(handle_stmt, iBind + 1,
					SQL_INTEGER,
					&iData[iBind],
					0,
					&lTemp);
				if (ret != SQL_SUCCESS)
				{
					Check();
					return false;
				}
				record_data.record.push_back(data);
			}break;
			case SQL_REAL: {
				Table_Field data;
				data.field_data_type = SQL_C_FLOAT;
				ret = SQLBindCol(handle_stmt, iBind + 1,
					SQL_C_FLOAT,
					&iData[iBind],
					0,
					&lTemp);
				if (ret != SQL_SUCCESS)
				{
					Check();
					return false;
				}
				record_data.record.push_back(data);
			};
			}
		}
		//스트링으로 모두 저장?
		while (SQLFetch(handle_stmt) != SQL_NO_DATA)
		{
			for (int iCol = 0; iCol < table.table_data.size(); iCol++)
			{
				record_data.record[iCol].field_data_type =
					record_data.record[iCol].field_data_type;
				if (record_data.record[iCol].field_data_type == SQL_UNICODE)
				{
					record_data.record[iCol].field_data = szData[iCol];
				}
				else
				{
					record_data.record[iCol].field_data = std::to_wstring(iData[iCol]);
				}
			}
			table_string_data.push_back(record_data);
		}
		SQLCloseCursor(handle_stmt);
		table_list.push_back(table);
		return true;
}

bool KODBC::Execute_Select(const TCHAR* statement)
{
	SQLLEN lid = SQL_NTS;
	SQLLEN lname = SQL_NTS;
	SQLLEN llevel = SQL_NTS;
	int user_id = 0;
	int user_level = 0;
	TCHAR user_name[20] = { 0, };
	SQLRETURN ret;
	//결과를 저장 바인드
	//첫번째 필드로 바인드 각각의 레코드를 반환해준다. 그중에 1번 필드
	//명령 핸들에 사용자 c++ 변수를 바인딩해줌 메모리 연결
	ret = SQLBindCol(handle_stmt, 1, SQL_C_ULONG, &user_id, 0, &lid);
	ret = SQLBindCol(handle_stmt, 2, SQL_UNICODE, user_name, sizeof(user_name), &lname);
	ret = SQLBindCol(handle_stmt, 3, SQL_C_ULONG, &user_level, 0, &llevel);
	
	//결과를 받기전에 뭐를 해야한다. 해당한 필드 목록을 먼저 한다.
	//필드 안에 어떤 타입이 있는지 확인하고. 바인드해야한다.
	ret = SQLExecDirect(handle_stmt, (SQLTCHAR*)statement, SQL_NTS);

	// 행을 반환할 때 바인딩된 각 열에 대한 데이터를 
	// 해당 열에 바인딩된 버퍼에 넣는다
	while (SQLFetch(handle_stmt) != SQL_NO_DATA)
	{
		std::wcout << L"번호 : " << user_id << L"\t이름 : " <<
			user_name << L"\t레벨 : " << user_level << std::endl;
	}
	SQLCloseCursor(handle_stmt);
	return true;
}

bool KODBC::Execute_Delete(const TCHAR* statement)
{
	return false;
}

bool KODBC::Execute_Insert(const TCHAR* statement)
{
	return false;
}

bool KODBC::Execute_Update(const TCHAR* statement)
{
	return false;
}

bool KODBC::Execute_CreateAccount()
{
	SQLRETURN retcode;
	SWORD sReturn = 0;
	SQLLEN cbRetParam = SQL_NTS;
	retcode = SQLBindParameter(handle_stmt, 1, SQL_PARAM_OUTPUT,
		SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	SQLWCHAR id[10] = L"test";
	retcode = SQLBindParameter(handle_stmt, 2, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id), 0, id, sizeof(id), NULL);
	SQLWCHAR ps[10] = L"11111";
	retcode = SQLBindParameter(handle_stmt, 3, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(ps), 0, ps, sizeof(ps), NULL);

	TCHAR callsp[] = L"{?=call AccountCreate(?,?)}";
	retcode = SQLPrepare(handle_stmt, callsp, SQL_NTS);
	//retcode = SQLExecDirect(hstmt,callsp, SQL_NTS);
	retcode = SQLExecute(handle_stmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return false;
	}
	/*while (SQLFetch(hstmt) != SQL_NO_DATA)
	{
	}*/
	while (SQLMoreResults(handle_stmt) != SQL_NO_DATA);
	SQLFreeStmt(handle_stmt, SQL_UNBIND);
	SQLFreeStmt(handle_stmt, SQL_RESET_PARAMS);
	SQLCloseCursor(handle_stmt);
	return true;
}

bool KODBC::Execute_CheckPW()
{
	SQLRETURN retcode;
	SWORD ret = 0;
	SQLLEN param = SQL_NTS;
	retcode = SQLBindParameter(handle_stmt, 1, SQL_PARAM_OUTPUT,
		SQL_C_SSHORT, SQL_INTEGER, 0, 0, &ret, 0, &param);

	SQLWCHAR id1[10] = L"testuser";
	retcode = SQLBindParameter(handle_stmt, 2, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id1), 0, id1, sizeof(id1), NULL);
	SQLWCHAR outps[10] = { 0, };
	retcode = SQLBindParameter(handle_stmt, 3, SQL_PARAM_OUTPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(outps), 0, outps, sizeof(outps), NULL);

	TCHAR callsp1[] = L"{?=call AccountCheckPW(?,?)}";
	retcode = SQLPrepare(handle_stmt, callsp1, SQL_NTS);
	retcode = SQLExecute(handle_stmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return false;
	}
	while (SQLMoreResults(handle_stmt) != SQL_NO_DATA);
	SQLFreeStmt(handle_stmt, SQL_UNBIND);
	SQLFreeStmt(handle_stmt, SQL_RESET_PARAMS);
	SQLCloseCursor(handle_stmt);
	return true;
}

bool KODBC::Execute(const TCHAR* statement)
{
	return false;
}

bool KODBC::Release()
{
	if (SQLFreeHandle(SQL_HANDLE_STMT, handle_stmt) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	if (SQLDisconnect(handle_dbc) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	if (SQLFreeHandle(SQL_HANDLE_DBC, handle_dbc)!= SQL_SUCCESS)
	{
		Check();
		return false;
	}
	if (SQLFreeHandle(SQL_HANDLE_ENV, handle_env)!= SQL_SUCCESS)
	{
		Check();
		return false;
	}
	return true;
}
