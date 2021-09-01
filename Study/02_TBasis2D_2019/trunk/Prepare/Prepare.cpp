// Prepare.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx
#include "stdafx.h"
#define MAX_COL_NAME_LEN  256
#define MAX_COLS 5

int main() {

	SQLHENV   henv = SQL_NULL_HENV;   // Environment
	SQLHDBC   hdbc = SQL_NULL_HDBC;   // Connection handle
	SQLHSTMT  hstmt = SQL_NULL_HSTMT;  // Statement handle
	SQLRETURN retcode;

	SQLTCHAR *      ColumnName[MAX_COLS];
	SQLSMALLINT    ColumnNameLen[MAX_COLS];
	SQLSMALLINT    ColumnDataType[MAX_COLS];
	SQLULEN        ColumnDataSize[MAX_COLS];
	SQLSMALLINT    ColumnDataDigits[MAX_COLS];
	SQLSMALLINT    ColumnDataNullable[MAX_COLS];
	SQLTCHAR *      ColumnData[MAX_COLS];
	SQLLEN         ColumnDataLen[MAX_COLS];
	SQLSMALLINT    i, j;

	SQLTCHAR Statement[] = _T("select ID, NAME, PASS, GLEVEL, SEX, ACCOUNT, LOGIN, LOGOUT from USERLIST");
	SQLSMALLINT numCols;

	// Initialise buffers
	for (i = 0; i<MAX_COLS; i++) {
		ColumnName[i] = NULL;
		ColumnData[i] = NULL;
	}

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	//CHECK_ERROR(retcode, "SQLAllocHandle(SQL_HANDLE_ENV)",		henv, SQL_HANDLE_ENV);

	// Set the ODBC version environment attribute
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER*)SQL_OV_ODBC3, 0);
	//CHECK_ERROR(retcode, "SQLSetEnvAttr(SQL_ATTR_ODBC_VERSION)",		henv, SQL_HANDLE_ENV);


	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	//CHECK_ERROR(retcode, "SQLAllocHandle(SQL_HANDLE_DBC)",		hdbc, SQL_HANDLE_DBC);


	retcode = SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT,
		(SQLPOINTER)5, 0);
	//CHECK_ERROR(retcode, "SQLSetConnectAttr(SQL_LOGIN_TIMEOUT)",		hdbc, SQL_HANDLE_DBC);


	retcode = SQLConnect(hdbc, (SQLTCHAR*)_T("gameuserlist"), SQL_NTS,(SQLTCHAR*)NULL, 0, NULL, 0);
	//CHECK_ERROR(retcode, "SQLConnect(DATASOURCE)",		hdbc, SQL_HANDLE_DBC);


	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	//CHECK_ERROR(retcode, "SQLAllocHandle(SQL_HANDLE_STMT)",		hstmt, SQL_HANDLE_STMT);

	//��ɹ� �ڵ��� SELECT���� �Բ� ���Ǹ� SQLPrepare()�� ȣ���ϱ� ���� SQLFreeStmt()�� ȣ���Ͽ� Ŀ���� �ݾƾ� �Ѵ�.
	retcode = SQLPrepare(hstmt, Statement, _tcslen(Statement));
	//CHECK_ERROR(retcode, "SQLPrepare(SQL_HANDLE_ENV)",		hstmt, SQL_HANDLE_STMT);

	//���� stmt�� ���õ� ��ɹ��� ������ ��ȯ���� ������, SQLNumResultCols()�� *col�� 0�� �����Ѵ�.
	//�� �Լ��� ȣ���ϱ� ���� SQLPrepare() �Ǵ� SQLExecDirect()�� ȣ���ؾ� �Ѵ�.
	//�� �Լ��� ȣ���� �Ŀ� SQLDescribeCol(), SQLColAttribute(), SQLBindCol() �Ǵ� SQLGetData()�� 
	//ȣ���� �� �ִ�.
	//SQLNumResultCols()�� ���� ��ɹ��� �غ�, ���� �Ǵ� ������ ������ �� ���������� ȣ��� �� �ִ�.
	retcode = SQLNumResultCols(hstmt, &numCols);
	//CHECK_ERROR(retcode, "SQLNumResultCols()", hstmt,		SQL_HANDLE_STMT);

	printf("Number of Result Columns %i\n", numCols);

	for (i = 0; i<numCols; i++)
	{
		ColumnName[i] = (SQLTCHAR *)malloc(MAX_COL_NAME_LEN);
		// ��� ���տ��� �� ���� �̸�, ������ ��, ���� ����, nullability�� �����ش�. 
		//���ø����̼��� ���������� SQLPrepare()�� ȣ�� �� �Ŀ� �׸��� SQLExecute()�� ȣ�� �ϱ� ���� 
		//SQLDescribeCol()�� ȣ���Ѵ�.���� ���ø����̼��� SQLExecDirect()�� ȣ�� �� �Ŀ� 
		//SQLDescribeCol()�� ȣ���� �� �ִ�.
		//SQLDescribeCol()�� SELECT ���� ���� ������ ���� �̸�, Ÿ��, �׸��� ���̸� �˻��Ѵ�.
		//���� ���� ǥ�����̸� *name�� ǥ�����̴�.
		//SQLDescribeCol()�� SELECT���� ���� ������ ��� ��Ʈ�� ǥ�õ� ���� ���� ��� ������ ����(����(column name), ����, ���е�)�� �����Ѵ�.
		//���ø����̼��� ������ ������ �� �Ӽ��� �ʿ��� ��� SQLDescribeCol() ��� SQLColAttributes() �Լ��� ����� �� �ֽ��ϴ�.
		retcode = SQLDescribeCol(
			hstmt,                    // Select Statement (Prepared)
			i + 1,                      // Columnn Number
			ColumnName[i],            // Column Name (returned)
			MAX_COL_NAME_LEN,         // size of Column Name buffer
			&ColumnNameLen[i],        // Actual size of column name
			&ColumnDataType[i],       // SQL Data type of column
			&ColumnDataSize[i],       // Data size of column in table
			&ColumnDataDigits[i],     // Number of decimal digits
			&ColumnDataNullable[i]);  // Whether column nullable

		//CHECK_ERROR(retcode, "SQLDescribeCol()", hstmt, SQL_HANDLE_STMT);

		// Display column data
		printf("\nColumn : %i\n", i + 1);
		printf("Column Name : %s\n  Column Name Len : %i\n  SQL Data Type : %i\n  Data Size : %i\n  DecimalDigits : %i\n  Nullable %i\n",
			ColumnName[i], (int)ColumnNameLen[i], (int)ColumnDataType[i],
			(int)ColumnDataSize[i], (int)ColumnDataDigits[i],
			(int)ColumnDataNullable[i]);

		// Bind column, changing SQL data type to C data type
		// (assumes INT and VARCHAR for now)
		ColumnData[i] = (SQLTCHAR *)malloc(ColumnDataSize[i] + 1);
		switch (ColumnDataType[i])
		{
		case SQL_INTEGER:
			ColumnDataType[i] = SQL_C_LONG;
			break;
		case SQL_VARCHAR:
			ColumnDataType[i] = SQL_C_CHAR;
			break;
		}

		retcode = SQLBindCol(hstmt,                  // Statement handle
			i + 1,                    // Column number
			ColumnDataType[i],      // C Data Type
			ColumnData[i],          // Data buffer
			ColumnDataSize[i],      // Size of Data Buffer
			&ColumnDataLen[i]); // Size of data returned

		//CHECK_ERROR(retcode, "SQLBindCol()", hstmt, SQL_HANDLE_STMT);
	}

	// Fetch records
	printf("\nRecords ...\n\n");
	retcode = SQLExecute(hstmt);
	//CHECK_ERROR(retcode, "SQLExecute()", hstmt, SQL_HANDLE_STMT);

	printf("\n  Data Records\n  ------------\n");
	for (i = 0; ; i++) 
	{
		retcode = SQLFetch(hstmt);

		//No more data?
		if (retcode == SQL_NO_DATA) {
			break;
		}

		//CHECK_ERROR(retcode, "SQLFetch()", hstmt, SQL_HANDLE_STMT);

		//Display it
		printf("\nRecord %i \n", i + 1);

		for (j = 0; j<numCols; j++) 
		{
			printf("Column %s : ", ColumnName[j]);
			if (ColumnDataType[j] == SQL_INTEGER) {
				printf(" %i\n", (int)*ColumnData[j]);
			}
			else {
				printf(" %s\n", _T(" "));
			}
		}
	}

exit:

	for (i = 0; i<numCols; i++) {
		if (ColumnName[i] != NULL) free(ColumnName[i]);
		if (ColumnData[i] != NULL) free(ColumnData[i]);
	}

	printf("\nComplete.\n");

	// Free handles
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

	return 0;
}


//int main()
//{
//	SQLHENV     henv;
//	SQLHDBC     hdbc;
//	SQLTCHAR     sqlstmt[MAX_STMT_LEN + 1] = { 0, };
//	SQLTCHAR     sqltrans[sizeof(_T("ROLLBACK"))];
//	SQLRETURN   rc;
//
//	rc = initialize(&henv, &hdbc);
//	if (rc == SQL_ERROR) return(terminate(henv, hdbc));
//
//	printf("Enter an SQL statement to start a transaction(or 'q' to Quit):\n");
//	//_getts_s(sqlstmt);
//	
//	wsprintf(sqlstmt,_T("%s"), _T("select ID, NAME, PASS, GLEVEL, SEX, ACCOUNT, LOGIN, LOGOUT from USERLIST"));
//
//	while (sqlstmt[0] != 'q')
//	{
//		while (sqlstmt[0] != 'q')
//		{
//			rc = process_stmt(henv, hdbc, sqlstmt);
//			if (rc == SQL_ERROR) return(SQL_ERROR);
//			printf("Enter an SQL statement(or 'q' to Quit):\n");
//			_getts_s(sqlstmt);
//		}
//
//		printf("Enter 'c' to COMMIT or 'r' to ROLLBACK the transaction\n");
//		_fgetts(sqltrans, sizeof("ROLLBACK"), stdin);
//
//		if (sqltrans[0] == 'c')
//		{
//			rc = SQLTransact(henv, hdbc, SQL_COMMIT);
//			if (rc == SQL_SUCCESS)
//				printf("Transaction commit was successful\n");
//			else
//				check_error(henv, hdbc, SQL_NULL_HSTMT, rc);
//		}
//
//		if (sqltrans[0] == 'r')
//		{
//			rc = SQLTransact(henv, hdbc, SQL_ROLLBACK);
//			if (rc == SQL_SUCCESS)
//				printf("Transaction roll back was successful\n");
//			else
//				check_error(henv, hdbc, SQL_NULL_HSTMT, rc);
//		}
//
//		printf("Enter an SQL statement to start a transaction or 'q' to quit\n");
//		_getts_s(sqlstmt);
//	}
//
//	terminate(henv, hdbc);
//
//	return (SQL_SUCCESS);
//}/* end main */


/*******************************************************************
** main
** - initialize
** - terminate
*******************************************************************/
//int main()
//{
//	SQLHENV     henv;
//	SQLHDBC     hdbc;
//	SQLTCHAR     sqlstmt[MAX_STMT_LEN + 1] = { 0, };
//	SQLRETURN   rc;
//
//	rc = initialize(&henv, &hdbc);
//	if (rc == SQL_ERROR) return(terminate(henv, hdbc));
//
//	{SQLHSTMT   hstmt;
//	SQLTCHAR    sqlstmt[] = _T("select ID, NAME, PASS from USERLIST where NAME=?");
//	SQLTCHAR    deptname[15],
//		location[14],
//		division[11];
//	SQLINTEGER id;
//	SQLINTEGER rlength,	plength;
//
//	rc = SQLAllocStmt(hdbc, &hstmt);
//	if (rc != SQL_SUCCESS)
//		check_error(henv, hdbc, SQL_NULL_HSTMT, rc);
//
//	/* prepare statement for multiple use */
//	rc = SQLPrepare(hstmt, sqlstmt, SQL_NTS);
//	if (rc != SQL_SUCCESS)
//		check_error(henv, hdbc, hstmt, rc);
//
//	/* bind division to parameter marker in sqlstmt */
//	rc = SQLSetParam(hstmt, 1, SQL_CHAR, SQL_CHAR, 10, 10, division,&plength);
//	if (rc != SQL_SUCCESS)
//		check_error(henv, hdbc, hstmt, rc);
//
//	/* bind deptname to first column in the result set */
//	rc = SQLBindCol(hstmt, 1, SQL_CHAR, (SQLPOINTER)deptname, 15,
//		&rlength);
//	if (rc != SQL_SUCCESS)
//		check_error(henv, hdbc, hstmt, rc);
//	rc = SQLBindCol(hstmt, 2, SQL_CHAR, (SQLPOINTER)location, 14,
//		&rlength);
//	if (rc != SQL_SUCCESS)
//		check_error(henv, hdbc, hstmt, rc);
//
//	printf("\nEnter Division Name or 'q' to quit:\n");
//	printf("(Eastern, Western, Midwest, Corporate)\n");
//	_getts_s(division);
//	plength = SQL_NTS;
//
//	while (division[0] != 'q')
//	{
//		rc = SQLExecute(hstmt);
//		if (rc != SQL_SUCCESS)
//			check_error(henv, hdbc, hstmt, rc);
//
//		printf("Departments in %s Division:\n", division);
//		printf("DEPTNAME       Location\n");
//		printf("-------------- -------------\n");
//
//		while ((rc = SQLFetch(hstmt)) == SQL_SUCCESS)
//		{
//			printf("%-14.14s %-13.13s \n", deptname, location);
//		}
//		if (rc != SQL_NO_DATA_FOUND)
//			check_error(henv, hdbc, hstmt, rc);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		printf("\nEnter Division Name or 'q' to quit:\n");
//		printf("(Eastern, Western, Midwest, Corporate)\n");
//		_getts_s(division);
//	}
//	}
//
//	rc = SQLTransact(henv, hdbc, SQL_ROLLBACK);
//	if (rc != SQL_SUCCESS)
//		check_error(henv, hdbc, SQL_NULL_HSTMT, rc);
//
//	terminate(henv, hdbc);
//	return (0);
//}/* end main */

/* commit all active transactions on the connection */
//cliRC = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT)
/* ... */

/* rollback all active transactions on the connection */
//cliRC = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);

/* ... */

/* rollback all active transactions on all connections
in this environment */
//cliRC = SQLEndTran(SQL_HANDLE_ENV, henv, SQL_ROLLBACK);