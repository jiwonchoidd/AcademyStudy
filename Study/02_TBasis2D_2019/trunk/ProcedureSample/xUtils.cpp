//#include "xUtils.h"
/////* local functions for util.c */
//void HandleLocationPrint(SQLRETURN, int, char *);
//void HandleDiagnosticsPrint(SQLSMALLINT, SQLHANDLE);
////
/////*#############################################################################
////**    1. HANDLE_CHECK section
////**
////**        1.1 - HandleInfoPrint - prints on the screen everything that
////**                                goes unexpected with a SQL... function.
////**                1.1.1 - HandleLocationPrint    - used by HandleInfoPrint
////**                1.1.2 - HandleDiagnosticsPrint - used by HandleInfoPrint
////**        1.2 - CLIAppCleanUp - rolls back all the transactions
////**                            - closes all the connections
////**                            - frees all the connection handles
////**                            - frees the environment handle
////**        1.3 - StmtResourcesFree - no more comments
////**        1.4 - EnvHandleFree - frees the environment handle
////#############################################################################*/
////
/////******************************************************************************
////**        1.1 - HandleInfoPrint - prints on the screen everything that
////**                                goes unexpected with a SQL... function.
////******************************************************************************/
//int HandleInfoPrint(SQLSMALLINT htype,  /* handle type identifier */
//	SQLHANDLE   hndl,   /* handle used by the SQL...func. */
//	SQLRETURN   sqlrc,  /* ret. code of the SQL... func. */
//	int         line,
//	char *      file)
//{
//	int rc = 0;
//
//	switch (sqlrc)
//	{
//	case SQL_SUCCESS:
//		rc = 0;
//		break;
//	case SQL_INVALID_HANDLE:
//		printf("\n-SQL INVALID HANDLE-----\n");
//		HandleLocationPrint(sqlrc, line, file);
//		rc = 1;
//		break;
//	case SQL_ERROR:
//		printf("\n--SQL ERROR--------------\n");
//		HandleLocationPrint(sqlrc, line, file);
//		HandleDiagnosticsPrint(htype, hndl);
//		rc = 2;
//		break;
//	case SQL_SUCCESS_WITH_INFO:
//		rc = 0;
//		break;
//	case SQL_STILL_EXECUTING:
//		rc = 0;
//		break;
//	case SQL_NEED_DATA:
//		rc = 0;
//		break;
//	case SQL_NO_DATA_FOUND:
//		rc = 0;
//		break;
//	default:
//		printf("\n--default----------------\n");
//		HandleLocationPrint(sqlrc, line, file);
//		rc = 3;
//		break;
//	}
//
//	return (rc);
//}
////
/////*--> SQLL1X32.SCRIPT */
/////******************************************************************************
////**                1.1.1 - HandleLocationPrint    - used by HandleInfoPrint
////******************************************************************************/
//void HandleLocationPrint(SQLRETURN   sqlrc,int         line,char *      file)
//{
//	printf("  sqlrc             = %d\n", sqlrc);
//	printf("  line              = %d\n", line);
//	printf("  file              = %s\n", file);
//}
////
/////******************************************************************************
////**                1.1.2 - HandleDiagnosticsPrint - used by HandleInfoPrint
////******************************************************************************/
//void HandleDiagnosticsPrint(SQLSMALLINT htype, /* handle type identifier */	SQLHANDLE   hndl  /* handle */)
//{
//	SQLWCHAR     message[SQL_MAX_MESSAGE_LENGTH + 1];
//	SQLWCHAR     SQLSTATE[SQL_SQLSTATE_SIZE + 1];
//	SQLINTEGER  sqlcode;
//	SQLSMALLINT length, i;
//
//	i = 1;
//	/*--> 00003364.snippet */
//	while (SQLGetDiagRec(htype,
//		hndl,
//		i,
//		SQLSTATE,
//		&sqlcode,
//		message,
//		SQL_MAX_MESSAGE_LENGTH + 1,
//		&length	) == SQL_SUCCESS) 
//	{
//		printf("\n  SQLSTATE          = %s\n", SQLSTATE);
//		printf("  Native Error Code = %ld\n", sqlcode);
//		printf("%s\n", message);
//		i++;
//	}
//	/* 00003364.snippet <--*/
//
//	printf("-------------------------\n");
//}
////
////
/////******************************************************************************
////**        1.2 - CLIAppCleanUp - rolls back all the transactions
////**                            - closes all the connections
////**                            - frees all the connection handles
////**                            - frees the environment handle
////******************************************************************************/
/////* this function is used in HANDLE_CHECK, */
/////* it can not use HANDLE_CHECK            */
////void CLIAppCleanUp(SQLHANDLE * pHenv,
////	SQLHANDLE a_hdbc[],
////	int a_hdbc_dim)
////{
////	SQLRETURN  sqlrc = SQL_SUCCESS;
////	int        rc = 0;
////	SQLCHAR    db[SQL_MAX_DSN_LENGTH + 1];
////	int        db_nb = 0;
////
////
////	/* rollback all the transactions */
////	for (db_nb = 0; db_nb < a_hdbc_dim; db_nb++)
////	{   /*--> 00003360.snippet */
////		printf("\nRolling back the transaction nb. %d ...\n", db_nb + 1);
////
////		sqlrc = SQLEndTran(SQL_HANDLE_DBC, a_hdbc[db_nb], SQL_ROLLBACK);
////		rc = HandleInfoPrint(SQL_HANDLE_DBC, a_hdbc[db_nb],
////			sqlrc, __LINE__, __FILE__);
////		if (rc == 0)
////		{
////			printf("The transaction nb. %d rolled back.\n", db_nb + 1);
////		}
////		/* 00003360.snippet <--*/
////	}
////
////
////	/* disconnect from all the databases */
////	for (db_nb = 0; db_nb < a_hdbc_dim; db_nb++)
////	{   /*--> 00003334.snippet */
////		printf("\nDisconnecting from the database nb. %d ...\n", db_nb + 1);
////
////		sqlrc = SQLDisconnect(a_hdbc[db_nb]);
////		rc = HandleInfoPrint(SQL_HANDLE_DBC, a_hdbc[db_nb],
////			sqlrc, __LINE__, __FILE__);
////		if (rc == 0)
////		{
////			printf("Disconnected from the database nb. %d.\n", db_nb + 1);
////		}
////		/* 00003334.snippet <--*/
////	}
////
////
////	/* free all the connection handles */
////	for (db_nb = 0; db_nb < a_hdbc_dim; db_nb++)
////	{
////		printf("\nFreeing the connection handle nb. %d ...\n", db_nb + 1);
////		sqlrc = SQLFreeHandle(SQL_HANDLE_DBC, a_hdbc[db_nb]);
////		rc = HandleInfoPrint(SQL_HANDLE_DBC, a_hdbc[db_nb],
////			sqlrc, __LINE__, __FILE__);
////		if (rc == 0)
////		{
////			printf("The connection handle nb. %d is free.\n", db_nb + 1);
////		}
////	}
////
////	/*--> 00003338.snippet */
////	/* free the environment handle */
////	printf("\nFreeing the environment handle ...\n");
////	sqlrc = SQLFreeHandle(SQL_HANDLE_ENV, *pHenv);
////	rc = HandleInfoPrint(SQL_HANDLE_ENV, *pHenv,
////		sqlrc, __LINE__, __FILE__);
////	if (rc == 0)
////	{
////		printf("The environment handle is free.\n");
////	}
////	/* 00003338.snippet <--*/
////}
////
////
/////******************************************************************************
////**        1.3 - StmtResourcesFree - no more comments
////******************************************************************************/
/////* this function is used in STMT_HANDLE_CHECK, */
/////* it can not contain STMT_HANDLE_CHECK        */
////int  StmtResourcesFree(SQLHANDLE hstmt)
////{
////	SQLRETURN   sqlrc = SQL_SUCCESS;
////	int         rc = 0;
////
////	/*--> 00003339.snippet */
////	sqlrc = SQLFreeStmt(hstmt, SQL_UNBIND);
////	rc = HandleInfoPrint(SQL_HANDLE_STMT, hstmt,
////		sqlrc, __LINE__, __FILE__);
////	if (rc != 0) return(1);
////	/* 00003339.snippet <--*/
////
////	sqlrc = SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
////	rc = HandleInfoPrint(SQL_HANDLE_STMT, hstmt,
////		sqlrc, __LINE__, __FILE__);
////	if (rc != 0) return(1);
////
////	/*--> 00003339.snippet */
////	sqlrc = SQLFreeStmt(hstmt, SQL_CLOSE);
////	rc = HandleInfoPrint(SQL_HANDLE_STMT, hstmt,
////		sqlrc, __LINE__, __FILE__);
////	if (rc != 0) return(1);
////	/* 00003339.snippet <--*/
////
////	return(0);
////}
////
////
/////*#############################################################################
////**     2. non HANDLE_CHECK section
////**
////**          2.1 - CmdLineArgsCheck1 - checks the command line arguments, ver.1
////**          2.2 - CmdLineArgsCheck2 - checks the command line arguments, ver.2
////**          2.3 - CLIAppInit - allocates an environment handle
////**                           - alloctes a connection handle
////**                           - set AUTOCOMMIT off or on
////**                           - connects to the database
////**          2.4 - CLIAppTerm - disconnects from the database
////**                           - frees the connection handle
////**                           - frees the environment handle
////**          2.5 - StmtResultPrint - no more comments
////#############################################################################*/
////
/////******************************************************************************
////**          2.1 - CmdLineArgsCheck1 - checks the command line arguments, ver.1
////******************************************************************************/
////int  CmdLineArgsCheck1(int argc,
////	char * argv[],
////	char dbAlias[],
////	char user[],
////	char pswd[])
////{
////	int rc = 0;
////
////	switch (argc)
////	{
////	case 1:
////		strcpy(dbAlias, "sample");
////		strcpy(user, "");
////		strcpy(pswd, "");
////		break;
////	case 2:
////		strcpy(dbAlias, argv[1]);
////		strcpy(user, "");
////		strcpy(pswd, "");
////		break;
////	case 4:
////		strcpy(dbAlias, argv[1]);
////		strcpy(user, argv[2]);
////		strcpy(pswd, argv[3]);
////		break;
////	default:
////		printf("\nUSAGE: %s [dbAlias [userid  passwd]]]\n", argv[0]);
////		rc = 1;
////		break;
////	}  /* endswitch */
////
////	return (rc);
////}
////
/////******************************************************************************
////**          2.2 - CmdLineArgsCheck2 - checks the command line arguments, ver.2
////******************************************************************************/
////int  CmdLineArgsCheck2(int argc,
////	char * argv[],
////	char dbAlias[],
////	char user[],
////	char pswd[],
////	char remoteNodeName[])
////{
////	int rc = 0;
////
////	switch (argc)
////	{
////	case 1:
////		strcpy(dbAlias, "sample");
////		strcpy(user, "");
////		strcpy(pswd, "");
////		strcpy(remoteNodeName, "");
////		break;
////	case 2:
////		strcpy(dbAlias, argv[1]);
////		strcpy(user, "");
////		strcpy(pswd, "");
////		strcpy(remoteNodeName, "");
////		break;
////	case 4:
////		strcpy(dbAlias, argv[1]);
////		strcpy(user, argv[2]);
////		strcpy(pswd, argv[3]);
////		strcpy(remoteNodeName, "");
////		break;
////	case 5:
////		strcpy(dbAlias, argv[1]);
////		strcpy(user, argv[2]);
////		strcpy(pswd, argv[3]);
////		strcpy(remoteNodeName, argv[4]);
////		break;
////	default:
////		printf("\nUSAGE: %s [dbAlias [userid passwd [remoteNodeName]]]\n",
////			argv[0]);
////		rc = 1;
////		break;
////	}  /* endswitch */
////
////	return (rc);
////}
////
/////******************************************************************************
////**          2.3 - CLIAppInit - allocates an environment handle
////**                           - alloctes a connection handle
////**                           - set AUTOCOMMIT off or on
////**                           - connects to the database
////******************************************************************************/
////int CLIAppInit(char dbAlias[],
////	char user[],
////	char pswd[],
////	SQLHANDLE * pHenv,
////	SQLHANDLE * pHdbc,
////	SQLPOINTER autocommitValue)
////{
////	SQLRETURN  sqlrc = SQL_SUCCESS;
////	int        rc = 0;
////
////	/*--> 00003328.snippet */
////	/* allocate an environment handle */
////	sqlrc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, pHenv);
////	if (sqlrc != SQL_SUCCESS)
////	{
////		printf("\n--ERROR while allocating the environment handle.\n");
////		printf("  sqlrc             = %d\n", sqlrc);
////		printf("  line              = %d\n", __LINE__);
////		printf("  file              = %s\n", __FILE__);
////		return(1);
////	}
////	/* 00003328.snippet <--*/
////
////	/*--> 00003328.snippet */
////	/* allocate a database connection handle */
////	sqlrc = SQLAllocHandle(SQL_HANDLE_DBC, *pHenv, pHdbc);
////	HANDLE_CHECK(SQL_HANDLE_ENV, *pHenv, sqlrc, pHenv, pHdbc);
////	/* 00003328.snippet <--*/
////
////	/*--> 00003344.snippet */
////	/* set AUTOCOMMIT off or on */
////	sqlrc = SQLSetConnectAttr(*pHdbc,
////		SQL_ATTR_AUTOCOMMIT,
////		autocommitValue, SQL_NTS);
////	HANDLE_CHECK(SQL_HANDLE_DBC, *pHdbc, sqlrc, pHenv, pHdbc);
////	/* 00000563.snippet <--*/
////
////	/*-->00003344.snippet */
////	/* connect to the database */
////	printf("\nConnecting to %s ...\n", dbAlias);
////	sqlrc = SQLConnect(*pHdbc,
////		(SQLCHAR *)dbAlias, SQL_NTS,
////		(SQLCHAR *)user, SQL_NTS,
////		(SQLCHAR *)pswd, SQL_NTS
////	);
////	HANDLE_CHECK(SQL_HANDLE_DBC, *pHdbc, sqlrc, pHenv, pHdbc);
////	printf("Connected to %s.\n", dbAlias);
////	/* 00000576.snippet <--*/
////
////	return(0);
////}
/////* 00000576.snippet <--*/
////
/////******************************************************************************
////**        2.4 - CLIAppTerm - disconnects from the database
////**                         - frees the connection handle
////**                         - frees the environment handle
////******************************************************************************/
////int CLIAppTerm(SQLHANDLE * pHenv,
////	SQLHANDLE * pHdbc,
////	char dbAlias[])
////{
////	SQLRETURN  sqlrc = SQL_SUCCESS;
////	int        rc = 0;
////
////	printf("\nDisconnecting from %s ...\n", dbAlias);
////	sqlrc = SQLDisconnect(*pHdbc);
////	HANDLE_CHECK(SQL_HANDLE_DBC, *pHdbc, sqlrc, pHenv, pHdbc);
////	printf("Disconnected from %s.\n", dbAlias);
////
////	sqlrc = SQLFreeHandle(SQL_HANDLE_DBC, *pHdbc);
////	HANDLE_CHECK(SQL_HANDLE_DBC, *pHdbc, sqlrc, pHenv, pHdbc);
////
////	sqlrc = SQLFreeHandle(SQL_HANDLE_ENV, *pHenv);
////	HANDLE_CHECK(SQL_HANDLE_ENV, *pHenv, sqlrc, pHenv, pHdbc);
////
////	return(0);
////}
////
////
////
////
/////******************************************************************************
////**          2.5 - StmtResultPrint - no more comments
////******************************************************************************/
////int StmtResultPrint(SQLHANDLE hstmt)
////{
////	SQLRETURN   sqlrc = SQL_SUCCESS;
////	int         rc = 0;
////
////	SQLSMALLINT i; /* index */
////	SQLSMALLINT nResultCols; /* variable for SQLNumResultCols */
////
////	SQLCHAR     colName[32];    /* variables for SQLDescribeCol  */
////	SQLSMALLINT colNameLen;
////	SQLSMALLINT colType;
////	SQLUINTEGER colSize;
////	SQLSMALLINT colScale;
////
////	SQLINTEGER colDataDisplaySize; /* the max. size of the data */
////
////	SQLINTEGER colDisplaySize[MAX_COLUMNS]; /* the max. size of the column */
////
////	struct
////	{
////		SQLCHAR    * buff;
////		SQLINTEGER len;
////		SQLINTEGER buffLen;
////	} outData[MAX_COLUMNS];  /* var. to read the results */
////
////	/*--> 00003342.snippet */
////	/* identify the output columns */
////	sqlrc = SQLNumResultCols(hstmt, &nResultCols);
////	STMT_HANDLE_CHECK(hstmt, sqlrc);
////	/* 00003342.snippet <--*/
////
////	printf("\n");
////	for (i = 0; i < nResultCols; i++)
////	{   /*-->00000581.snippet */
////		sqlrc = SQLDescribeCol(hstmt,
////			(SQLSMALLINT)(i + 1),
////			colName,
////			sizeof(colName),
////			&colNameLen,
////			&colType,
////			&colSize,
////			&colScale,
////			NULL);
////		/* 00000581.snippet <--*/
////
////		STMT_HANDLE_CHECK(hstmt, sqlrc);
////		/*-->00003359.snippet */
////		/* get display size for column */
////		sqlrc = SQLColAttribute(hstmt,
////			(SQLSMALLINT)(i + 1),
////			SQL_DESC_DISPLAY_SIZE,
////			NULL,
////			0,
////			NULL,
////			&colDataDisplaySize);
////		STMT_HANDLE_CHECK(hstmt, sqlrc);
////		/* 00003359.snippet <--*/
////
////		/* set "column display size" to max of "column data display size",
////		   and "column name length". Plus at least one space between
////		   columns.
////		*/
////		colDisplaySize[i] = max(colDataDisplaySize,
////			colNameLen
////		) + 1;
////
////		/* print the column name */
////		printf("%-*.*s",
////			(int)colDisplaySize[i],
////			(int)colDisplaySize[i],
////			colName);
////
////		/* set "output data buffer length" to "column data display size".
////		   Plus one byte for null terminator.
////		*/
////		outData[i].buffLen = colDataDisplaySize + 1;
////
////		/* allocate memory to bind column */
////		outData[i].buff = (SQLCHAR *)malloc((int)outData[i].buffLen);
////
////		/*--> 00003329.snippet */
////		/* bind columns to program vars, converting all types to CHAR */
////		sqlrc = SQLBindCol(hstmt,
////			(SQLSMALLINT)(i + 1),
////			SQL_C_CHAR,
////			outData[i].buff,
////			outData[i].buffLen,
////			&outData[i].len);
////		STMT_HANDLE_CHECK(hstmt, sqlrc);
////		/* 00003329.snippet <--*/
////	}
////
////
////	printf("\n");
////	/*-->00000592.snippet */
////	/* fetch each row and display */
////	sqlrc = SQLFetch(hstmt);
////	if (sqlrc == SQL_NO_DATA_FOUND)
////	{
////		printf("\nData not found.\n");
////	}
////	/* 00000592.snippet <--*/
////	while (sqlrc == SQL_SUCCESS || sqlrc == SQL_SUCCESS_WITH_INFO)
////	{
////		for (i = 0; i < nResultCols; i++)
////		{   /* check for NULL data */
////			if (outData[i].len == SQL_NULL_DATA)
////			{
////				printf("%-*.*s",
////					(int)colDisplaySize[i],
////					(int)colDisplaySize[i],
////					"NULL");
////			}
////			else
////			{   /* print outData for this column */
////				printf("%-*.*s",
////					(int)colDisplaySize[i],
////					(int)colDisplaySize[i],
////					outData[i].buff);
////			}
////		}                          /* for all columns in this row  */
////
////		printf("\n");
////		sqlrc = SQLFetch(hstmt);
////		STMT_HANDLE_CHECK(hstmt, sqlrc);
////	}                              /* while rows to fetch */
////
////	/* free data buffers */
////	for (i = 0; i < nResultCols; i++)
////	{
////		free(outData[i].buff);
////	}
////
////	return(rc);
////}
