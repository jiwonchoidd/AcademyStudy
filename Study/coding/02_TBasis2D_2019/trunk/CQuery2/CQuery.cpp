
// API ������Ʈ�� ���� ��� ���� �� ���� ���Խ�Ų��.
#include <windows.h>
#include "CQuery.h"

// MFC ������Ʈ�� ���� ��� stdafx.h�� CQuery.h�� �����Ѵ�.
// #include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
/*
CQuery Ŭ����:ODBC�� ���� Ŭ����. API ���ؿ��� ���� �����ϵ��� �ۼ��Ͽ���.
SQL���� ���ϰ� �����ϰ� ����� ���� �дµ� ������ ���߾����� SQL ������
�׼������� �Ϻ��ϰ� �׽�Ʈ�Ǿ���.
255�� ������ ����, ���ڿ� �÷��� ���ؼ��� ��� �����ϸ� �� �̻��� ���̸� ��
���� �ʵ�� "������ �߸�" ���� ��� �߻��ϹǷ� �� Ŭ������ ���� �� ������
ODBC API �Լ��� ���� ����ؾ� �Ѵ�.
�ִ� �÷� ������ 100���̴�. �ʰ��� �������� ���� �޽��� �ڽ��� ����ϹǷ� ��
�ѵ��� �˾Ƽ� ���� �ʵ��� �����ؾ� �Ѵ�. Ư�� �ʵ���� ���� ���̺��� select *
�� �д� ���� �ﰡ�ϴ� ���� ����.
���� ���� Ŀ���� ����ϹǷ� �̹� ���� ���ڵ�� ������ �ٽ� �����ؾ߸� ���� ��
�ִ�.

����
1.CQuery Ŭ������ ��ü�� �����Ѵ�. �������̸� �������� �����ϴ� ���� ������
�ʿ��� ����ŭ ��ü�� �����. ���� �� �� ������ ����ϴ�.
2.������ Connect �Լ��� ȣ���Ͽ� ������ �ҽ��� �����Ѵ�.
3.Exec �Լ��� SQL���� �����Ѵ�. ���� ó���� Exec �Լ� ���ο��� ó���Ѵ�.
4.Fetch �Լ��� ��� �� �ϳ��� �����´�. �������� ������� �ִ� ���� while
������ ���鼭 ���ʴ�� ������ �� �ִ�.
5.Get* �Լ��� �÷� ���� �д´�.
6.Clear �Լ��� ��� �ڵ��� �ݴ´�.(Select���� ��츸)
7.��ü�� �ı��Ѵ�. ���� ��ü�� ���� �Ϻη� �ı��� �ʿ䰡 ����.
*/
static char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0 };
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength, // ���
		NULL, NULL);
	return retData;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength); // ���
	return retData;
}
// ������:���� �ʱ�ȭ�� ����Ѵ�.
CQuery::CQuery()
{
	AffectCount = -1;
	ret = SQL_SUCCESS;

	// ȯ�� �ڵ��� �Ҵ��ϰ� ���� �Ӽ��� �����Ѵ�.
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
}

// �ı���:���� �ڵ��� �����Ѵ�.
CQuery::~CQuery()
{
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

// ���� �ڵ��� �Ҵ��ϰ� ������ �� ����ڵ���� ���� �Ҵ��Ѵ�.
// Type=1:ConStr�� MDB ������ ��θ� ������. ��� ������ ���� ���丮���� MDB�� ã�´�.
// Type=2:ConStr�� SQL ������ ���� ������ ������ DSN ������ ��θ� ������. 
//        ��δ� �ݵ�� ���� ��η� �����ؾ� �Ѵ�.
// Type=3:SQLConnect �Լ��� DSN�� ���� �����Ѵ�.
// ���� �Ǵ� ��� �ڵ� �Ҵ翡 �����ϸ� FALSE�� �����Ѵ�.
BOOL CQuery::Connect(int Type, TCHAR *ConStr, TCHAR *UID, TCHAR *PWD)
{
	SQLTCHAR InCon[255] = { 0, };
	SQLTCHAR OutCon[255] = { 0, };
	SQLSMALLINT cbOutCon;

	int ii = 1;
	SQLRETURN Ret;
	SQLINTEGER NativeError;
	SQLTCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	TCHAR str[256];
	SQLSMALLINT size = sizeof(OutCon);
	// ���� Ÿ�Կ� ���� MDB �Ǵ� SQL ����, �Ǵ� DSN�� �����Ѵ�.
	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
	switch (Type) {
	case 1:
	{
		TCHAR Dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, Dir);
		wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, ConStr);
		ret = SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)InCon, sizeof(InCon), OutCon,
			sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	}break;
	case 2:
		wsprintf((TCHAR *)InCon, _T("FileDsn=%s"), ConStr);
		ret = SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)InCon, wcslen(InCon), NULL,
			0, &cbOutCon, SQL_DRIVER_NOPROMPT);
		break;
	case 3:
		ret = SQLConnect(hDbc, (SQLTCHAR *)ConStr, SQL_NTS, (SQLTCHAR *)UID, SQL_NTS,
			(SQLTCHAR *)PWD, SQL_NTS);
		break;
	}

	// ���� ������ ���� ������ �����ش�.
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) {
		while (Ret = SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, ii, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
			_stprintf(str, _T("SQLSTATE:%s, ��������:%s"), (LPCTSTR)SqlState, (LPCTSTR)Msg);
			::MessageBox(NULL, str, _T("���� ����"), 0);
			ii++;
		}
		return FALSE;
	}

	// ��� �ڵ��� �Ҵ��Ѵ�.
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) {
		hStmt = 0;
		return FALSE;
	}
	return TRUE;
}

// SQL���� �����Ѵ�. ���н� ���� ������ ����ϰ� FALSE�� �����Ѵ�.
BOOL CQuery::Exec(LPCTSTR szSQL)
{
	int c;

	// SQL���� �����Ѵ�. SQL_NO_DATA�� ������ ��쵵 �ϴ� �������� ����Ѵ�. 
	// �� ��� Fetch���� EOF�� �����ϵ��� �Ǿ� �ֱ� ������ ���� ������ ����� �ʿ�� ����.
	ret = SQLExecDirect(hStmt, (SQLTCHAR *)szSQL, SQL_NTS);
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) && (ret != SQL_NO_DATA)) {
		PrintDiag();
		return FALSE;
	}

	// Update, Delete, Insert ��ɽ� ������� ���ڵ� ������ ���� ���´�.
	SQLRowCount(hStmt, &AffectCount);

	SQLNumResultCols(hStmt, &nCol);
	if (nCol > MAXCOL) {
		::MessageBox(NULL, _T("�ִ� �÷� ���� �ʰ��߽��ϴ�"), _T("CQuery ����"), MB_OK);
		return FALSE;
	}

	// nCol�� 0�� ���� Select�� �̿��� �ٸ� ����� ������ ����̹Ƿ� 
	// ���ε��� �� �ʿ䰡 ����.
	if (nCol == 0) {
		Clear();
		return TRUE;
	}

	// ��� �÷��� ���ڿ��� ���ε��� ���´�. Col�迭�� zero base, 
	// �÷� ��ȣ�� one base�ӿ� ������ ��
	for (c = 0; c<nCol; c++)
	{
		SQLBindCol(hStmt, c + 1, SQL_C_CHAR, Col[c], 255, &lCol[c]);
		SQLDescribeCol(hStmt, c + 1, ColName[c], 30, NULL, NULL, NULL, NULL, NULL);
	}
	return TRUE;
}

// SQL���� �����ϰ� ������� ù Į������ �������� �о� ������ �ش�. 
// ����� ���� �� �������� �� �ش�.
int CQuery::ExecGetInt(LPCTSTR szSQL)
{
	int i;

	if (Exec(szSQL) == FALSE)
		return CQUERYERROR;

	// �����Ͱ� ���� ��� EOF�� �����Ѵ�.
	if (Fetch() == SQL_NO_DATA) {
		Clear();
		return CQUERYEOF;
	}
	i = GetInt(1);
	Clear();
	return i;
}

// SQL���� �����ϰ� ������� ù Į������ ���ڿ��� �о� ������ �ش�.
void CQuery::ExecGetStr(LPCTSTR szSQL, TCHAR *buf)
{
	// SQL�� ������ ������ �߻��� ��� ���ڿ� ���ۿ� ������ ���� ������.
	if (Exec(szSQL) == FALSE) {
		lstrcpy(buf, _T("CQUERYERROR"));
		return;
	}

	// �����Ͱ� ���� ��� EOF�� �����Ѵ�.
	if (Fetch() == SQL_NO_DATA) {
		Clear();
		lstrcpy(buf, _T("EOF"));
		return;
	}
	GetStr(1, buf);
	Clear();
}

// ����¿��� �� ���� �����´�.
SQLRETURN CQuery::Fetch()
{
	ret = SQLFetch(hStmt);
	return ret;
}

// Ŀ���� �ݰ� ���ε� ������ �����Ѵ�.
void CQuery::Clear()
{
	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
}

// �÷� �̸����κ��� �÷� �ε����� ã�´�. ���� ��� -1�� �����Ѵ�.
int CQuery::FindCol(TCHAR *name)
{
	int i;
	for (i = 0; i<nCol; i++) {
		if (_tcsicmp(name, (LPCTSTR)ColName[i]) == 0)
			return i + 1;
	}
	return -1;
}

// nCol�� �÷����� ������ �о��ش�. NULL�� ��� CQUERYNULL�� �����Ѵ�.
int CQuery::GetInt(int nCol)
{
	if (nCol > this->nCol)
		return CQUERYNOCOL;

	if (lCol[nCol - 1] == SQL_NULL_DATA) {
		return CQUERYNULL;
	}
	else {
		return _ttoi(Col[nCol-1]);
		//return atoi(Col[nCol - 1]);
	}
}

// sCol�� �÷����� ������ �о��ش�.
int CQuery::GetInt(TCHAR *sCol)
{
	int n;
	n = FindCol(sCol);
	if (n == -1) {
		return CQUERYNOCOL;
	}
	else {
		return GetInt(n);
	}
}

// nCol�� �÷����� ���ڿ��� �о��ش�. NULL�� ��� ���ڿ��� NULL�� ä���ش�. 
// buf�� ���̴� �ּ��� 256�̾�� �ϸ� ���� ������ ���� �ʴ´�.
void CQuery::GetStr(int nCol, TCHAR *buf)
{
	if (nCol > this->nCol) {
		lstrcpy(buf, _T("CQUERYNOCOL"));
		return;
	}

	if (lCol[nCol - 1] == SQL_NULL_DATA) {
		lstrcpy(buf, _T("NULL"));
	}
	else {
		lstrcpy(buf, Col[nCol - 1]);
	}
}

// sCol�� �÷����� ���ڿ��� �о��ش�.
void CQuery::GetStr(TCHAR *sCol, TCHAR *buf)
{
	int n;
	n = FindCol(sCol);
	if (n == -1) {
		lstrcpy(buf, _T("ERROR:Colume not found"));
	}
	else {
		GetStr(n, buf);
	}
}

// ���� �߻��� ���� ������ ����� �ش�.
void CQuery::PrintDiag()
{
	int ii;
	SQLRETURN Ret;
	SQLINTEGER NativeError;
	SQLTCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	TCHAR str[256];

	ii = 1;
	while (Ret = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, ii, SqlState, &NativeError,
		Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		wsprintf(str, _T("SQLSTATE:%s, ��������:%s"), (LPCTSTR)SqlState, (LPCTSTR)Msg);
		::MessageBox(NULL, str, _T("���� ����"), 0);
		ii++;
	}

	// ���� �Ұ����� ������ �߻��� ��� ���α׷��� �����Ѵ�. �ش��� ���� ó���� ���ʿ���
	// ���� �� �ڵ带 �ּ� ó���ϰų� �����ϰ� �ٸ� ��ƾ���� �ٲ�� �Ѵ�.
	/*	if (ii > 1) {
	::MessageBox(NULL,"���� ������ ��µǾ����ϴ�. �� ������ ��µ� ���� ��Ʈ�� ����, DB ���� ���� ����\r\n"
	"���� �Ұ����� ������ �߻��� ����̸� ���α׷� ������ ����� �� �����ϴ�.\r\n"
	"������ �����Ͻ� �� ���α׷��� �ٽ� ������ �ֽʽÿ�.","Critical Error",MB_OK | MB_ICONERROR);

	// ���� �� �� �ϳ��� ������ ��
	PostQuitMessage(0);
	// ExitProcess(0);
	}
	*/
}

// BLOB �����͸� buf�� ä���ش�. �̶� buf�� ����� ũ���� �޸𸮸� �̸� �Ҵ��� 
// ���ƾ� �Ѵ�. NULL�� ��� 0�� �����ϰ� ���� �߻��� -1�� �����Ѵ�. ������ ���� 
// �� ����Ʈ ���� �����Ѵ�. szSQL�� �ϳ��� BLOB �ʵ带 �д� Select SQL���̾�� �Ѵ�.
int CQuery::ReadBlob(LPCTSTR szSQL, void *buf)
{
	SQLCHAR BinaryPtr[BLOBBATCH];
	SQLLEN LenBin;
	TCHAR *p;
	int nGet;
	int TotalGet = 0;

	ret = SQLExecDirect(hStmt, (SQLTCHAR *)szSQL, SQL_NTS);
	if (ret != SQL_SUCCESS) {
		PrintDiag();
		return -1;
	}

	while ((ret = SQLFetch(hStmt)) != SQL_NO_DATA) {
		p = (TCHAR *)buf;
		while ((ret = SQLGetData(hStmt, 1, SQL_C_BINARY, BinaryPtr, sizeof(BinaryPtr),
			&LenBin)) != SQL_NO_DATA) {
			if (LenBin == SQL_NULL_DATA) {
				Clear();
				return 0;
			}
			if (ret == SQL_SUCCESS)
				nGet = LenBin;
			else
				nGet = BLOBBATCH;
			TotalGet += nGet;
			memcpy(p, BinaryPtr, nGet);
			p += nGet;
		}
	}
	Clear();
	return TotalGet;
}

// buf�� BLOB �����͸� �����Ѵ�. szSQL�� �ϳ��� BLOB �����͸� �����ϴ� Update, Insert
// SQL���̾�� �Ѵ�.
void CQuery::WriteBlob(LPCTSTR szSQL, void *buf, int size)
{
	SQLLEN cbBlob;
	char tmp[BLOBBATCH], *p;
	SQLPOINTER pToken;
	int nPut;

	cbBlob = SQL_LEN_DATA_AT_EXEC(size);
	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
		size, 0, (SQLPOINTER)1, 0, &cbBlob);
	SQLExecDirect(hStmt, (SQLTCHAR *)szSQL, SQL_NTS);
	ret = SQLParamData(hStmt, &pToken);
	while (ret == SQL_NEED_DATA) {
		if (ret == SQL_NEED_DATA) {
			if ((int)pToken == 1) {
				for (p = (char *)buf; p<(char *)buf + size; p += BLOBBATCH) {
					nPut = min(BLOBBATCH, (char *)buf + size - p);
					memcpy(tmp, p, nPut);
					SQLPutData(hStmt, (PTR)tmp, nPut);
				}
			}
		}
		ret = SQLParamData(hStmt, &pToken);
	}
	Clear();
}