#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"

// �ڵ�
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

LRESULT CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL DBConnect();
void DBDisConnect();
BOOL DBExecuteSQL();
HINSTANCE g_hInst;
HWND hDlgMain;
HWND hList,hFName,hFPrice,hFKorean;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	
	if (DBConnect() == FALSE) {
		MessageBox( NULL,_T("������ ���̽��� ������ �� �����ϴ�"),_T("����"),MB_OK);
		return 0;
	}

	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOGMAIN), HWND_DESKTOP,(DLGPROC)MainDlgProc);

	DBDisConnect();
	return 0;
}

// ���� �߻��� ���� ������ ����� �ش�.
void PrintDiag()
{
	int ii;
	SQLRETURN Ret;
	SQLINTEGER NativeError;
	SQLTCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	TCHAR str[256];

	ii = 1;
	while (Ret = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, ii, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) 
	{
		wsprintf(str, _T("SQLSTATE:%s, ��������:%s"), (LPCTSTR)SqlState, (LPCTSTR)Msg);
		::MessageBox(NULL, str, _T("���� ����"), 0);
		ii++;
	}
}
BOOL DBConnect()
{
	// ���� ������ ���� ������
	TCHAR InCon[255];
	SQLTCHAR OutCon[1024];
	SQLSMALLINT cbOutCon;
	TCHAR Dir[MAX_PATH];
	SQLRETURN Ret;

	// ȯ�� �ڵ��� �Ҵ��ϰ� ���� �Ӽ��� �����Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return FALSE;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return FALSE;

	// ���� �ڵ��� �Ҵ��ϰ� �����Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
		return FALSE;

	// MDB ���Ͽ� �����ϱ�
	GetCurrentDirectory(MAX_PATH, Dir);
	// ���� --> *.mdb,"��ĭ�پ�� �Ѵ�."*.accdb
	//<*.accdb> ����
	_stprintf(InCon,_T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\cigarette.accdb;"),Dir);
	////<*.mdb> ����
	//_stprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\cigarette.mdb;"), Dir);
	////<*.dsn> ����
	//_stprintf((TCHAR *)InCon, _T("FileDsn=%s"),_T("cigaretteaccdb.dsn"));
	//_stprintf((TCHAR *)InCon, _T("Dsn=%s"),_T("SVRKGCA"));
	//// sqlserver(client) ����
	//_stprintf(InCon, _T("%s"), _T("Driver={SQL Server};Server=shader.kr;Address=112.216.123.139,1433;Network=dbmssocn;Database=WildPlanetDB;Uid=sa;Pwd=kgca!@34;"));
	Ret = SQLDriverConnect(hDbc, NULL, (SQLTCHAR*)InCon, _countof(InCon), OutCon,
		_countof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	/*Ret = SQLConnect(hDbc,
		(SQLTCHAR*)L"ACCOUNT", SQL_NTS,
		(SQLTCHAR*)L"sa", SQL_NTS,
		(SQLTCHAR*)L"kgca!@34", SQL_NTS); */
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
	{
		SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
		SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
		SQLINTEGER sqlcode;
		SQLSMALLINT length;
		// �ܼ��� ����
		SQLError(hEnv, hDbc, hStmt, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length);
		MessageBox(NULL, (LPTSTR)buffer, (LPTSTR)sqlstate, MB_OK);

		// ��� ���� ������.
		int iErrorCnt = 1;
		while (Ret = SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, iErrorCnt++, sqlstate, &sqlcode,
			buffer, sizeof(buffer), &length) != SQL_NO_DATA)
		{
			MessageBox(NULL, (LPTSTR)buffer, (LPTSTR)sqlstate, MB_OK);
		}	
		return FALSE;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt) != SQL_SUCCESS)
		return 0;

	return TRUE;
}

void DBDisConnect()
{
	// ������
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
}

BOOL DBExecuteSQL()
{
	// ������� �����ޱ� ���� ������
	TCHAR Name[21];
	SQLLEN lName;

	// ����� �����ޱ� ���� ���ε��Ѵ�.
	SQLBindCol(hStmt,1, SQL_UNICODE,Name, sizeof(Name),&lName);

	// SQL���� �����Ѵ�.
	if (SQLExecDirect(hStmt,(SQLTCHAR *)_T("select name from tblCigar"),SQL_NTS) != SQL_SUCCESS) {
		return FALSE;
	}

	// �о�� ������ ���
	SendMessage(hList,LB_RESETCONTENT,0,0);
	while (SQLFetch(hStmt)!=SQL_NO_DATA) {
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)Name);
	}

	//setlocale(LC_ALL, "korean");
	//while (SQLFetch(hStmt) != SQL_NO_DATA)
	//{
	//	std::wcout << Name << std::endl;
	//}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

// ���� ���õ� ���ڵ带 �о� ����Ʈ�� ����Ѵ�.
BOOL ReadRecord()
{
	// ������� �����ޱ� ���� ������
	SQLTCHAR Name[21];
	int Price;
	BOOL Korean;
	SQLLEN lName,lPrice,lKorean;
	int idx;
	TCHAR Key[256], szSQL[256];

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1)
		return FALSE;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)Key);
	wsprintf(szSQL,_T("select name,price,korean from tblCigar where name='%s'"),Key);

	// ����� �����ޱ� ���� ���ε��Ѵ�.
	SQLBindCol(hStmt,1, SQL_UNICODE,Name,sizeof(Name),&lName);
	SQLBindCol(hStmt,2,SQL_C_ULONG,&Price,0,&lPrice);
	SQLBindCol(hStmt,3,SQL_C_ULONG,&Korean,0,&lKorean);

	// SQL���� �����Ѵ�.
	if (SQLExecDirect(hStmt,(SQLTCHAR *)szSQL,SQL_NTS) != SQL_SUCCESS) 
	{
		return FALSE;
	}

	// �о�� ������ ���
	if (SQLFetch(hStmt)!=SQL_NO_DATA) 
	{
		//SendMessage(hFName, WM_SETTEXT, 0, (LPCTSTR)Name);
		SetWindowText(hFName,(LPCTSTR)Name);
		wsprintf(szSQL,_T("%d"),Price);
		SetWindowText(hFPrice,szSQL);
		SendMessage(hFKorean,BM_SETCHECK,(Korean ? BST_CHECKED:BST_UNCHECKED),0);
	}
	
	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

// ���ڵ带 �߰��Ѵ�.
void AddRecord()
{
	SQLCHAR Name[21];
	int Price;
	BOOL Korean;
	TCHAR szSQL[256];

	// �� ����
	GetWindowText(hFName,(LPTSTR)Name,20);
	if (lstrlen((LPCTSTR)Name) == 0) {
		MessageBox(hDlgMain,_T("�߰��� ���ڵ��� �̸��� ���� �Է��Ͻʽÿ�"),_T("�˸�"),MB_OK);
		return;
	}
	GetWindowText(hFPrice,szSQL,256);

	Price= _ttoi(szSQL);
	Korean=(SendMessage(hFKorean,BM_GETCHECK,0,0)==BST_CHECKED);
	wsprintf(szSQL,_T("Insert into tblCigar (name,price,korean) VALUES ('%s',%d,%d)"),
		Name,Price,Korean);

	if (SQLExecDirect(hStmt,(SQLTCHAR *)szSQL,SQL_NTS) != SQL_SUCCESS) {
		MessageBox(hDlgMain,_T("���ڵ� �߰� ����. �̸��� ���� ���ڵ�� �߰��� �� �����ϴ�"),_T("����"),MB_OK);
		return;
	}

	if (hStmt) SQLCloseCursor(hStmt);
	DBExecuteSQL();
}

// ���õ� ���ڵ带 �����Ѵ�.
void DeleteRecord()
{
	TCHAR Key[256], szSQL[256];
	int idx;

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1) {
		MessageBox(hDlgMain,_T("������ ���ڵ带 ���� �����Ͻʽÿ�"),_T("�˸�"),MB_OK);
		return;
	}
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)Key);
	wsprintf(szSQL,_T("Delete from tblCigar where name='%s'"),Key);

	if (SQLExecDirect(hStmt,(SQLTCHAR *)szSQL,SQL_NTS) != SQL_SUCCESS)
		return;

	if (hStmt) SQLCloseCursor(hStmt);
	DBExecuteSQL();
}

// ������Ʈ �Ѵ�.
void UpdateRecord()
{
	SQLCHAR OldName[21], NewName[21];
	int Price;
	BOOL Korean;
	TCHAR szSQL[256];
	int idx;

	// �� ����
	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1)
		return;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)OldName);

	GetWindowText(hFName,(LPTSTR)NewName,20);
	if (lstrlen((LPCTSTR)NewName) == 0) {
		MessageBox(hDlgMain,_T("������ ���ڵ��� �̸��� ���� �Է��Ͻʽÿ�"),_T("�˸�"),MB_OK);
		return;
	}
	GetWindowText(hFPrice,szSQL,256);
	Price= _ttoi(szSQL);
	Korean=(SendMessage(hFKorean,BM_GETCHECK,0,0)==BST_CHECKED);
	wsprintf(szSQL,_T("Update tblCigar set name='%s', price=%d, korean=%d where name='%s'"),
		NewName,Price,Korean,OldName);

	if (SQLExecDirect(hStmt,(SQLTCHAR *)szSQL,SQL_NTS))
		return;

	if (hStmt) SQLCloseCursor(hStmt);
	DBExecuteSQL();
}

// hWnd �����츦 ȭ�� �߾����� ������.
void CenterWindow(HWND hWnd)
{
	RECT rcTemp1, rcTemp2 ;
	LONG iX, iY, iWidth, iHeight ;

	GetWindowRect(GetDesktopWindow(),&rcTemp1) ;
	GetWindowRect(hWnd,&rcTemp2) ;
	iWidth = rcTemp2.right - rcTemp2.left ;
	iHeight = rcTemp2.bottom - rcTemp2.top ;
	iX = LONG((rcTemp1.right - iWidth) / 2) ;
	iY = LONG((rcTemp1.bottom - iHeight) / 2) ;
	MoveWindow(hWnd,iX,iY,iWidth,iHeight,TRUE) ;
}

LRESULT CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		hList=GetDlgItem(hDlgMain,IDC_LISTCIGAR);
		hFName=GetDlgItem(hDlgMain,IDC_FNAME);
		hFPrice=GetDlgItem(hDlgMain,IDC_FPRICE);
		hFKorean=GetDlgItem(hDlgMain,IDC_FKOREAN);
		CenterWindow(hDlg);
		DBExecuteSQL();
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlgMain,IDOK);
			return TRUE;
		case IDC_BTNADD:
			AddRecord();
			return TRUE;
		case IDC_BTNDEL:
			DeleteRecord();
			return TRUE;
		case IDC_BTNUPDATE:
			UpdateRecord();
			return TRUE;
		case IDC_LISTCIGAR:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				ReadRecord();
				return TRUE;
			}
		}
		return FALSE;
	}
	return FALSE;
}

// odbc storedproc 
//1)ODBC CALL �̽������� �������� ����ϴ� SQL ���� �����մϴ�.
//�� �������� �� �� / ��� �� ��� �Ű� ������ ���ν��� ��ȯ ��(�ִ� ���)�� ���� �Ű� ���� ǥ���� ����մϴ�.
//   { ? = CALL procname(? , ? ) }
//2)ȣ�� SQLBindParameter �� �Է¿� �� �� �� / ��� �� ��� �Ű� ���� �� ���ν��� ��ȯ ��(�ִ� ���).
//3)��� �� �� ���� ������ SQLExecDirect�մϴ�.
