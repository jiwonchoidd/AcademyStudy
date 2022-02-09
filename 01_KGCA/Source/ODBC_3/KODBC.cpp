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
	//ȯ�� ���� ȯ�� ���� ����
	if (SQLSetEnvAttr(handle_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)//����ó��
	{
		return false;
	}
	//ȯ�� �ڵ� ���� �����ڵ� ���� (ȯ���ڵ��� �θ�)
	if (SQLAllocHandle(SQL_HANDLE_DBC, handle_env, &handle_dbc) != SQL_SUCCESS)
	{
		return false;
	}

	return true;
}
//Ŀ���� Ÿ�Կ� ���� �����ϴ� ����� �޶�����.
bool KODBC::Connect(int type, const TCHAR* dsn_dir)
{
	TCHAR inCon[256] = { 0, };
	TCHAR outCon[256] = { 0, };
	SQLSMALLINT cbOut_Len;
	SQLRETURN ret = 0;
	switch (type)
	{
		//���̷�Ʈ�� ����
		case 0:
		{
			_stprintf(inCon, _T("%s"),
				_T("Driver={SQL Server};Address=121.137.252.85,1433;Network=dbmssocn;Database=USERTBL;Uid=sa;Pwd=1q2w3e4r!;"));
			ret = SQLDriverConnect(handle_dbc, NULL,
				inCon, _countof(inCon),
				outCon, _countof(outCon),
				&cbOut_Len, SQL_DRIVER_NOPROMPT);
		}break;
		//���� dsn
		case 1:
		{
			_stprintf(inCon, _T("Dsn=%s"), dsn_dir);
			ret = SQLConnect(handle_dbc,
				(SQLTCHAR*)dsn_dir, SQL_NTS,
				(SQLTCHAR*)L"sa", SQL_NTS,
				(SQLTCHAR*)L"1q2w3e4r!", SQL_NTS);
		}break;
		//�ý��� dsn
		case 2:
		{
			_stprintf(inCon, _T("FileDsn=%s"), dsn_dir);
			ret = SQLDriverConnect(handle_dbc, NULL,
				inCon, _countof(inCon),
				outCon, _countof(outCon),
				&cbOut_Len, SQL_DRIVER_NOPROMPT);
		}break;
		case 3://ms access ��ȭ���� ����
		{
			HWND hWnd = GetDesktopWindow();
			SQLSMALLINT len;
			ret = SQLDriverConnect(handle_dbc, hWnd,
				(SQLWCHAR*)L"Driver={Microsoft Access Driver (*.mdb, *.accdb)}", SQL_NTS,
				(SQLWCHAR*)inCon, _countof(inCon),
				&len, SQL_DRIVER_PROMPT);
		}break;
		//���� ��ȭ����
		case 4: // //SQL Sever ��ȭ���� ����
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

	//���ῡ ������ �Ŀ� ��� �ڵ�
	if (SQLAllocHandle(SQL_HANDLE_STMT, handle_dbc, &handle_stmt) != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	return true;
}


//�ʱ⿡ ���̺��� ��� ��ȸ�� �� ������ ���ε��ϴ� �۾�
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

	//��������� �� ������ ��ȯ��
	SQLNumResultCols(handle_stmt, &table.table_col_num);
	//������ ���̽��� ���� 0�� �ƴ� 1���� ������
	for (int icol = 1; icol <= table.table_col_num; icol++)
	{
		//�� ���� ����ü ����
		Table_ColumnInfo column;
		//�� �� 
		column.col_num = icol;
		//�� �̸� ��ŭ ������
		int col_size = _countof(column.col_name);

		/*SQLDescribeCol ��� ������ �� ���� ���� ��� ������-
		�� �̸�, ����, �� ũ��, 10 ���� ���� �� null ��� ���θ� ��ȯ �մϴ�. */
		SQLDescribeCol(handle_stmt,
			icol,
			column.col_name,
			col_size,
			&column.col_name_ptr,
			&column.col_data_type,// ��������
			&column.col_size_ptr,
			&column.col_decimal_ptr, // 10���� �ڸ���
			&column.col_nullable);// NULL ��뿩��
		table.table_data.push_back(column);
	}
		//���� Ÿ�� ���ε�
		SQLLEN lTemp = 0;
		TCHAR szData[100][21] = { 0, };
		int   iData[100];
		Table_Record record_data;
		//��� ����ŭ ���鼭 ���� ���ε�
		for (int iBind = 0; iBind < table.table_data.size(); iBind++)
		{
			switch (table.table_data[iBind].col_data_type)
			{
			case SQL_TYPE_TIMESTAMP://�ð� Ÿ���϶�
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
			case SQL_WCHAR://���ڿ� Ÿ���϶�,
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
		//��Ʈ������ ��� ����?
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
	//����� ���� ���ε�
	//ù��° �ʵ�� ���ε� ������ ���ڵ带 ��ȯ���ش�. ���߿� 1�� �ʵ�
	//��� �ڵ鿡 ����� c++ ������ ���ε����� �޸� ����
	ret = SQLBindCol(handle_stmt, 1, SQL_C_ULONG, &user_id, 0, &lid);
	ret = SQLBindCol(handle_stmt, 2, SQL_UNICODE, user_name, sizeof(user_name), &lname);
	ret = SQLBindCol(handle_stmt, 3, SQL_C_ULONG, &user_level, 0, &llevel);
	
	//����� �ޱ����� ���� �ؾ��Ѵ�. �ش��� �ʵ� ����� ���� �Ѵ�.
	//�ʵ� �ȿ� � Ÿ���� �ִ��� Ȯ���ϰ�. ���ε��ؾ��Ѵ�.
	ret = SQLExecDirect(handle_stmt, (SQLTCHAR*)statement, SQL_NTS);

	// ���� ��ȯ�� �� ���ε��� �� ���� ���� �����͸� 
	// �ش� ���� ���ε��� ���ۿ� �ִ´�
	while (SQLFetch(handle_stmt) != SQL_NO_DATA)
	{
		std::wcout << L"��ȣ : " << user_id << L"\t�̸� : " <<
			user_name << L"\t���� : " << user_level << std::endl;
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
