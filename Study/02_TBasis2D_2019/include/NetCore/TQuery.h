#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <vector>
#pragma comment(lib,"odbc32.lib")

///
//ODBC 데이터 원본(64비트) 드라이버 설치
//설치프로그램->Microsoft Access Database Engine 2010 재배포 가능 패키지
//https ://www.microsoft.com/ko-kr/download/details.aspx?id=13255
///

class TQuery
{
public:
	// 모든 컬럼의 값을 스트링으로 저장한다.
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
	// 1)핸들선언
	SQLHENV		m_hEnv = SQL_NULL_HENV; // 환경핸들
	SQLHDBC		m_hDbc = SQL_NULL_HDBC; // 연결핸들
	SQLHSTMT	m_hStmt = SQL_NULL_HSTMT;// 명령핸들
	SQLINTEGER  m_iRetRaram;
public:
	TCHAR		m_szUserID[32] = { 0, };
	TCHAR		m_szUserPS[32] = { 0, };
	SQLHSTMT	m_hLoginStmt = SQL_NULL_HSTMT;// 명령핸들
	SQLHSTMT	m_hLoginStmtCall = SQL_NULL_HSTMT;// 명령핸들
	SQLHSTMT	m_hAccountStmt = SQL_NULL_HSTMT;// 명령핸들
	SQLHSTMT	m_hUserListStmt = SQL_NULL_HSTMT;// 명령핸들
	int			m_iNameLength;
	SQLLEN		m_cbColumn = SQL_NTS;
	SQLLEN		m_cbColumnAccount[2];
	typedef std::vector< std::wstring> RECORD;	
	std::vector< ColDescription>	m_ColumnList;
	RECORD							m_ColumnResultList;
	std::vector< RECORD >			m_RecordResultList;
public:
	void		CreatePrepare();
	SQLRETURN	GetDescribeCol(SQLHSTMT stmt);
	SQLRETURN	GetColData(SQLHSTMT stmt, int column, std::wstring& rt);
	bool		GetRecordData(SQLHSTMT	hStmt);
	bool		ExecPrepare(SQLHSTMT stmt, bool bReturn=false);
	bool		ExecLogin(std::wstring szID);
	bool		ExecLogin(std::wstring szID, std::wstring szPass);
	bool		ExecAccount(std::wstring szID, std::wstring szPS);
public:
	bool	GetDiagRec(SQLHANDLE handle, SQLSMALLINT fHandleType = SQL_HANDLE_STMT);
	void	Check(SQLHSTMT stmt);
public:
	bool	Connect(int iType=0);
	bool	Exec(const TCHAR* sql);
	bool	ExecSelect(const TCHAR * sql, std::wstring& szID, std::wstring& szPS);
	bool	ExecUpdate(const TCHAR * sql);
	bool	ExecDelete(const TCHAR * sql);
	bool	ExecInsert(const TCHAR * sql);
public:
	virtual bool Init();
	virtual bool Release();
public:
	TQuery();
	virtual ~TQuery();
};

