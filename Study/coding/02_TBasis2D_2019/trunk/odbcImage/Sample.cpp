#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include <string.h>

/* 1)데이터 베이스 생성 방법*/
//SQL SCRIPT -> 실행 -> 새로고침 -> 확인
/*
	USE master;
	GO

	CREATE DATABASE SAMPLEUSERBLOB;
	GO

	USE SAMPLEUSERBLOB;
	GO

	CREATE TABLE USERBLOBDATA
	(
		TestID int IDENTITY(1, 1),
		BLOBName varChar(50),
		BIRTHDAY datatime,
		BLOBData varBinary(MAX)
	);
	GO

	INSERT INTO BLOBTest(BLOBName, BLOBData)
	SELECT 'First test file', GETDATA(), BulkColumn FROM OPENROWSET( Bulk 'C:\temp\nextup.jpg', SINGLE_BLOB) AS BLOB
*/
/* 2)저장 프로시져 생성 방법 -2-*/
	//SQL SCRIPT -> 실행 -> 새로고침 -> 확인
/*
	USE [SAMPLEUSERBLOB]
	GO
	SET ANSI_NULLS ON
	GO
	SET QUOTED_IDENTIFIER ON
	GO
	CREATE PROCEDURE[dbo].[sp_OutputParameterStreaming] @Param VARBINARY(MAX) OUTPUT
	AS
	BEGIN
	SET NOCOUNT ON;
	--Insert statements for procedure here
	SELECT @Param = BLOBData FROM dbo.USERBLOBDATA  WHERE TestID = 1
	END
	}
*/

/* 3)DSN odbc64 관리자에서 생성-3-*/


#define TEXTSIZE  12000  
#define MAXBUFLEN 256
SQLHENV   henv = SQL_NULL_HENV;
SQLHDBC   hdbc = SQL_NULL_HDBC;
SQLHSTMT  hstmt = SQL_NULL_HSTMT;

void Release()
{
	// Statement
	if (hstmt != SQL_NULL_HSTMT)
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

	// Connection
	if (hdbc != SQL_NULL_HDBC) {
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	}

	// Environment
	if (henv != SQL_NULL_HENV)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

BOOL displaySimilarPicture(BYTE* image, ULONG lengthOfImage, SQLHSTMT hstmt);
BOOL displayPicture(SQLUINTEGER idOfPicture, SQLHSTMT hstmt);

int main()
{
	SQLRETURN retcode, retcode2;
	SQLLEN    lengthofpicture;          // The actual length of the picture
	char      smallbuffer[100];         // A very small buffer
	char      filename[14] = "photo.bmp";
	FILE      *pfile;

	// Allocate an environment handle
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);


	// Notify ODBC that this is an ODBC 3.8 app
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3_80, 0);

	// Allocate ODBC connection handle and connect
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

	// Connect to data source
	SQLCHAR DBName[] = "TestBlob";
	retcode = SQLConnectA(hdbc, DBName, SQL_NTS, (SQLCHAR *)"sa", SQL_NTS, (SQLCHAR *)"kgca!@34", SQL_NTS);

	// Allocate a statement handle
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	////////////////////////////////////////////Put//////////////////////////////////////////////////////	

	///////////////////////////////////////////Get////////////////////////////////////////////////////////////
	BOOL params = TRUE; // TRUE if you want to use parameter markers

	SQLCHAR szCol1[50] = "GAME";
	SQLINTEGER char_len = sizeof(szCol1);
	SQLLEN cbCol1 = char_len;
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, char_len, 0, szCol1, char_len + 1, &cbCol1);

	// Bind the streamed output parameter
	retcode = SQLBindParameter(
		hstmt,
		2,
		SQL_PARAM_OUTPUT_STREAM,   // A streamed output parameterk
		SQL_C_BINARY,
		SQL_VARBINARY,
		0,                         // ColumnSize: The maximum size of varbinary(max)
		0,                         // DecimalDigits is ignored for binary type
		(SQLPOINTER)2,             // ParameterValuePtr: An application-defined
								   // token (this will be returned from SQLParamData)
								   // In this example, we used the ordinal
								   // of the parameter
		0,                         // BufferLength is ignored for streamed output parameters
		&lengthofpicture);         // StrLen_or_IndPtr: The status variable returned

	SQLCHAR sql[] = "{call SP_OutputParameterStreaming2(?,?)}";
	retcode = SQLPrepareA(hstmt, sql, SQL_NTS);

	retcode = SQLExecute(hstmt);

	pfile = fopen(filename, "w+b");
	if (!pfile) {
		printf("failed to open '%s' for w+b\n", filename);
		Release();
	}

	// Assume that the retrieved picture exists. Use SQLBindCol or SQLGetData to retrieve the result-set.
	// Process the result set and move to the streamed output parameters
	if (retcode != SQL_PARAM_DATA_AVAILABLE)
		retcode = SQLMoreResults(hstmt);

	// SQLGetData retrieves and displays the picture in parts.
	// The streamed output parameter is available.
	while (retcode == SQL_PARAM_DATA_AVAILABLE)
	{
		SQLPOINTER token;   // Output by SQLParamData
		SQLLEN cbleft;      // #bytes remained
		retcode = SQLParamData(hstmt, &token);   // Returned token is 2 (according to the binding)
		if (retcode == SQL_PARAM_DATA_AVAILABLE) 
		{
			// A do-while loop retrieves the picture in parts
			do {
				retcode2 = SQLGetData(hstmt, (UWORD)token,SQL_C_BINARY,	smallbuffer, sizeof(smallbuffer),&cbleft);
				if (retcode2 == -1)
				{
					SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
					SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
					SQLINTEGER sqlcode;
					SQLSMALLINT length;
					// 단순한 에러
					SQLError(henv, hdbc, hstmt, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length);
					MessageBox(NULL, (LPTSTR)buffer, (LPTSTR)sqlstate, MB_OK);
				}
				if (cbleft > sizeof(smallbuffer)) // Full buffer
				{
					fwrite(smallbuffer, sizeof(char), sizeof(smallbuffer), pfile);
				}
				else // Partial buffer on last SQLGetData
				{
					fwrite(smallbuffer, sizeof(char), cbleft, pfile);
				}
			} while (retcode2 == SQL_SUCCESS_WITH_INFO);
		}
	}

	// Close the file
	fflush(pfile);
	fclose(pfile);

	printf("\nComplete.\n");
	Release();
	return 0;
}

////////////////// 사용 시나리오: 결과 집합에서 부분에서 이미지를 검색  //////////
// CREATE PROCEDURE SP_TestOutputPara  
//      @ID_of_picture   as int,  
//      @Picture         as varbinary(max) out  
// AS  
//     output the image data through streamed output parameter  
// GO  
BOOL displayPicture(SQLUINTEGER idOfPicture, SQLHSTMT hstmt) {
	SQLLEN      lengthOfPicture;    // The actual length of the picture.  
	BYTE        smallBuffer[100];   // A very small buffer.  
	SQLRETURN   retcode, retcode2;

	// Bind the first parameter (input parameter)  
	SQLBindParameter(
		hstmt,
		1,                         // The first parameter.   
		SQL_PARAM_INPUT,           // Input parameter: The ID_of_picture.  
		SQL_C_ULONG,               // The C Data Type.  
		SQL_INTEGER,               // The SQL Data Type.  
		0,                         // ColumnSize is ignored for integer.  
		0,                         // DecimalDigits is ignored for integer.  
		&idOfPicture,              // The Address of the buffer for the input parameter.  
		0,                         // BufferLength is ignored for integer.  
		NULL);                     // This is ignored for integer.  

  // Bind the streamed output parameter.  
	SQLBindParameter(
		hstmt,
		2,                         // The second parameter.  
		SQL_PARAM_OUTPUT_STREAM,   // A streamed output parameter.   
		SQL_C_BINARY,              // The C Data Type.    
		SQL_VARBINARY,             // The SQL Data Type.  
		0,                         // ColumnSize: The maximum size of varbinary(max).  
		0,                         // DecimalDigits is ignored for binary type.  
		(SQLPOINTER)2,             // ParameterValuePtr: An application-defined  
								   // token (this will be returned from SQLParamData).  
								   // In this example, we used the ordinal   
								   // of the parameter.  
		0,                         // BufferLength is ignored for streamed output parameters.  
		&lengthOfPicture);         // StrLen_or_IndPtr: The status variable returned.   

	SQLTCHAR sql[] = L"{call SP_TestOutputPara(?, ?)}";
	retcode = SQLPrepare(hstmt, sql, SQL_NTS);
	if (retcode == SQL_ERROR)
		return FALSE;

	retcode = SQLExecute(hstmt);
	if (retcode == SQL_ERROR)
		return FALSE;

	// Assume that the retrieved picture exists.  Use SQLBindCol or SQLGetData to retrieve the result-set.  

	// Process the result set and move to the streamed output parameters.  
	retcode = SQLMoreResults(hstmt);

	// SQLGetData retrieves and displays the picture in parts.  
	// The streamed output parameter is available.  
	while (retcode == SQL_PARAM_DATA_AVAILABLE) {
		SQLPOINTER token;   // Output by SQLParamData.  
		SQLLEN cbLeft;      // #bytes remained  
		retcode = SQLParamData(hstmt, &token);   // returned token is 2 (according to the binding)  
		if (retcode == SQL_PARAM_DATA_AVAILABLE) {
			// A do-while loop retrieves the picture in parts.  
			do {
				retcode2 = SQLGetData(
					hstmt,
					(UWORD)token,          // the value of the token is the ordinal.   
					SQL_C_BINARY,           // The C-type.  
					smallBuffer,            // A small buffer.   
					sizeof(smallBuffer),    // The size of the buffer.  
					&cbLeft);               // How much data we can get.  
			} while (retcode2 == SQL_SUCCESS_WITH_INFO);
		}
	}

	return TRUE;
}


// 사용 시나리오: 송신 및 수신 스트리밍된 입/출력 매개 변수로 큰 개체
// CREATE PROCEDURE SP_TestInOut  
//       @picture as varbinary(max) out  
// AS  
//    output the image data through output parameter   
// go  

BOOL displaySimilarPicture(BYTE* image, ULONG lengthOfImage, SQLHSTMT hstmt) 
{
	BYTE smallBuffer[100];   // A very small buffer.  
	SQLRETURN retcode, retcode2;
	SQLLEN statusOfPicture;

	// First bind the parameters, before preparing the statement that binds the output streamed parameter.  
	SQLBindParameter(
		hstmt,
		1,                                 // The first parameter.  
		SQL_PARAM_INPUT_OUTPUT_STREAM,     // I/O-streamed parameter: The Picture.  
		SQL_C_BINARY,                      // The C Data Type.  
		SQL_VARBINARY,                     // The SQL Data Type.  
		0,                                 // ColumnSize: The maximum size of varbinary(max).  
		0,                                 // DecimalDigits is ignored.   
		(SQLPOINTER)1,                     // An application defined token.   
		0,                                 // BufferLength is ignored for streamed I/O parameters.  
		&statusOfPicture);                 // The status variable.  

	statusOfPicture = SQL_DATA_AT_EXEC;   // Input data in parts (DAE parameter at input).  

	SQLTCHAR sql[] = L"call SP_TestInOut(?)";
	retcode = SQLPrepare(hstmt, sql, SQL_NTS);
	if (retcode == SQL_ERROR)
		return FALSE;

	// Execute the statement.  
	retcode = SQLExecute(hstmt);
	if (retcode == SQL_ERROR)
		return FALSE;

	if (retcode == SQL_NEED_DATA) {
		// Use SQLParamData to loop through DAE input parameters.  For  
		// each, use SQLPutData to send the data to database in parts.  

		// This example uses an I/O parameter with streamed output.  
		// Therefore, the last call to SQLParamData should return  
		// SQL_PARAM_DATA_AVAILABLE to indicate the end of the input phrase   
		// and report that a streamed output parameter is available.  

		// Assume retcode is set to the return value of the last call to  
		// SQLParamData, which is equal to SQL_PARAM_DATA_AVAILABLE.  
	}

	// Start processing the streamed output parameters.  
	while (retcode == SQL_PARAM_DATA_AVAILABLE) {
		SQLPOINTER token;   // Output by SQLParamData.  
		SQLLEN cbLeft;     // #bytes remained  
		retcode = SQLParamData(hstmt, &token);
		if (retcode == SQL_PARAM_DATA_AVAILABLE) {
			do {
				retcode2 = SQLGetData(
					hstmt,
					(UWORD)token,          // the value of the token is the ordinal.   
					SQL_C_BINARY,           // The C-type.  
					smallBuffer,            // A small buffer.   
					sizeof(smallBuffer),    // The size of the buffer.  
					&cbLeft);               // How much data we can get.  
			} while (retcode2 == SQL_SUCCESS_WITH_INFO);
		}
	}

	return TRUE;
}