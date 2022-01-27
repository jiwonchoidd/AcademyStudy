#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <string>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
//SQLHANDLE = void* 임 이름만 다름
SQLHENV handle_HENV; // 환경핸들
SQLHDBC handle_HDBC; // 접속핸들
SQLHSTMT handle_HSTMT; // 명령핸들
void Check();
void main()
{
	setlocale(LC_ALL, "");
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &handle_HENV) != SQL_SUCCESS)
	{
		return;
	}
	//환경 설정 환경 버젼 선택
	if (SQLSetEnvAttr(handle_HENV, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)//버젼처리
	{
		return;
	}
	//환경 핸들 갖고 접속핸들 만듬 (환경핸들이 부모)
	if (SQLAllocHandle(SQL_HANDLE_DBC, handle_HENV, &handle_HDBC) != SQL_SUCCESS)
	{
		return;
	}

	//접속을 해야함 유니코드 버젼으로 하겠다. 버젼선택 mdb, accdb 파일을 읽어온다.
	//파일 경로 스트링
	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	std::wstring dbpath = dir;
	dbpath += L"\\User_DB.dsn";

	TCHAR inCon[256] = { 0, };
	TCHAR outCon[256] = { 0, };

	//연결 Connect. _countof 문자열의 길이
	//원래는 .dsn 파일의 규격의 텍스트 파일을 읽어서 연결해야한다. 
	_stprintf(inCon, _T("FileDsn=%s"), dbpath.c_str());
	
	SQLSMALLINT cbOut_Len;
	SQLRETURN ret = SQLDriverConnect(handle_HDBC, NULL, inCon,_countof(inCon),
					outCon, _countof(outCon), &cbOut_Len, SQL_DRIVER_NOPROMPT);

	//접속이 성공하면 success가 뜨는데 완벽한 성공, 두번째는 부족한대 성공 
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, handle_HDBC, &handle_HSTMT) != SQL_SUCCESS)
	{
		return;
	}

	SQLLEN lid = SQL_NTS;
	SQLLEN lname = SQL_NTS;
	SQLLEN llevel = SQL_NTS;
	int user_id = 0;
	int user_level= 0;
	TCHAR user_name[20] = { 0, };

	//결과를 저장 바인드
	//첫번째 필드로 바인드 각각의 레코드를 반환해준다. 그중에 1번 필드
	//명령 핸들에 사용자 c++ 변수를 바인딩해줌 메모리 연결
	ret = SQLBindCol(handle_HSTMT, 1, SQL_C_ULONG, &user_id, 0, &lid);
	ret = SQLBindCol(handle_HSTMT, 2, SQL_UNICODE, user_name, sizeof(user_name), &lname);
	ret = SQLBindCol(handle_HSTMT, 3, SQL_C_ULONG, &user_level, 0, &llevel);

	//쿼리문 SQL문 demogame 테이블에서 모든 값을 출력
	TCHAR sql[MAX_PATH] = L"select * from user_table";
	ret = SQLExecDirect(handle_HSTMT, (SQLTCHAR*)&sql, SQL_NTS);

	// 행을 반환할 때 바인딩된 각 열에 대한 데이터를 
	// 해당 열에 바인딩된 버퍼에 넣는다
	while (SQLFetch(handle_HSTMT) != SQL_NO_DATA)
	{
		std::wcout << L"번호 : " << user_id << L"\t이름 : " <<
			user_name << L"\t레벨 : " << user_level<< std::endl;
	}
	SQLCloseCursor(handle_HSTMT);

	ret = SQLBindCol(handle_HSTMT, 1, SQL_UNICODE, user_name, sizeof(user_name), &lname);
	TCHAR sql2[MAX_PATH]= L"select user_name from user_table where ID = 1";

	ret = SQLExecDirect(handle_HSTMT, (SQLTCHAR*)&sql2, SQL_NTS);
	// 행을 반환할 때 바인딩된 각 열에 대한 데이터를 
	// 해당 열에 바인딩된 버퍼에 넣는다
	while (SQLFetch(handle_HSTMT) != SQL_NO_DATA)
	{
		std::wcout <<L"\n이름 : " <<
			user_name << std::endl;
	}
	SQLCloseCursor(handle_HSTMT);

	SQLFreeHandle(SQL_HANDLE_STMT, handle_HSTMT);
	SQLDisconnect(handle_HDBC);
	SQLFreeHandle(SQL_HANDLE_DBC, handle_HDBC);
	SQLFreeHandle(SQL_HANDLE_ENV, handle_HENV);
}
void Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(handle_HENV, handle_HDBC,
		handle_HSTMT,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}