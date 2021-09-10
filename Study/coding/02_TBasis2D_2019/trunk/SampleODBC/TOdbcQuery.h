#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>

typedef std::vector< std::wstring> RECORD;

class TOdbcQuery
{
	bool m_bSaveImage;
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
public:
	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLHSTMT	m_hStmt;
	SQLHSTMT	m_hReadStmt;
	std::vector< ColDescription> m_ColumnList;	
	RECORD					m_ColumnResultList;
	std::vector< RECORD >	m_RecordResultList;
public:
	bool	Init();
	bool	Release();
	bool	Connect(int iType, const TCHAR* loadDB,	const TCHAR* UID=nullptr, const TCHAR* PWD = nullptr);
	void	Check();
	bool	Exec(const TCHAR* sql);
	bool	ExecReturn(const TCHAR* sql);
	bool	Exec(SQLHSTMT stmt);
	SQLRETURN	GetDescribeCol(SQLHSTMT stmt);
	SQLRETURN	GetColData(SQLHSTMT stmt, int iCol, std::wstring& rt);
	bool		GetRecordData(SQLHSTMT	hStmt);
	bool		GetDiagRec();
	bool		Prepare(const TCHAR* sql);
public:
	TCHAR		m_szSelectName[32];
	SQLINTEGER	m_iDataLength;
	SQLLEN		m_cbColumn;	
public:
	TOdbcQuery();
	virtual ~TOdbcQuery();
};

