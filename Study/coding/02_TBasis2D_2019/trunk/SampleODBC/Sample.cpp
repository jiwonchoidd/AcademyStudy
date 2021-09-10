#include "TOdbcQuery.h"
#include <string>
#include <time.h>
#include "resource.h"
HWND g_hDlg;
HWND g_hList;
HWND g_hIDKey;
HWND g_hName;
HWND g_hPrice;
HWND g_hKor;
TOdbcQuery  g_odbc;


SWORD sReturn = 0;
SWORD sOutReturn = 0;
BYTE* g_pFileBuffer = nullptr;

bool FileLoad(std::wstring name);
void GetListData();
void ImageLoadDirectSQL();
void ImageSaveDirectSQL();
void ReadRecord();
void ReadRecordParameter();
void ReadRecordParameterPrepare();
void AddRecord();
void AddRecordDirectSQL();
void DeleteRecord();
void UpdateRecord();

bool FileLoad(std::wstring name)
{
	DWORD dwTrans;
	LARGE_INTEGER filesize;
	HANDLE hReadFile = CreateFile(name.c_str(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hReadFile!= INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &filesize);
		g_pFileBuffer = new BYTE[filesize.LowPart];
		ReadFile(hReadFile, g_pFileBuffer,
			filesize.LowPart, &dwTrans, NULL);
	}
	else
	{
		return false;
	}
	CloseHandle(hReadFile);
	return true;
}
void GetListData()
{
	SendMessage(g_hList, LB_RESETCONTENT, 0, 0);
	TCHAR sql[MAX_PATH] = L"select USERID from USERLIST";

	g_odbc.Exec(sql);

	for (int iRow = 0; iRow < g_odbc.m_RecordResultList.size(); iRow++)
	{
		SendMessage(g_hList, LB_ADDSTRING, 0,
			(LPARAM)g_odbc.m_RecordResultList[iRow][0].c_str());
	}
}
void ImageLoadDirectSQL()
{
	SQLRETURN ret;
	CHAR smallbufer[1000] = { 0, };
	TCHAR sql[MAX_PATH] = L"{call GetImageStreaming(?,?)}";
	
	TCHAR szSelectName[32] = { 0, };
	int idx = SendMessage(g_hList, LB_GETCURSEL, 0, 0);
	SendMessage(g_hList, LB_GETTEXT, idx, (LPARAM)szSelectName);

	SQLINTEGER strLength = sizeof(szSelectName);
	SQLLEN m_cbColumn = SQL_NTS;
	ret = SQLBindParameter(	g_odbc.m_hStmt, 1, SQL_PARAM_INPUT,
		SQL_UNICODE, SQL_VARCHAR, strLength,
		0, szSelectName, strLength,&m_cbColumn);

	SQLLEN cbPutSize = 0;
	ret = SQLBindParameter(g_odbc.m_hStmt,
		2, SQL_PARAM_OUTPUT_STREAM,
		SQL_C_BINARY, SQL_VARBINARY, 0, 0,
		(SQLPOINTER)2,
		0, &cbPutSize);
	if (ret != SQL_SUCCESS && ret != SQL_NEED_DATA)
	{
		g_odbc.Check();
	}
	SQLPrepare(g_odbc.m_hStmt, sql, SQL_NTS);
	SQLExecute(g_odbc.m_hStmt);

	DWORD dwTrans;
	LARGE_INTEGER filesize;

	std::wstring fileImage = szSelectName;	
	fileImage += L".bmp";

	FILE* fp = _wfopen(fileImage.c_str(), L"w+b");

	if (fp != NULL)
	{
		SQLPOINTER token;
		SQLLEN cb;
		ret = SQLParamData(g_odbc.m_hStmt, &token);
		if (ret == SQL_PARAM_DATA_AVAILABLE)
		{
			do {
				ret = SQLGetData(g_odbc.m_hStmt, 
					(UWORD)token, SQL_C_BINARY,
					smallbufer, sizeof(smallbufer), 
					&cb);
				if (ret == SQL_NULL_DATA)
				{
					break;
				}
				if (cb > sizeof(smallbufer) )
					fwrite(smallbufer, sizeof(char), sizeof(smallbufer), fp);
				else
					fwrite(smallbufer, sizeof(char), cb, fp);
			} while (ret == SQL_SUCCESS_WITH_INFO);
		}

	}
	fflush(fp);
	fclose(fp);

	SQLCloseCursor(g_odbc.m_hStmt);
	SQLFreeStmt(g_odbc.m_hStmt, SQL_CLOSE);

	GetListData();
}
void ImageSaveDirectSQL()
{
	SQLRETURN ret;
	TCHAR saveUserName[MAX_PATH] = { 0, };
	static int iNumImage = 0;
	iNumImage += 1;
	std::wstring fileImage = L"../../data/";
	fileImage += std::to_wstring(iNumImage);
	fileImage += L".bmp";

	if (FileLoad(fileImage)==false) return;

	TCHAR NewName[MAX_PATH] = { 0, };
	TCHAR NewPass[MAX_PATH] = { 0, };
	SendMessage(g_hName, WM_GETTEXT, MAX_PATH, (LPARAM)NewName);
	GetWindowText(g_hPrice, NewPass, MAX_PATH);
	int  iNewPrice = _ttoi(NewPass);
	int kor = SendMessage(g_hKor, BM_GETCHECK, 0, 0);

	// 날짜+시간
	time_t cTime = time(NULL);
	struct tm*  tCurrentTime = localtime(&cTime);
	TCHAR addDataTime[MAX_PATH] = { 0, };
	wcsftime(addDataTime, MAX_PATH, L"%F %T", tCurrentTime);

	TCHAR sql[MAX_PATH] = { 0, };
	wsprintf(sql,L"INSERT INTO USERLIST(USERID, USERPS, LoginTime, BLOBData) VALUES('%s','%s','%s',?)", NewName, NewPass, addDataTime);

	SQLLEN lbytes = _msize(g_pFileBuffer);
	SQLLEN cbPutSize = SQL_LEN_DATA_AT_EXEC(lbytes);
	ret = SQLBindParameter(g_odbc.m_hStmt,
		1, SQL_PARAM_INPUT,
		SQL_C_BINARY, SQL_LONGVARBINARY, lbytes,0, 
		(SQLPOINTER)g_pFileBuffer, 
		lbytes, &cbPutSize);

	ret = SQLExecDirect(g_odbc.m_hStmt,	(SQLTCHAR*)sql, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_NEED_DATA)
	{
		g_odbc.Check();
	}
	
	SQLPOINTER pParamID;
	ret = SQLParamData(g_odbc.m_hStmt, &pParamID);
	SQLLEN pos = 0;
	if (ret == SQL_NEED_DATA)
	{
		SWORD cbBatch = 1000;
		
		while (lbytes > cbBatch)
		{
			SQLPutData(g_odbc.m_hStmt,
				&g_pFileBuffer[pos],
				cbBatch);
			lbytes -= cbBatch;
			pos += cbBatch;
		}
	}
	SQLPutData(g_odbc.m_hStmt,&g_pFileBuffer[pos],lbytes);
	// 최종 전송 알림
	ret = SQLParamData(g_odbc.m_hStmt, &pParamID);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		g_odbc.Check();
	}
	delete g_pFileBuffer;

	SQLCloseCursor(g_odbc.m_hStmt);
	SQLFreeStmt(g_odbc.m_hStmt, SQL_CLOSE);

	GetListData();
}
void ReadRecord()
{
	//return;
	TCHAR szSelectName[256] = { 0, };
	int idx = SendMessage(g_hList,LB_GETCURSEL, 0, 0);
	SendMessage(g_hList, LB_GETTEXT,idx, (LPARAM)szSelectName);
	
	TCHAR sql[MAX_PATH] = { 0, };
	wsprintf(sql,L"select name,price,korean from tblCigar where name='%s'",szSelectName);
		
	g_odbc.Exec(sql);

	SendMessage(g_hName, WM_SETTEXT, 0, (LPARAM)g_odbc.m_RecordResultList[0][0].c_str());
	SendMessage(g_hName, WM_SETTEXT,0, (LPARAM)g_odbc.m_RecordResultList[0][1].c_str());
	SetWindowText(g_hPrice, g_odbc.m_RecordResultList[0][2].c_str());
	int iFlag= _ttoi( g_odbc.m_RecordResultList[0][3].c_str() );
	SendMessage(g_hKor, BM_SETCHECK,(iFlag) ? BST_CHECKED: BST_UNCHECKED, 0);
}
void ReadRecordParameter()
{
	SQLRETURN ret;
	TCHAR szSelectName[64] = { 0, };
	int idx = SendMessage(g_hList, LB_GETCURSEL, 0, 0);
	SendMessage(g_hList, LB_GETTEXT, idx, (LPARAM)szSelectName);

	SQLINTEGER strLength =  sizeof(szSelectName);
	SQLLEN m_cbColumn = SQL_NTS;
	ret = SQLBindParameter(	g_odbc.m_hReadStmt, 1, SQL_PARAM_INPUT,	SQL_UNICODE, SQL_CHAR, strLength, 
		0, szSelectName, strLength,&m_cbColumn);

	if (ret != SQL_SUCCESS)
	{
		g_odbc.Check();
		return;
	}
	TCHAR sql[MAX_PATH] = { 0, };
	wsprintf(sql,L"select name,price,korean from tblCigar where name=?",SQL_NTS);

	if (g_odbc.Exec(g_odbc.m_hReadStmt))
	{
		SendMessage(g_hName, WM_SETTEXT, 0, (LPARAM)g_odbc.m_RecordResultList[0][0].c_str());
		SendMessage(g_hName, WM_SETTEXT, 0, (LPARAM)g_odbc.m_RecordResultList[0][1].c_str());
		SetWindowText(g_hPrice, g_odbc.m_RecordResultList[0][2].c_str());
		int iFlag = _ttoi(g_odbc.m_RecordResultList[0][3].c_str());
		SendMessage(g_hKor, BM_SETCHECK, (iFlag) ? BST_CHECKED : BST_UNCHECKED, 0);
	}
}
void ReadRecordParameterPrepare()
{
	SQLRETURN ret;
	TCHAR szSelectName[32] = { 0, };
	int idx = SendMessage(g_hList, LB_GETCURSEL, 0, 0);
	SendMessage(g_hList, LB_GETTEXT, idx, (LPARAM)szSelectName);

	_tcscpy(g_odbc.m_szSelectName, szSelectName);

	if (g_odbc.Exec(g_odbc.m_hReadStmt))
	{
		SendMessage(g_hIDKey, WM_SETTEXT, 0, (LPARAM)g_odbc.m_RecordResultList[0][0].c_str());
		SendMessage(g_hName, WM_SETTEXT, 0, (LPARAM)g_odbc.m_RecordResultList[0][1].c_str());
		SetWindowText(g_hPrice, g_odbc.m_RecordResultList[0][2].c_str());
		//int iFlag = _ttoi(g_odbc.m_RecordResultList[0][3].c_str());
		//SendMessage(g_hKor, BM_SETCHECK, (iFlag) ? BST_CHECKED : BST_UNCHECKED, 0);
	}		
}
void AddRecord()
{
	TCHAR NewName[MAX_PATH] = { 0, };
	TCHAR NewPass[MAX_PATH] = { 0, };	
	SendMessage(g_hName, WM_GETTEXT,MAX_PATH, (LPARAM)NewName);
	GetWindowText(g_hPrice, NewPass, MAX_PATH);
	int  iNewPrice = _ttoi(NewPass);	
	int kor = SendMessage(g_hKor, BM_GETCHECK, 0, 0);

	TCHAR sql[MAX_PATH] = 	_T("{?=CALL CreateAccountUser(?,?,?)}");

	SQLINTEGER strLength = _tcslen(NewName);
	SQLLEN m_cbColumn = SQL_NTS;
	
	SQLRETURN ret = SQLBindParameter(	g_odbc.m_hStmt, 1, 
										SQL_PARAM_OUTPUT,SQL_C_SHORT, SQL_INTEGER, 
										0,0, &sReturn, 0,&m_cbColumn);
	ret = SQLBindParameter(				g_odbc.m_hStmt, 2, 
										SQL_PARAM_INPUT,SQL_UNICODE, SQL_VARCHAR, 
										strLength,0, NewName, strLength,&m_cbColumn);
	
	strLength = _tcslen(NewPass);
	ret = SQLBindParameter(				g_odbc.m_hStmt, 3, 
										SQL_PARAM_INPUT,	SQL_UNICODE, SQL_VARCHAR, 
										strLength,0, NewPass, strLength,&m_cbColumn);
	ret = SQLBindParameter(				g_odbc.m_hStmt, 4, 
										SQL_PARAM_OUTPUT,SQL_C_SHORT, SQL_INTEGER,
										0,	0, &sOutReturn, 0,	&m_cbColumn);

	g_odbc.ExecReturn(sql);

	GetListData();
}
void AddRecordDirectSQL()
{
	TCHAR NewName[MAX_PATH] = { 0, };
	TCHAR NewPass[MAX_PATH] = { 0, };
	SendMessage(g_hName, WM_GETTEXT, MAX_PATH, (LPARAM)NewName);
	GetWindowText(g_hPrice, NewPass, MAX_PATH);

	// 날짜+시간
	time_t cTime = time(NULL);
	struct tm*  tCurrentTime = localtime(&cTime);
	//CHAR * tmp1 = ctime(&cTime);
	//CHAR * tmp2 = asctime(tCurrentTime);
	// 포맷설정 스트링화 함수
	TCHAR addDataTime[MAX_PATH] = { 0, };
	wcsftime(addDataTime, MAX_PATH,	L"%F %T", tCurrentTime);
	// L"2019-07-25 00:00:00"
	TCHAR sql[MAX_PATH] = { 0, };
	wsprintf(sql,L"Insert into USERLIST(USERID, USERPS, AccountTime) values('%s','%s','%s')",NewName, NewPass, addDataTime);

	//g_odbc.Exec(sql);	
	g_odbc.ExecReturn(sql);
	GetListData();
}
void DeleteRecord()
{
	TCHAR deleteName[MAX_PATH] = { 0, };
	int idx = SendMessage(g_hList, LB_GETCURSEL, 0, 0);
	if (idx == -1)
	{
		MessageBox(g_hDlg, _T("선택하시오!"), L"알림", MB_OK);
		return;
	}
	SendMessage(g_hList, LB_GETTEXT, idx, (LPARAM)deleteName);

	TCHAR sql[MAX_PATH] = { 0, };
	wsprintf(sql, _T("Delete from USERLIST where USERID='%s'"), deleteName);
	
	g_odbc.ExecReturn(sql);

	SQLCloseCursor(g_odbc.m_hStmt);
	SQLFreeStmt(g_odbc.m_hStmt, SQL_CLOSE);

	GetListData();
}
void UpdateRecord()
{
	TCHAR UpdateName[MAX_PATH] = { 0, };
	int idx = SendMessage(g_hList, LB_GETCURSEL, 0, 0);
	if (idx == -1)
	{
		MessageBox(g_hDlg, _T("선택하시오!"), L"알림", MB_OK);
		return;
	}
	SendMessage(g_hList, LB_GETTEXT, idx, (LPARAM)UpdateName);

	// 수정할 데이터
	TCHAR ID_key[30] = { 0, };
	TCHAR NewName[30] = { 0, };
	TCHAR NewPass[30] = { 0, };
	GetWindowText(g_hIDKey, ID_key, 30);
	GetWindowText(g_hName, NewName, 30);
	GetWindowText(g_hPrice, NewPass, 30);
	if (_tcslen(NewName) == 0 || _tcslen(NewPass) == 0)
	{
		MessageBox(g_hDlg, _T("수정하시오!"), L"알림", MB_OK);
		return;
	}
	int iPrice = _ttoi(NewPass);
	int isKor = false;
	if (SendMessage(g_hKor, BM_GETCHECK, 0, 0) 	== BST_CHECKED)
	{
		isKor = true;
	}

	/*TCHAR sql[MAX_PATH] = { 0, };
	wsprintf(sql,_T("Update USERLIST set USERID='%s', USERPS=%s where name='%s'"),	NewName, NewPass, UpdateName);*/
	TCHAR sql[MAX_PATH] = _T("{CALL UpdateSetUser(?,?,?)}");

	SQLINTEGER strLength = sizeof(NewName);
	SQLLEN m_cbColumn = SQL_NTS;
	SQLRETURN ret = SQLBindParameter(g_odbc.m_hStmt, 1, SQL_PARAM_INPUT,SQL_UNICODE, SQL_VARCHAR, strLength,0, NewName, strLength,&m_cbColumn);

	strLength = sizeof(NewPass);
	ret = SQLBindParameter(	g_odbc.m_hStmt, 2, SQL_PARAM_INPUT,	SQL_UNICODE, SQL_VARCHAR, strLength,	0, NewPass, strLength,	&m_cbColumn);

	strLength = sizeof(UpdateName);
	ret = SQLBindParameter(	g_odbc.m_hStmt, 3, SQL_PARAM_INPUT,	SQL_UNICODE, SQL_VARCHAR, strLength,	0, UpdateName, strLength,	&m_cbColumn);

	g_odbc.ExecReturn(sql);

	GetListData();
}

LRESULT CALLBACK DlgProc(HWND hDlg, 
	UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{		
		g_hDlg = hDlg;
		g_hList= GetDlgItem(hDlg, IDC_LIST1);
		g_hIDKey = GetDlgItem(hDlg, IDC_ID_KEY);
		g_hName = GetDlgItem(hDlg, IDC_EDIT1);
		g_hPrice = GetDlgItem(hDlg, IDC_EDIT2);
		g_hKor = GetDlgItem(hDlg, IDC_CHECK1);
		GetListData();
	}break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case IDOK:
			case IDCANCEL:
			{
				EndDialog(hDlg, IDOK);
				return TRUE;
			}break;
			case IDC_DATA_INSERT:
			{
				AddRecordDirectSQL();
				//AddRecord();
				return true;
			}break;
			case IDC_DATA_INSERT2:
			{
				ImageSaveDirectSQL();
			}break;
			case IDC_DATA_INSERT3:
			{
				ImageLoadDirectSQL();
			}break;
			case IDC_LIST1:
			{
				switch (HIWORD(wParam))
				{
					case LBN_SELCHANGE:
					{
						ReadRecordParameterPrepare();
						return TRUE;
					}break;
				}
			}break;
			case IDC_DELETE_RECORD:
			{
				DeleteRecord();
				return TRUE;
			}break;
			case IDC_UPDATE_RECORD:
			{
				UpdateRecord();
			}break;
		}
	}break;
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	g_odbc.Init();
	//g_odbc.Connect(1,L"cigarette.mdb");
	//g_odbc.Connect(2,L"cigarette.mdb.dsn");
	g_odbc.Connect(3,L"USERLIST", L"sa", L"kgca!@34");
	
	if (g_odbc.Prepare(L"select * from USERLIST where USERID=?") == false) return 1;
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),HWND_DESKTOP, DlgProc);
	return 0;
}