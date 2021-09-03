// BitmapSave.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "BitmapSave.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND	g_hWnd;
HINSTANCE g_hInstance;
COLORREF  g_maskColor;

BOOL GetFileName(HWND hWnd, TCHAR *filename)
{
	OPENFILENAME ofn;       // common dialog box structure

							// Initialize OPENFILENAME
	::ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = _MAX_PATH;;
	ofn.lpstrFilter = L"BITMAP(*.bmp)\0*.bmp\0All(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = L"BmptoMask Open File";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	return GetOpenFileName(&ofn);
}

bool SaveFile(HBITMAP hBitmap, TCHAR* strSaveFile)
{
	
	OPENFILENAME AA;
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	BITMAPINFOHEADER bitHeader;
	memset(&bitHeader, 0, sizeof(BITMAPINFOHEADER));
	bitHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitHeader.biWidth = bitmap.bmWidth;
	bitHeader.biHeight = bitmap.bmHeight;
	bitHeader.biPlanes = 1;
	bitHeader.biBitCount = bitmap.bmBitsPixel;
	bitHeader.biCompression = BI_RGB;
	//bitHeader.biSizeImage = bitmap.bmWidthBytes * bitmap.bmHeight;

	HDC hdc = GetDC(GetDesktopWindow());
	// biSizeImage ���� ���
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, NULL, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	// ��Ʈ�� ������ ���
	LPBYTE lpBits = new BYTE[bitHeader.biSizeImage];
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, lpBits, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	ReleaseDC(GetDesktopWindow(), hdc);

	// ���� ���� �� ����
	HANDLE hFile = CreateFile(strSaveFile, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		delete[] lpBits;
		return false;
	}
	// ���� ��� ����
	BITMAPFILEHEADER bmFileHeader;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bmFileHeader.bfType = ((WORD)('M' << 8) | 'B');//MAKEWORD(
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bitHeader.biSizeImage;

	DWORD dwWritten;
	//������� ����
	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//������� ����
	WriteFile(hFile, &bitHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	//������ ����
	WriteFile(hFile, lpBits, bitHeader.biSizeImage, &dwWritten, NULL);
	CloseHandle(hFile);

	delete[] lpBits;
	return true;
}
HBITMAP CreateMaskBitmap(HBITMAP initBitmap, COLORREF in_transColor)
{
	HDC  hdc = GetDC(g_hWnd);
	HDC     memDC_org, memDC_mask, memDC_ret;
	HBITMAP bitmap_mask;
	HBITMAP bitmap_maskOld, bmSaveOld, bitmap_retOld;
	BITMAP  bitmap_info;

	GetObject(initBitmap, sizeof(BITMAP), &bitmap_info);
	bitmap_mask = CreateBitmap(bitmap_info.bmWidth, bitmap_info.bmHeight, 1, 1, NULL);
	// ������ ��Ʈ�� ����
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, bitmap_info.bmWidth, bitmap_info.bmHeight);

	memDC_org = CreateCompatibleDC(NULL);
	memDC_mask = CreateCompatibleDC(NULL);
	memDC_ret = CreateCompatibleDC(NULL);
	bmSaveOld = (HBITMAP)SelectObject(memDC_org, initBitmap);
	bitmap_maskOld = (HBITMAP)SelectObject(memDC_mask, bitmap_mask);
	bitmap_retOld = (HBITMAP)SelectObject(memDC_ret, hbmResult);

	// in_transColor�� ����� �ǰ� �������� �������� �ȴ�.
	COLORREF crOldBack = SetBkColor(memDC_org, in_transColor);
	BitBlt(memDC_mask, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_org, 0, 0, SRCCOPY);
	BitBlt(memDC_ret, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_mask, 0, 0, SRCINVERT);
	SetBkColor(memDC_org, crOldBack);

	SelectObject(memDC_org, bmSaveOld);
	SelectObject(memDC_mask, bitmap_maskOld);
	SelectObject(memDC_ret, bitmap_retOld);
	DeleteDC(memDC_org);
	DeleteDC(memDC_mask);
	DeleteDC(memDC_ret);
	DeleteObject(bitmap_mask);
	ReleaseDC(g_hWnd, hdc);
	return hbmResult;
}
HBITMAP CreateMaskBitmapColor(HBITMAP initBitmap, COLORREF in_transColor)
{
	HDC  hdc = GetDC(g_hWnd);
	HDC     memDC_org, memDC_mask, memDC_ret;
	HBITMAP bitmap_mask;
	HBITMAP bitmap_maskOld, bmSaveOld, bitmap_retOld;
	BITMAP  bitmap_info;

	GetObject(initBitmap, sizeof(BITMAP), &bitmap_info);
	bitmap_mask = CreateBitmap(bitmap_info.bmWidth, bitmap_info.bmHeight, 1, 1, NULL);
	// ������ ��Ʈ�� ����
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, bitmap_info.bmWidth, bitmap_info.bmHeight);

	memDC_org = CreateCompatibleDC(NULL);
	memDC_mask = CreateCompatibleDC(NULL);
	memDC_ret = CreateCompatibleDC(NULL);

	bmSaveOld = (HBITMAP)SelectObject(memDC_org, initBitmap);
	bitmap_maskOld = (HBITMAP)SelectObject(memDC_mask, bitmap_mask);
	bitmap_retOld = (HBITMAP)SelectObject(memDC_ret, hbmResult);
	// in_transColor�� ����� �ǰ� �������� �������� �ȴ�.
	COLORREF crOldBack = SetBkColor(memDC_org, in_transColor);
	BitBlt(memDC_mask, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_org, 0, 0, SRCCOPY);
	BitBlt(memDC_ret, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_mask, 0, 0, SRCINVERT);
	SetBkColor(memDC_org, crOldBack);
	// ����ũ �ȼ�=���, �÷��� �״��
	BitBlt(memDC_ret, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_org, 0, 0, SRCPAINT);


	SelectObject(memDC_org, bmSaveOld);
	SelectObject(memDC_mask, bitmap_maskOld);
	SelectObject(memDC_ret, bitmap_retOld);

	DeleteDC(memDC_org);
	DeleteDC(memDC_mask);
	DeleteDC(memDC_ret);

	DeleteObject(bitmap_mask);
	ReleaseDC(g_hWnd, hdc);
	return hbmResult;
}
bool BitmapSave(bool bFlag)
{
	
	TCHAR inFilename[_MAX_PATH];
	inFilename[0] = '\0';
	TCHAR outFilename[_MAX_PATH];
	if (!GetFileName(NULL, inFilename))
	{
		return 0;
	}
	_tcscpy_s(outFilename, inFilename);
	TCHAR *dot = _tcsrchr(outFilename, '.');
	if (dot == NULL)
	{
		_tcscat_s(outFilename, L"mask.bmp");
	}
	else
	{
		if (bFlag == true)
		{
			TCHAR szName[] = L"mask.bmp";
			_tcscpy(dot, szName);
		}
		else
		{
			TCHAR szName[] = L"color.bmp";
			_tcscpy(dot, szName);
		}
	}
	
	HBITMAP		m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		inFilename, IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE |
		LR_LOADFROMFILE);
	if (m_hBitmap != NULL)
	{		
		if(bFlag)
			SaveFile(CreateMaskBitmap(m_hBitmap, g_maskColor), outFilename);
		else
			SaveFile(CreateMaskBitmapColor(m_hBitmap, g_maskColor), outFilename);
	}
	DeleteObject(m_hBitmap);
	return true;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	g_hInstance = hInstance;
	
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BITMAPSAVE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BITMAPSAVE));

    MSG msg;
	g_maskColor = RGB(255, 255, 255);
    // �⺻ �޽��� �����Դϴ�.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BITMAPSAVE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BITMAPSAVE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 300, 100, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   g_hWnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
			case ID_COLORREF:
			{
				//CC_ANYCOLOR: ����� �� �ִ� �⺻ ���� ������ �ִ� �� ��ȭ���ڸ� �����ش�.
				//CC_FULLOPEN : ����� ������ ������ Ȯ���� �� ��ȭ����
				//CC_PREVENTFULLOPEN : ����� ������ ����⸦ �� �� ���� �� ��ȭ���ڸ� �����ش�.
				//CC_RGBINT : rgbResult �ʵ忡 �ִ� ���� ��ȭ������ �⺻ ���� ������ ǥ���Ѵ�.
				DWORD tmp[16];
				for (int i = 0; i < 16; i++)
					tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);

				CHOOSECOLOR COLOR;
				memset(&COLOR, 0, sizeof(CHOOSECOLOR));
				COLOR.lStructSize = sizeof(CHOOSECOLOR);
				COLOR.hwndOwner = g_hWnd;
				COLOR.lpCustColors = tmp;
				COLOR.Flags = CC_FULLOPEN;
				if (ChooseColor(&COLOR) != 0)
				{
					g_maskColor = COLOR.rgbResult;					
				}
				break;
			}
			break;

			case ID_BITMAPSAVE:
				BitmapSave(true);
				break;
			case ID_BITMAPSAVECOLOR:
				BitmapSave(false);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
