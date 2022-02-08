#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <string>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <vector>
struct Table_ColumnInfo
{
	SQLUSMALLINT        col_num;
	SQLWCHAR			col_name[20];
	SQLSMALLINT         col_name_length;
	SQLSMALLINT			col_name_ptr;
	SQLSMALLINT			col_data_type;
	SQLULEN				col_size_ptr;
	SQLSMALLINT			col_decimal_ptr;
	SQLSMALLINT			col_nullable;
};
struct Table_Info
{
	std::vector<Table_ColumnInfo> table_data;//��¥ �����͸� ��� ����
	SQLSMALLINT					 table_col_num;
	std::wstring				 table_name;
};
struct Table_Field
{
	std::wstring	field_data;
	int				field_data_type;
};
struct Table_Record
{
	std::vector<Table_Field> record;
};
class KODBC
{
public:
	SQLHENV						handle_env; // ȯ���ڵ�
	SQLHDBC						handle_dbc; // �����ڵ�
	SQLHSTMT					handle_stmt; // ����ڵ�
	std::vector<Table_Info>		table_list; // ���̺� ������ ���� ���̺� ����Ʈ
	std::vector<Table_Record>	table_string_data; // ���̺� ��Ʈ�� ����

public:
	bool Init();
	bool Connect(int type, const TCHAR* dsn_dir);
	bool Check();
	bool Execute_TableSet(const TCHAR* tablename);
	bool Execute_Select(const TCHAR* statement);
	bool Execute_Delete(const TCHAR* statement);
	bool Execute_Insert(const TCHAR* statement);
	bool Execute_Update(const TCHAR* statement);
	bool Execute_CreateAccount();
	bool Execute_CheckPW();
	bool Execute(const TCHAR* statement);
	bool Release();
};

