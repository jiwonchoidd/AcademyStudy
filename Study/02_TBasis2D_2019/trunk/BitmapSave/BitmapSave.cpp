// BitmapSave.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "BitmapSave.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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
	// biSizeImage 정보 얻기
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, NULL, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	// 비트맵 데이터 얻기
	LPBYTE lpBits = new BYTE[bitHeader.biSizeImage];
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, lpBits, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	ReleaseDC(GetDesktopWindow(), hdc);

	// 파일 생성 및 저장
	HANDLE hFile = CreateFile(strSaveFile, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		delete[] lpBits;
		return false;
	}
	// 파일 헤더 저장
	BITMAPFILEHEADER bmFileHeader;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bmFileHeader.bfType = ((WORD)('M' << 8) | 'B');//MAKEWORD(
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bitHeader.biSizeImage;

	DWORD dwWritten;
	//파일헤더 저장
	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//정보헤더 저장
	WriteFile(hFile, &bitHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	//데이터 저장
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
	// 검정색 비트맵 생성
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, bitmap_info.bmWidth, bitmap_info.bmHeight);

	memDC_org = CreateCompatibleDC(NULL);
	memDC_mask = CreateCompatibleDC(NULL);
	memDC_ret = CreateCompatibleDC(NULL);
	bmSaveOld = (HBITMAP)SelectObject(memDC_org, initBitmap);
	bitmap_maskOld = (HBITMAP)SelectObject(memDC_mask, bitmap_mask);
	bitmap_retOld = (HBITMAP)SelectObject(memDC_ret, hbmResult);

	// in_transColor는 흰색이 되고 나머지는 검은색이 된다.
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
	// 검정색 비트맵 생성
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, bitmap_info.bmWidth, bitmap_info.bmHeight);

	memDC_org = CreateCompatibleDC(NULL);
	memDC_mask = CreateCompatibleDC(NULL);
	memDC_ret = CreateCompatibleDC(NULL);

	bmSaveOld = (HBITMAP)SelectObject(memDC_org, initBitmap);
	bitmap_maskOld = (HBITMAP)SelectObject(memDC_mask, bitmap_mask);
	bitmap_retOld = (HBITMAP)SelectObject(memDC_ret, hbmResult);
	// in_transColor는 흰색이 되고 나머지는 검은색이 된다.
	COLORREF crOldBack = SetBkColor(memDC_org, in_transColor);
	BitBlt(memDC_mask, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_org, 0, 0, SRCCOPY);
	BitBlt(memDC_ret, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_mask, 0, 0, SRCINVERT);
	SetBkColor(memDC_org, crOldBack);
	// 마스크 픽셀=흰색, 컬러는 그대로
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BITMAPSAVE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BITMAPSAVE));

    MSG msg;
	g_maskColor = RGB(255, 255, 255);
    // 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
			case ID_COLORREF:
			{
				//CC_ANYCOLOR: 사용할 수 있는 기본 생상 정보가 있는 색 대화상자를 보여준다.
				//CC_FULLOPEN : 사용자 지정색 만들기로 확장한 색 대화상자
				//CC_PREVENTFULLOPEN : 사용자 지정색 만들기를 할 수 없는 색 대화상자를 보여준다.
				//CC_RGBINT : rgbResult 필드에 있는 색을 대화상자의 기본 선택 색으로 표시한다.
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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
