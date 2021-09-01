#define _CRT_SECURE_NO_WARNINGS
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <tchar.h>
#include <string>
#include <iostream>
using namespace std;

class CQuery
{
public:
	// �ִ� �÷���, BLOB ����� ����, NULL �ʵ尪
	enum { MAXCOL=100, BLOBBATCH=10000, CQUERYNULL=-100, CQUERYEOF=-101, 
		CQUERYNOCOL=-102, CQUERYERROR=-103 };
private:
	SQLHENV hEnv;							// ȯ�� �ڵ�
	SQLHDBC hDbc;							// ���� �ڵ�
	SQLRETURN ret;							// ���� ������ SQL���� �����
	TCHAR Col[MAXCOL][255];					// ���ε��� �÷� ����

	int FindCol(TCHAR *name);				// �÷��� �̸����κ��� ��ȣ�� ã���ش�.

public:
	SQLLEN AffectCount;					// ������� ���ڵ� ����
	SQLHSTMT hStmt;							// ��� �ڵ�. ���� ��� ����
	SQLSMALLINT nCol;						// �÷� ����
	SQLTCHAR ColName[MAXCOL][50];			// �÷��� �̸���
	SQLLEN lCol[MAXCOL];				// �÷��� ����/���� ����

	void PrintDiag();						// ���� ���� ���
	CQuery();								// ������
	~CQuery();								// �ı���:���� �ڵ��� �����Ѵ�.
	// ������ �ҽ��� �����Ѵ�.
	BOOL Connect(int Type, const TCHAR *ConStr, const TCHAR *UID=NULL, const  TCHAR *PWD=NULL);
	BOOL Exec(LPCTSTR szSQL);				// SQL���� �����Ѵ�.
	int ExecGetInt(LPCTSTR szSQL);			// SQL���� �����ϰ� ù��° �÷��� ���� ����
	// SQL���� �����ϰ� ù��° �÷��� ���ڿ� ����
	void ExecGetStr(LPCTSTR szSQL, TCHAR *buf);		
	SQLRETURN Fetch();						// �� �� ��������
	void Clear();							// Ŀ�� ���� �� �� ���ε�
	int GetInt(int nCol);					// ������ �÷� �б�
	int GetInt(TCHAR *sCol);				// ������ �÷� �б�
	void GetStr(int nCol, TCHAR *buf);		// ���ڿ��� �÷� �б�
	void GetStr(TCHAR *sCol, TCHAR *buf);	// ���ڿ��� �÷� �б�
	int ReadBlob(LPCTSTR szSQL, void *buf);
	void WriteBlob(LPCTSTR szSQL, void *buf, int size);


	void display_results(SQLHSTMT hstmt, SQLSMALLINT nresultcols);
};