#include "KODBC.h"

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

bool KODBC::Connect(const TCHAR* dsn_dir)
{
	//접속을 해야함 유니코드 버젼으로 하겠다. 버젼선택 mdb, accdb 파일을 읽어온다.
	//파일 경로 스트링
	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	std::wstring dbpath = dir;
	dbpath += dsn_dir;

	TCHAR inCon[256] = { 0, };
	TCHAR outCon[256] = { 0, };

	//연결 Connect. _countof 문자열의 길이
	//원래는 .dsn 파일의 규격의 텍스트 파일을 읽어서 연결해야한다. 
	_stprintf(inCon, _T("FileDsn=%s"), dbpath.c_str());

	SQLSMALLINT cbOut_Len;
	SQLRETURN ret = SQLDriverConnect(handle_dbc, NULL, inCon, _countof(inCon),
		outCon, _countof(outCon), &cbOut_Len, SQL_DRIVER_NOPROMPT);

	//접속이 성공하면 success가 뜨는데 완벽한 성공, 두번째는 부족한대 성공 
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return false;
	}

	//연결에 성공한 후에 명령 핸들
	if (SQLAllocHandle(SQL_HANDLE_STMT, handle_dbc, &handle_stmt) != SQL_SUCCESS)
	{
		return false;
	}
	return true;
}

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
		int iSize = _countof(column.col_name);

		/*SQLDescribeCol 결과 집합의 한 열에 대해 결과 설명자-
		열 이름, 형식, 열 크기, 10 진수 숫자 및 null 허용 여부를 반환 합니다. */
		SQLDescribeCol(handle_stmt,
			icol,
			column.col_name,
			iSize,
			&column.NameLengthPtr,
			&column.pfSqlType,// 데이터형
			&column.ColumnSizePtr,
			&column.DecimalDigitsPtr, // 10진수 자리수
			&column.pfNullable);// NULL 허용여부
		table.table_data.push_back(column);
	}
		//변수 타입 바인딩
		SQLLEN lTemp;
		TCHAR szData[100][21] = { 0, };
		int   iData[100];
		Table_Record record_data;
		//모든 열만큼 돌면서 변수 바인딩
		for (int iBind = 0; iBind < table.table_data.size(); iBind++)
		{
			switch (table.table_data[iBind].pfSqlType)
			{
			case SQL_WCHAR:
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
			case -7: {
				Table_Field data;
				data.field_data_type = SQL_C_ULONG;
				ret = SQLBindCol(handle_stmt, iBind + 1,
					SQL_C_ULONG,
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

bool KODBC::Execute(const TCHAR* statement)
{
	return false;
}

bool KODBC::Release()
{
	SQLFreeHandle(SQL_HANDLE_STMT, handle_stmt);
	SQLDisconnect(handle_dbc);
	SQLFreeHandle(SQL_HANDLE_DBC, handle_dbc);
	SQLFreeHandle(SQL_HANDLE_ENV, handle_env);
	
	return true;
}
