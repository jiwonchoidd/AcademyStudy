#include "Sample.h"
#include <commctrl.h>
INT_PTR CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	TCHAR m_ipaddress[128]=L"127.0.0.1";
	switch( msg )
	{
	case WM_INITDIALOG:
		{
			//HWND Handle = GetDlgItem( hDlg, IDC_IPADDRESS1);
			//SendMessage( Handle, IPM_SETADDRESS, 0, 0x11223344 );
			//SendDlgItemMessage(hDlg,IDC_IPADDRESS1, IPM_SETADDRESS, 0, 0x11223344 );
			SetDlgItemText( hDlg, IDC_IPADDRESS1, m_ipaddress);
		}return TRUE;
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ))
			{			
			case IDOK:
				{
					/*DWORD address;
					SendDlgItemMessage(hDlg,IDC_IPADDRESS1, IPM_GETADDRESS, 0, (LPARAM)&address );
					TCHAR buffer[256] = {0,};
					wsprintf( buffer, L"%d.%d.%d.%d",
						FIRST_IPADDRESS(address),
						SECOND_IPADDRESS(address),
						THIRD_IPADDRESS(address),
						FOURTH_IPADDRESS(address)
						);
					MessageBox( hDlg, buffer, L"ip", MB_OK );*/			
					GetDlgItemText( hDlg, IDC_IPADDRESS1, m_ipaddress, 128 );
					EndDialog( hDlg, IDOK );
				}
				return TRUE;
			case IDCANCEL:
				{
					EndDialog( hDlg, IDCANCEL );
				}return TRUE;
			}
		}break;
	}
	return FALSE;
}
INT_PTR CALLBACK ModelessDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		{
			HMENU hMenu, hPopup;
			hMenu = CreateMenu();
			hPopup = CreatePopupMenu();
			AppendMenu( hMenu, MF_STRING  | MF_POPUP, (UINT)hPopup, L"AA" );
			AppendMenu( hPopup, MF_STRING, 10000, L"aa" );
			AppendMenu( hPopup, MF_STRING, 10001, L"bb" );
			SetMenu(hDlg, hMenu);
		}return TRUE;
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ))
			{
			case 10000:
				{
					MessageBox( hDlg, L"1번 선택", L"메뉴", MB_OK );
				}break;
			case 10001:
				{
					MessageBox( hDlg, L"2번 선택", L"메뉴", MB_OK );
				}break;
			case IDOK:
				{
					DestroyWindow( hDlg );
				}
				return TRUE;
			case IDCANCEL:
				{
					DestroyWindow( hDlg);
				}return TRUE;		
			}
		}break;
	}
	return FALSE;
}
LRESULT Sample::MsgProc( HWND hWnd, 
						UINT msg,
						WPARAM wParam,
						LPARAM lParam )
{
	switch( msg )
	{
	case WM_CONTEXTMENU ://WM_LBUTTONDOWN :
	{
		HMENU hMenu = LoadMenu( g_hInstance, MAKEINTRESOURCE(IDR_MENU1) );
		HMENU hPopup = GetSubMenu( hMenu, 0 );
		TrackPopupMenu( hPopup, TPM_LEFTALIGN, (short)LOWORD( lParam ), 
			(short)HIWORD(lParam), 0, hWnd, NULL );
		DestroyMenu(hMenu);
	}break;
	case WM_COMMAND :
		{
			switch( LOWORD( wParam ))
			{
			case ID_Modal:
				{
					DialogBox( g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 
						hWnd, DlgProc );			
				}break;
			case ID_Modaless:
				{
					if( !IsWindow( m_hModelessDlg ) )
					{
						m_hModelessDlg= CreateDialog( g_hInstance, MAKEINTRESOURCE(IDD_DIALOG2), 
									hWnd, ModelessDlgProc );			
						ShowWindow( m_hModelessDlg, SW_SHOW );		
					}
				}break;
			}
		}return 1;	
	}
	return -1;
}
Sample::Sample(void)
{
}

Sample::~Sample(void)
{
}

TCORE_START;
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance,
	L"SampleDlg", 800, 600, WS_POPUPWINDOW))
{
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;