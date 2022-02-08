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
	std::vector<Table_ColumnInfo> table_data;//진짜 데이터를 들고 있음
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
	SQLHENV						handle_env; // 환경핸들
	SQLHDBC						handle_dbc; // 접속핸들
	SQLHSTMT					handle_stmt; // 명령핸들
	std::vector<Table_Info>		table_list; // 테이블 정보를 담은 테이블 리스트
	std::vector<Table_Record>	table_string_data; // 테이블 스트링 정보

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

