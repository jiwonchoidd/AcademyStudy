//#pragma once
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sql.h>
//#include <sqlext.h>
////
////#define MAX_UID_LENGTH 18
////#define MAX_PWD_LENGTH 30
////#define MAX_STMT_LEN   255
////#define MAX_COLUMNS    255
////#ifdef DB2WIN
////#define MAX_TABLES  50
////#else
////#define MAX_TABLES  255
////#endif  
////
////#ifndef max
////#define  max(a,b) (a > b ? a : b)
////#endif
////
////
/////* Macro for common Handle Checking */
////#define HANDLE_CHECK( htype, hndl, SQLRC, phenv, phdbc )              \
////    if ( SQLRC != SQL_SUCCESS )                                       \
////    {   rc = HandleInfoPrint( htype, hndl, SQLRC,                     \
////                          __LINE__, __FILE__) ;                       \
////        if ( rc != 0 )                                                \
////	{   CLIAppCleanUp( phenv, phdbc, 1);                          \
////            return rc;                                                \
////        }		                                              \
////    }
////
/////* Macro for Handle Checking in applications with multiple connections */
////#define MC_HANDLE_CHECK( htype, hndl, SQLRC, phenv, a_hdbc, numConns )  \
////    if ( SQLRC != SQL_SUCCESS )                                         \
////    {   rc = HandleInfoPrint( htype, hndl, SQLRC,                       \
////                          __LINE__, __FILE__) ;                         \
////        if ( rc != 0 )                                                  \
////	{   CLIAppCleanUp( phenv, a_hdbc, numConns );                   \
////            return rc;                                                  \
////        }		                                                \
////    }
////
/////* Macro for Statement Handle Checking */
////#define STMT_HANDLE_CHECK( hstmt, SQLRC )                                \
////    if ( SQLRC != SQL_SUCCESS )                                          \
////    {   rc = HandleInfoPrint( SQL_HANDLE_STMT, hstmt,                    \
////                          SQLRC, __LINE__, __FILE__ ) ;                  \
////        if ( rc == 2 )  StmtResourcesFree( hstmt );                      \
////        if ( rc != 0 )  return rc;                                       \
////    } 
////
/////* Macro for Connection Handle Checking */
////#define DBC_HANDLE_CHECK( hdbc, SQLRC )                                  \
////    if ( SQLRC != SQL_SUCCESS )                                          \
////    {   rc = HandleInfoPrint( SQL_HANDLE_DBC, hdbc,                      \
////                          SQLRC, __LINE__, __FILE__ ) ;                  \
////        if ( rc != 0 )  return rc;                                       \
////    }   
////
/////* Macro for Enironment Handle Checking */
//#define ENV_HANDLE_CHECK( henv, SQLRC )                                  \
//    if ( SQLRC != SQL_SUCCESS )                                          \
//    {   rc = HandleInfoPrint( SQL_HANDLE_ENV, henv,                      \
//                          SQLRC, __LINE__, __FILE__ ) ;                  \
//        if ( rc != 0 )  return rc;                                       \
//    } 
//
/////* functions used in ...CHECK_HANDLE macros */
//int  HandleInfoPrint(SQLSMALLINT, SQLHANDLE, SQLRETURN, int, char *);
////void CLIAppCleanUp(SQLHANDLE *, SQLHANDLE a_hdbc[], int);
////int  StmtResourcesFree(SQLHANDLE);
////
/////* other useful functions with self-explanatory names */
////int CmdLineArgsCheck1(int, char * argv[], char *, char *, char *);
////int CmdLineArgsCheck2(int, char * argv[], char *, char *, char *, char *);
////int CLIAppInit(char *, char *, char *, SQLHANDLE *, SQLHANDLE *, SQLPOINTER);
////int CLIAppTerm(SQLHANDLE *, SQLHANDLE *, char *);
////int StmtResultPrint(SQLHANDLE);