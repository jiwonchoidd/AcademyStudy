//https://docs.microsoft.com/ko-kr/sql/odbc/microsoft-open-database-connectivity-odbc?view=sql-server-2017
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <string.h>  
#include <windows.h>  
#include <sql.h>  
#include <sqlext.h>  
#include <odbcss.h>  
#include <time.h>  

#define TEXTSIZE  12000  
#define MAXBUFLEN 256  

SQLHENV henv = SQL_NULL_HENV;
SQLHDBC hdbc1 = SQL_NULL_HDBC;
SQLHSTMT hstmt1 = SQL_NULL_HSTMT;

BYTE* g_pFileBuffer = nullptr;
DWORD Read(const TCHAR* filename)
{
	DWORD dwTrans = 0;
	HANDLE hReadFile = CreateFile(filename,	GENERIC_READ, 0,NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	LARGE_INTEGER FileSize;
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &FileSize);
		g_pFileBuffer = new BYTE[FileSize.LowPart];
		DWORD dwLength = FileSize.LowPart;
		BOOL bPending = FALSE;
		BOOL bResult = ReadFile(hReadFile, g_pFileBuffer, dwLength, &dwTrans, NULL);

		if (bResult == FALSE)
		{
			return 0;
		}
	}
	CloseHandle(hReadFile);
	return FileSize.LowPart;
}


void Cleanup() {
	if (hstmt1 != SQL_NULL_HSTMT)
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);

	if (hdbc1 != SQL_NULL_HDBC) {
		SQLDisconnect(hdbc1);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
	}

	if (henv != SQL_NULL_HENV)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

int main() {
	RETCODE retcode;

	// SQLBindParameter variables.  
	SQLLEN cbPutSize, lbytes;

	// SQLParamData variable.  
	PTR pParmID;

	char username[MAX_PATH] = "꼴뚝이";
	// SQLPutData variables.  
	Read(L"../../data/2.bmp");
	
	SDWORD cbBatch = 1000;

	// Allocate the ODBC environment and save handle.  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS)) {
		printf("SQLAllocHandle(Env) Failed\n\n");
		Cleanup();
		return(9);
	}

	// Notify ODBC that this is an ODBC 3.0 app.  
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS)) {
		printf("SQLSetEnvAttr(ODBC version) Failed\n\n");
		Cleanup();
		return(9);
	}

	// Allocate ODBC connection handle and connect.  
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
	if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS)) {
		printf("SQLAllocHandle(hdbc1) Failed\n\n");
		Cleanup();
		return(9);
	}

	retcode = SQLConnectA(hdbc1, (UCHAR*)"TestBlob", SQL_NTS, (SQLCHAR *)"sa", SQL_NTS, (SQLCHAR *)"kgca!@34", SQL_NTS);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLConnect() Failed\n\n");
		Cleanup();
		return(9);
	}

	// Allocate statement handle.  
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLAllocHandle(hstmt1) Failed\n\n");
		Cleanup();
		return(9);
	}

	// Set parameters based on total data to send.  
	lbytes = (SDWORD)_msize(g_pFileBuffer);
	cbPutSize = SQL_LEN_DATA_AT_EXEC(lbytes);

	SQLLEN cbRestData = SQL_DATA_AT_EXEC;
	retcode = SQLBindParameter(hstmt1, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, lbytes, 0,
		(SQLPOINTER)g_pFileBuffer, lbytes, &cbPutSize);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) 
	{
		SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
		SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
		SQLINTEGER sqlcode;
		SQLSMALLINT length;
		// 단순한 에러
		SQLError(henv, hdbc1, hstmt1, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length);
		MessageBox(NULL, (LPTSTR)buffer, (LPTSTR)sqlstate, MB_OK);
		Cleanup();
		return(9);
	}

	//https://downman.tistory.com/203
	time_t curr_time;
	// 시간 표시를 위한 구조체를 선언합니다.
	struct tm *curr_tm;
	// time 함수는 1970년 1월 1일 이후 몇초가 지났는지를 계산합니다. NULL을 인자로 사용합니다.
	curr_time = time(NULL);
	// 지역 시간을 기준으로 변환 및 출력 편의를 위하여 tm 구조체에 저장합니다.(포맷팅)
	curr_tm = localtime(&curr_time);

	char* timeday = asctime(curr_tm);
	char* timeday2 = ctime(&curr_time);
	char tmbuf[1024] = { 0, };
	strftime(tmbuf, 1024, "%F %T", curr_tm);
		
	char szSQL[MAX_PATH] = { 0, };
	sprintf(szSQL, "INSERT INTO USERBLOBDATA(BLOBName,BIRTHDAY,BLOBData) VALUES( '%s', '%s', ?)", username, tmbuf);

	retcode = SQLExecDirectA(hstmt1, (UCHAR*)szSQL, SQL_NTS);
		//SQLExecDirectA(hstmt1, (UCHAR*)"INSERT INTO USERBLOBDATA(BLOBName,BIRTHDAY,BLOBData) VALUES( 'GAME', '1950-11-14 00:00:00', ?)", SQL_NTS);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_NEED_DATA) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLExecDirect Failed\n\n");
		Cleanup();
		return(9);
	}

	// 정의된 매개변수 ID를 얻는다. 다른 실행 시 데이터 매개변수가 있는 경우 SQL_NEED_DATA가 반환횐다.
	retcode = SQLParamData(hstmt1, &pParmID);
	if (retcode == SQL_NEED_DATA) 
	{
		SQLLEN pos = 0;
		while (lbytes > cbBatch) 
		{
			// SQLPutData를 한번 이상 호출하여 길이가 전달될 때까지 매개변수 데이터를 보낸다.
			SQLPutData(hstmt1, &g_pFileBuffer[pos], cbBatch);
			lbytes -= cbBatch;
			pos += cbBatch;
		}		
		// 잔여량
		retcode = SQLPutData(hstmt1, &g_pFileBuffer[pos], lbytes);
	}

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) 
	{
		printf("SQLParamData Failed\n\n");
		Cleanup();
		return(9);
	}
	
	// 최종 실행시 데이터 매개변수의 모든 데이터가 전송되었음을 알린다.  
	retcode = SQLParamData(hstmt1, &pParmID);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) 
	{
		SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
		SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
		SQLINTEGER sqlcode;
		SQLSMALLINT length;
		// 단순한 에러
		SQLError(henv, hdbc1, hstmt1, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length);
		MessageBox(NULL, (LPTSTR)buffer, (LPTSTR)sqlstate, MB_OK);

		// 모든 에러 나열됨.
		int iErrorCnt = 1;
		while (retcode = SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, iErrorCnt++, sqlstate, &sqlcode,
			buffer, sizeof(buffer), &length) != SQL_NO_DATA)
		{
			MessageBox(NULL, (LPTSTR)buffer, (LPTSTR)sqlstate, MB_OK);
		}
		Cleanup();
		return(9);
	}

	delete(g_pFileBuffer);

	// Clean up.  
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
	SQLDisconnect(hdbc1);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
// 샘플 참고
//https://wwwi.tistory.com/82 timedate teimestamp

//BOOL params = TRUE; // TRUE if you want to use parameter markers
//SQLINTEGER char_len = 10, blob_len = 400;
//SQLCHAR szCol1[21], szCol2[400], szRecCol1[21], szRecCol2[400];
//SQLINTEGER cbCol1, cbCol2;
//SQLCHAR stmt[2048];
//
//// Create a table with a character column and a BLOB column
//rc = SQLExecDirect(hstmt, "CREATE TABLE TABBLOB(COL1 CHAR(10), COL2 BLOB(400))", SQL_NTS);
//
//strcpy(szCol1, "1234567890");
//if (!params)  // no parameter markers
//{
//	strcpy(szCol2, "414243444546"); // 0x41 = 'A', 0x42 = 'B', 0x43 = 'C', ...
//	wsprintf(stmt, "INSERT INTO TABBLOB VALUES('%s', BLOB(x'%s'))", szCol1, szCol2);
//}
//else
//{
//	strcpy(szCol2, "ABCDEF"); // 'A' = 0x41, 'B' = 0x42, 'C' = 0x43, ...        
//	strcpy(stmt, "INSERT INTO TABBLOB VALUES(?,?)");
//}
//
//// Prepare the 'Insert' statement
//rc = SQLPrepare(hstmt, stmt, SQL_NTS);
//
//// Bind the parameter markers 
//if (params)  // using parameter markers 
//{
//	cbCol1 = char_len;
//	rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
//		char_len, 0, szCol1, char_len + 1, &cbCol1);
//
//	cbCol2 = 6;
//	rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
//		blob_len, 0, szCol2, blob_len, &cbCol2);
//}
//
//// Execute the 'Insert' statement to put a row of data into the table
//rc = SQLExecute(hstmt);
//
//// Prepare and Execute a 'Select' statement
//rc = SQLExecDirect(hstmt, "SELECT * FROM TABBLOB", SQL_NTS);
//
//// Bind the columns
//rc = SQLBindCol(hstmt, 1, SQL_C_CHAR, szRecCol1, char_len + 1, &cbCol1);
//rc = SQLBindCol(hstmt, 2, SQL_C_BINARY, szRecCol2, blob_len, &cbCol2);
//
//// Fetch the first row
//rc = SQLFetch(hstmt);
//szRecCol2[cbCol2] = '\0';
//
//// At this point szRecCol1 should contain the data "1234567890"  
//// szRecCol2 should contain the data 0x414243444546 or "ABCDEF"