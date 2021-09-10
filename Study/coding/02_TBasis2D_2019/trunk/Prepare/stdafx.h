// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include<windows.h>
#include<sql.h>
#include<sqlext.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#pragma comment (lib, "odbc32.lib")
/*************************************************************************
//http://publib.boulder.ibm.com/html/as400/v5r1/ic2986/index.htm?info/cli/rzadpmst88.htm
** file = typical.c
**
** Example of executing interactive SQL statements, displaying result sets
** and simple transaction management.
**
** Functions used:
**
**        SQLAllocConnect      SQLFreeConnect
**        SQLAllocEnv          SQLFreeEnv
**        SQLAllocStmt         SQLFreeStmt
**        SQLConnect           SQLDisconnect
**
**        SQLBindCol           SQLFetch
**        SQLDescribeCol       SQLNumResultCols
**        SQLError             SQLRowCount
**        SQLExecDirect        SQLTransact
**
**************************************************************************/
#define MAX_STMT_LEN 255
#define  MAXCOLS   100

#define  max(a,b) (a > b ? a : b)

int initialize(SQLHENV *henv,
	SQLHDBC *hdbc);

int process_stmt(SQLHENV    henv,
	SQLHDBC        hdbc,
	SQLCHAR        *sqlstr);

int terminate(SQLHENV henv,
	SQLHDBC hdbc);

int print_error(SQLHENV    henv,
	SQLHDBC        hdbc,
	SQLHSTMT   hstmt);

int check_error(SQLHENV    henv,
	SQLHDBC        hdbc,
	SQLHSTMT   hstmt,
	SQLRETURN  frc);

void display_results(SQLHSTMT hstmt,
	SQLSMALLINT nresultcols);

/*******************************************************************/
static int check_error(SQLHENV    henv,
	SQLHDBC        hdbc,
	SQLHSTMT   hstmt,
	SQLRETURN  frc)
{
	SQLRETURN   rc;

	print_error(henv, hdbc, hstmt);

	switch (frc) {
	case SQL_SUCCESS: break;
	case SQL_ERROR:
	case SQL_INVALID_HANDLE:
		printf("\n ** FATAL ERROR, Attempting to rollback transaction **\n");
		rc = SQLTransact(henv, hdbc, SQL_ROLLBACK);
		if (rc != SQL_SUCCESS)
			printf("Rollback Failed, Exiting application\n");
		else
			printf("Rollback Successful, Exiting application\n");
		terminate(henv, hdbc);
		exit(frc);
		break;
	case SQL_SUCCESS_WITH_INFO:
		printf("\n ** Warning Message, application continuing\n");
		break;
	case SQL_NO_DATA_FOUND:
		printf("\n ** No Data Found ** \n");
		break;
	default:
		printf("\n ** Invalid Return Code ** \n");
		printf(" ** Attempting to rollback transaction **\n");
		SQLTransact(henv, hdbc, SQL_ROLLBACK);
		terminate(henv, hdbc);
		exit(frc);
		break;
	}
	return (SQL_SUCCESS);

}


 /*******************************************************************
 ** process_stmt
 ** - allocates a statement handle
 ** - executes the statement
 ** - determines the type of statement
 **   - if there are no result columns, therefore non-select statement
 **      - if rowcount > 0, assume statement was UPDATE, INSERT, DELETE
 **   else
 **      - assume a DDL, or Grant/Revoke statement
 **   else
 **      - must be a select statement.
 **      - display results
 ** - frees the statement handle
 *******************************************************************/

static int process_stmt(SQLHENV    henv,	SQLHDBC        hdbc,	SQLTCHAR    *sqlstr)
{
	SQLHSTMT        hstmt;
	SQLSMALLINT     nresultcols;
	SQLLEN      rowcount;
	SQLRETURN   rc;


	SQLAllocStmt(hdbc, &hstmt);       /* allocate a statement handle */

									  /* execute the SQL statement in "sqlstr"    */

	rc = SQLExecDirect(hstmt, sqlstr, SQL_NTS);
	if (rc != SQL_SUCCESS)
		if (rc == SQL_NO_DATA_FOUND) {
			printf("\nStatement executed without error, however,\n");
			printf("no data was found or modified\n");
			return (SQL_SUCCESS);
		}
		else
			check_error(henv, hdbc, hstmt, rc);

	SQLRowCount(hstmt, &rowcount);
	rc = SQLNumResultCols(hstmt, &nresultcols);
	if (rc != SQL_SUCCESS)
		check_error(henv, hdbc, hstmt, rc);

	/* determine statement type */
	if (nresultcols == 0) /* statement is not a select statement */
	{
		if (rowcount > 0) /* assume statement is UPDATE, INSERT, DELETE */
		{
			printf("Statement executed, %ld rows affected\n", rowcount);
		}
		else  /* assume statement is GRANT, REVOKE or a DLL statement */
		{
			printf("Statement completed successful\n");
		}
	}
	else /* display the result set */
	{
		display_results(hstmt, nresultcols);
	} /* end determine statement type */

	SQLFreeStmt(hstmt, SQL_DROP);       /* free statement handle */

	return (0);
}/* end process_stmt */

 /*******************************************************************
 ** initialize
 **  - allocate environment handle
 **  - allocate connection handle
 **  - prompt for server, user id, & password
 **  - connect to server
 *******************************************************************/

static int initialize(SQLHENV *henv,	SQLHDBC *hdbc)
{
	SQLTCHAR     server[18],
		uid[10],
		pwd[10];
	SQLRETURN   rc;

	rc = SQLAllocEnv(henv);         /* allocate an environment handle    */
	if (rc != SQL_SUCCESS)
		check_error(*henv, *hdbc, SQL_NULL_HSTMT, rc);

	rc = SQLAllocConnect(*henv, hdbc);  /* allocate a connection handle   */
	if (rc != SQL_SUCCESS)
		check_error(*henv, *hdbc, SQL_NULL_HSTMT, rc);

	printf("Enter Server Name:\n");
	_getts_s( server);
	printf("Enter User Name:\n");
	_getts_s(uid);
	printf("Enter Password Name:\n");
	_getts_s(pwd);

	if (uid[0] == '\0')
	{
		rc = SQLConnect(*hdbc, server, SQL_NTS, NULL, SQL_NTS, NULL, SQL_NTS);
		if (rc != SQL_SUCCESS)
			check_error(*henv, *hdbc, SQL_NULL_HSTMT, rc);
	}
	else
	{
		rc = SQLConnect(*hdbc, server, SQL_NTS, uid, SQL_NTS, pwd, SQL_NTS);
		if (rc != SQL_SUCCESS)
			check_error(*henv, *hdbc, SQL_NULL_HSTMT, rc);
	}
	return 0;
}/* end initialize */

 /*******************************************************************
 ** terminate
 **  - disconnect
 **  - free connection handle
 **  - free environment handle
 *******************************************************************/
static int terminate(SQLHENV henv,	SQLHDBC hdbc)
{
	SQLRETURN   rc;

	rc = SQLDisconnect(hdbc);               /* disconnect from database  */
	if (rc != SQL_SUCCESS)
		print_error(henv, hdbc, SQL_NULL_HSTMT);
	rc = SQLFreeConnect(hdbc);              /* free connection handle    */
	if (rc != SQL_SUCCESS)
		print_error(henv, hdbc, SQL_NULL_HSTMT);
	rc = SQLFreeEnv(henv);                  /* free environment handle   */
	if (rc != SQL_SUCCESS)
		print_error(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT);
	return 0;
}/* end terminate */

 /*******************************************************************
 ** display_results - displays the selected character fields
 **
 **  - for each column
 **      - get column name
 **      - bind column
 **  - display column headings
 **  - fetch each row
 **      - if value truncated, build error message
 **      - if column null, set value to "NULL"
 **      - display row
 **      - print truncation message
 **  - free local storage
 **
 *******************************************************************/
static void display_results(SQLHSTMT hstmt,	SQLSMALLINT nresultcols)
{
	SQLTCHAR         colname[32];
	SQLSMALLINT     coltype[MAXCOLS];
	SQLSMALLINT     colnamelen;
	SQLSMALLINT     nullable;
	SQLULEN       collen[MAXCOLS];
	SQLSMALLINT     scale;
	SQLLEN      outlen[MAXCOLS];
	SQLCHAR *       data[MAXCOLS];
	SQLCHAR         errmsg[256];
	SQLRETURN   rc;
	SQLINTEGER      i;
	SQLLEN      displaysize;

	for (i = 0; i < nresultcols; i++)
	{
		SQLDescribeCol(hstmt, i + 1, colname, sizeof(colname),
			&colnamelen, &coltype[i], &collen[i], &scale, &nullable);

		/* get display length for column */
		SQLColAttributes(hstmt, i + 1, SQL_DESC_PRECISION, NULL, 0,
			NULL, &displaysize);

		/* set column length to max of display length, and column name
		length.  Plus one byte for null terminator       */
		collen[i] = max(displaysize, collen[i]);
		collen[i] = max(collen[i], strlen((char *)colname)) + 1;

		printf("%-*.*s", collen[i], collen[i], colname);

		/* allocate memory to bind column                             */
		data[i] = (SQLCHAR *)malloc(collen[i]);

		/* bind columns to program vars, converting all types to CHAR */
		SQLBindCol(hstmt, i + 1, SQL_C_CHAR, data[i], collen[i], &outlen[i]);
	}
	printf("\n");

	/* display result rows                                            */
	while ((rc = SQLFetch(hstmt)) != SQL_NO_DATA_FOUND)
	{
		errmsg[0] = '\0';
		for (i = 0; i < nresultcols; i++)
		{
			/* Build a truncation message for any columns truncated */
			if (outlen[i] >= collen[i])
			{
				_stprintf((TCHAR *)errmsg + _tcslen((TCHAR *)errmsg),
					_T("%d chars truncated, col %d\n"),
					outlen[i] - collen[i] + 1, i + 1);
			}
			if (outlen[i] == SQL_NULL_DATA)
				printf("%-*.*s", collen[i], collen[i], "NULL");
			else
				printf("%-*.*s", collen[i], collen[i], data[i]);
		} /* for all columns in this row  */

		printf("\n%s", errmsg);  /* print any truncation messages    */
	} /* while rows to fetch */

	  /* free data buffers                                              */
	for (i = 0; i < nresultcols; i++)
	{
		free(data[i]);
	}

}/* end display_results

 /*******************************************************************
 ** SUPPORT FUNCTIONS
 **  - print_error   - call SQLError(), display SQLSTATE and message
 **  - check_error     - call print_error
 **                    - check severity of Return Code
 **                    - rollback & exit if error, continue if warning
 *******************************************************************/

 /*******************************************************************/
static int print_error(SQLHENV    henv,	SQLHDBC        hdbc,	SQLHSTMT   hstmt)
{
	SQLTCHAR     buffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLTCHAR     sqlstate[SQL_SQLSTATE_SIZE + 1];
	SQLINTEGER  sqlcode;
	SQLSMALLINT length;

	while (SQLError(henv, hdbc, hstmt, sqlstate, &sqlcode, buffer,
		SQL_MAX_MESSAGE_LENGTH + 1, &length) == SQL_SUCCESS)
	{
		std::cout << "**** ERROR *****" << std::endl;
		std::cout << "         SQLSTATE:" << sqlstate << std::endl;
		std::cout << "Native Error Code:" << sqlcode << std::endl;
		std::cout << buffer << std::endl;
	};
	return 0;
}
