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
	// ������ �޸𸮿� ��Ʈ�� ����
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
bool xBitmap::Load(DWORD bitmap)
{
	m_hBitmap = LoadBitmap(g_hInstance,
		MAKEINTRESOURCE(bitmap));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// ������ �޸𸮿� ��Ʈ�� ����
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
		// �߿� : ������ ���� �������κ��� ���� �� ���� ũ��
		//        ��ŭ�� Ȯ�� ����Ѵ�. ���� ���� �� ���� ũ�Ⱑ 
		//        ���� ��Ʈ�� ũ�⺸�� ũ�� �ִ�ũ�� ��ŭ�� �׷�����.
		//        �׷��� ������ ���� �� ���� ũ��� ��Ʈ�� ũ�� �ѵ� ������ �����Ǿ�� �Ѵ�.
		StretchBlt(
			// ��ǥ ��� ����
			hDC, pos.x+rt.right, pos.y,
			-(rt.right), rt.bottom,								  
			m_hMemDC,
			rt.left, rt.top, // ���� ����
			rt.right, rt.bottom, // ���� �������κ��� ���� �� ���� ũ��
			                      
			SRCCOPY);
	}break;
	case 2:
	{
		StretchBlt(
			// ��ǥ ��� ����
			hDC, pos.x, pos.y+rt.bottom,
			rt.right, -(rt.bottom),//+pos.y*1.33f,
			// ���� ����
			m_hMemDC,
			rt.left, rt.top, 
			rt.right, rt.bottom,
			SRCCOPY);
	}break;
	case 3:
	{
		StretchBlt(
			// ��ǥ ��� ����			
			hDC, pos.x + rt.right, 
				 pos.y + rt.bottom,
			-(rt.right), -(rt.bottom),			
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			SRCCOPY);

		//StretchBlt(
		//	// ��ǥ ��� ����
		//	hDC, rt.right - rt.left,
		//	rt.bottom - rt.top,
		//	-(rt.right) + rt.left * 2,
		//	-(rt.bottom) + rt.top * 2,
		//	// ���� ����
		//	m_hMemDC, 0, 0, m_BmpInfo.bmWidth,
		//	m_BmpInfo.bmHeight,
		//	SRCCOPY);
	}break;
	case 4:
	{
		// �÷�Ű�� �����Ͽ� �����ϰ� �Ѵ�. msimg32.lib �ʿ�
		TransparentBlt(
			hDC, pos.x, pos.y,rt.right,	rt.bottom,
			m_hMemDC,rt.left, rt.top,	rt.right, rt.bottom,
			RGB(255,255,255));
	}break;
	default:
	{
		BitBlt(
			// ��ǥ ��� ����
			hDC, pos.x, pos.y,
			rt.right,
			rt.bottom,
			// ���� ����
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

//BLACKNESS : �̹����� ������ ���� �ƴ� ���������� ä��ϴ�.
//DSTINVERT : ������ �̹����ʹ� ������� ȭ���� ������ ������ŵ�ϴ�.
//MERGECOPY : �̹����� ����� ���� ���õ� �귯�ø� AND �����ڸ� ����Ͽ� �����մϴ�.
//MERGEPAINT : ������ �̹����� ȭ���� ���� OR �����ڸ� ����Ͽ� �����մϴ�.
//NOMIRRORBITMAP : Prevents the bitmap from being mirrored.
//NOTSRCCOPY : �̹����� ������ ������ŵ�ϴ�.
//NOTSRCERASE : ȭ��� �̹����� OR �����ڸ� ����Ͽ� ��ģ ���� ������ ������ŵ�ϴ�.
//PATCOPY : ���� ���õ� �귯�ø� ����մϴ�.
//PATINVERT : ���� ���õ� �귯�� ���� ȭ���� XOR �����ڸ� ����Ͽ� �����մϴ�.
//PATPAINT : ���� ���õ� �귯�� ���� ������ ���� �̹����� OR �����ڸ� ����Ͽ� �����մϴ�.
             //�״��� OR �����ڸ� ����Ͽ� ȭ��� �����մϴ�.
//SRCAND : ȭ��� �̹����� AND �����ڸ� ����Ͽ� �����մϴ�.
//SRCCOPY : ���� �̹����� ����մϴ�.
//SRCERASE : ������ ȭ��� �̹����� AND �����ڸ� ����Ͽ� �����մϴ�.
//SRCINVERT : ȭ��� �̹����� XOR �����ڸ� ����Ͽ� �����մϴ�.
//SRCPAINT : ȭ��� �̹����� OR �����ڸ� ����Ͽ� �����մϴ�.
//WHITENESS : �̹����� ������ ���� �ƴ� �Ͼ������ ä��ϴ�.



