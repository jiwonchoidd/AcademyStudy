#include "KODBC.h"

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

bool KODBC::Connect(const TCHAR* dsn_dir)
{
	//������ �ؾ��� �����ڵ� �������� �ϰڴ�. �������� mdb, accdb ������ �о�´�.
	//���� ��� ��Ʈ��
	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	std::wstring dbpath = dir;
	dbpath += dsn_dir;

	TCHAR inCon[256] = { 0, };
	TCHAR outCon[256] = { 0, };

	//���� Connect. _countof ���ڿ��� ����
	//������ .dsn ������ �԰��� �ؽ�Ʈ ������ �о �����ؾ��Ѵ�. 
	_stprintf(inCon, _T("FileDsn=%s"), dbpath.c_str());

	SQLSMALLINT cbOut_Len;
	SQLRETURN ret = SQLDriverConnect(handle_dbc, NULL, inCon, _countof(inCon),
		outCon, _countof(outCon), &cbOut_Len, SQL_DRIVER_NOPROMPT);

	//������ �����ϸ� success�� �ߴµ� �Ϻ��� ����, �ι�°�� �����Ѵ� ���� 
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
		std::cout << "" << std::endl;
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
