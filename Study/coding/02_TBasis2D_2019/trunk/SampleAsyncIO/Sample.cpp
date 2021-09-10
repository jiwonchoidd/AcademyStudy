#include "Sample.h"
#include "TBasisSys.h"
#include <time.h>
// �񵿱� ����� ��� ����
// �������� ������ ���̱� ���ؼ�
// 2~3�� ������ ���
#define MEGA	1048567
void Sample::T_ERROR()
{
	if( WSAGetLastError() != WSA_IO_PENDING )
	{
		setlocale(LC_ALL,"KOREAN");
		char* lpMsgBuf;
		FormatMessageA( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER|
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		OutputDebugStringA((char*)lpMsgBuf) ;
		LocalFree(lpMsgBuf);		
	}
}
bool Sample::Init()
{
	DWORD   Count=1;
	char	sTmp[256] = {0,};
	char*	p;
	
	m_buf = (char*)calloc( 50, MEGA );
	if( m_buf == NULL ) return false;

	int iTotal = 50*MEGA;
	p = m_buf;

	srand((unsigned)time(NULL));
	char chWorld = 65 + rand() % 26;
	sprintf( sTmp, "%c", chWorld);
	for( int i=1;;i++ )
	{		
		int tlen = strlen( sTmp );
		int iSize =  p-m_buf+tlen;
		if( iSize >= iTotal ) 
		{
			break;
		}		
		strcpy( p, sTmp );
		p += tlen;
	}
	m_dwLength = p-m_buf;

	DeleteFile( L"BigFile.txt" );
	m_hFile = CreateFile(L"BigFile.txt",
						GENERIC_READ | GENERIC_WRITE, 
						0, 
						NULL,
						CREATE_ALWAYS ,// ������ �����ϸ� ���� �� ������. 
						FILE_ATTRIBUTE_NORMAL |
						FILE_FLAG_OVERLAPPED, 
						NULL );
	// ������ �����ϰ� ���� ������ ��쿡 �Լ��� ���������� �����ϸ� ��ȯ��.
	//if( ERROR_ALREADY_EXISTS == GetLastError() )
	//{
	//	//MessageBox( g_hWnd, L"�����ϴ� ������ ���������� ���� ���.", L"���� ���� ����", 0);
	//	T_ERROR();
	//}
	if( m_hFile == INVALID_HANDLE_VALUE )
	{
		T_ERROR();
		MessageBox( g_hWnd, L"���� ���� ����", L"����", 0);
		return 0;
	}
	
	m_ov.Offset		= 0;
	m_ov.OffsetHigh = 0;
	m_ov.hEvent		= NULL;

	DWORD dwWritten;
	BOOL Result = WriteFile(m_hFile, m_buf, m_dwLength, &dwWritten, &m_ov );
	
	DWORD dwError = GetLastError();
	if( Result == FALSE && dwError== ERROR_IO_PENDING )
	{
		T_ERROR();
		MessageBeep(0);
	}	
	return true;
}
bool Sample::Frame()
{
	if( m_hFile == NULL || m_hFile == INVALID_HANDLE_VALUE ) 
	{
		return true;
	}
	m_iCount++;	
	DWORD dwError;
	if( (dwError = WaitForSingleObject( m_hFile, 0) )== WAIT_OBJECT_0 )
	{
		DWORD Tran = 0;
		BOOL Result = GetOverlappedResult( m_hFile, &m_ov, &Tran, FALSE );
		if( Tran >= m_dwLength )
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
			_stprintf( m_strBuffer,L"%d ������ �� %d����Ʈ ����� �Ϸ�����.",m_iCount,	Tran );
		}
	}
	else if(dwError == WAIT_TIMEOUT )
	{		
		_stprintf( m_strBuffer,L"ī��Ʈ=%d �����",m_iCount );		
	}
	else
	{
		T_ERROR();
	}

	HDC hdc = GetDC( g_hWnd );
	TextOut( hdc, 10, 10, m_strBuffer, lstrlen( m_strBuffer ));
	ReleaseDC( g_hWnd, hdc );
	return true;
}

bool Sample::Release()
{
	if( m_buf ) 
	{
		free(m_buf);
		m_buf = 0;
	}
	if( m_hFile )
	{
		CloseHandle(m_hFile);
	}

	return true;
}
Sample::Sample(void)
{
	m_iCount	= 0;
	m_dwLength	= 0;
}

Sample::~Sample(void)
{
}
INT WINAPI wWinMain(   HINSTANCE hInst, 
								HINSTANCE hPreInst, 
								LPWSTR strCmdLine, 
								INT nShow )
{
	Sample sWin;
	sWin.SetWindow(hInst, 800, 600, L"SampleAsyncIO");
	return 0;
}