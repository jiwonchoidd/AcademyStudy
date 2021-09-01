#include "xBitmap.h"
#pragma comment(lib, "msimg32.lib")

bool xBitmap::Load(TCHAR* pszLoadFile)
{
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		pszLoadFile, IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE |
		LR_LOADFROMFILE);
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// 생성된 메모리에 비트맵 적용
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
bool xBitmap::Load(DWORD bitmap)
{
	m_hBitmap = LoadBitmap(g_hInstance,
		MAKEINTRESOURCE(bitmap));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// 생성된 메모리에 비트맵 적용
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
bool		xBitmap::Set(HDC hdc )
{
	m_hScreenDC = hdc;
	return true;
};
bool		xBitmap::Draw(HDC hDC, tPOINT pos, RECT rt,
	DWORD dwScale )
{
	switch (dwScale)
	{
	case 1:
	{
		// 중요 : 원본의 시작 지점으로부터 가로 및 세로 크기
		//        만큼을 확대 축소한다. 만약 가로 및 세로 크기가 
		//        원본 비트맵 크기보다 크면 최대크기 만큼만 그려진다.
		//        그렇기 때문에 가로 및 세로 크기는 비트맵 크기 한도 내에서 지정되어야 한다.
		StretchBlt(
			// 목표 대상 영역
			hDC, pos.x+rt.right, pos.y,
			-(rt.right), rt.bottom,								  
			m_hMemDC,
			rt.left, rt.top, // 시작 지점
			rt.right, rt.bottom, // 시작 지점으로부터 가로 및 세로 크기
			                      
			SRCCOPY);
	}break;
	case 2:
	{
		StretchBlt(
			// 목표 대상 영역
			hDC, pos.x, pos.y+rt.bottom,
			rt.right, -(rt.bottom),//+pos.y*1.33f,
			// 원본 영역
			m_hMemDC,
			rt.left, rt.top, 
			rt.right, rt.bottom,
			SRCCOPY);
	}break;
	case 3:
	{
		StretchBlt(
			// 목표 대상 영역			
			hDC, pos.x + rt.right, 
				 pos.y + rt.bottom,
			-(rt.right), -(rt.bottom),			
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			SRCCOPY);

		//StretchBlt(
		//	// 목표 대상 영역
		//	hDC, rt.right - rt.left,
		//	rt.bottom - rt.top,
		//	-(rt.right) + rt.left * 2,
		//	-(rt.bottom) + rt.top * 2,
		//	// 원본 영역
		//	m_hMemDC, 0, 0, m_BmpInfo.bmWidth,
		//	m_BmpInfo.bmHeight,
		//	SRCCOPY);
	}break;
	case 4:
	{
		// 컬러키를 지정하여 투명하게 한다. msimg32.lib 필요
		TransparentBlt(
			hDC, pos.x, pos.y,rt.right,	rt.bottom,
			m_hMemDC,rt.left, rt.top,	rt.right, rt.bottom,
			RGB(255,255,255));
	}break;
	default:
	{
		BitBlt(
			// 목표 대상 영역
			hDC, pos.x, pos.y,
			rt.right,
			rt.bottom,
			// 원본 영역
			m_hMemDC, rt.left, rt.top, SRCCOPY);
	}break;
	}
	
	return true;
};
bool		xBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
};

xBitmap::xBitmap()
{
}


xBitmap::~xBitmap()
{
}

//BLACKNESS : 이미지를 원본의 색이 아닌 검은색으로 채웁니다.
//DSTINVERT : 지정된 이미지와는 상관없이 화면의 색상을 반전시킵니다.
//MERGECOPY : 이미지의 색상과 현재 선택된 브러시를 AND 연산자를 사용하여 병합합니다.
//MERGEPAINT : 반전된 이미지와 화면의 색을 OR 연산자를 사용하여 병합합니다.
//NOMIRRORBITMAP : Prevents the bitmap from being mirrored.
//NOTSRCCOPY : 이미지의 색상을 반전시킵니다.
//NOTSRCERASE : 화면과 이미지를 OR 연산자를 사용하여 합친 다음 색상을 반전시킵니다.
//PATCOPY : 현재 선택된 브러시를 출력합니다.
//PATINVERT : 현재 선택된 브러시 색과 화면을 XOR 연산자를 사용하여 결합합니다.
//PATPAINT : 현재 선택된 브러시 색과 반전된 원본 이미지를 OR 연산자를 사용하여 결합합니다.
             //그다음 OR 연산자를 사용하여 화면과 결합합니다.
//SRCAND : 화면과 이미지를 AND 연산자를 사용하여 결합합니다.
//SRCCOPY : 원본 이미지를 출력합니다.
//SRCERASE : 반전된 화면과 이미지를 AND 연산자를 사용하여 결합합니다.
//SRCINVERT : 화면과 이미지를 XOR 연산자를 사용하여 결합합니다.
//SRCPAINT : 화면과 이미지를 OR 연산자를 사용하여 결합합니다.
//WHITENESS : 이미지를 원본의 색이 아닌 하얀색으로 채웁니다.



