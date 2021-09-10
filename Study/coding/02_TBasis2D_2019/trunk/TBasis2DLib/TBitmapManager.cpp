#include "TBitmapManager.h"
bool TBitmapManager::SaveFile(HBITMAP hBitmap, const TCHAR* strSaveFile)
{
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
HBITMAP TBitmapManager::CreateDIB(
	int iWidth, int iHeight,
	LPVOID* pPixelData)
{
	BITMAPINFO bmi;
	LPVOID pBits;

	HBITMAP hBitmap;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi,
		DIB_RGB_COLORS, &pBits, NULL, 0);


	for (int j = 0; j < iHeight; ++j)
	{
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pBits)[j * iWidth];
		for (int i = 0; i < iWidth; ++i)
		{			
			pbSrcRGB[0] = 0;
			pbSrcRGB[1] = 0;
			pbSrcRGB[2] = 0;
			pbSrcRGB[3] = 255;
			pbSrcRGB += 4;
		}
	}

	if (pPixelData != nullptr)
	{
		*pPixelData = pBits;
	}
	return hBitmap;

}
TBitmap*    TBitmapManager::Load(const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	TBitmap* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}


	pData = new TBitmap;
	pData->Init();
	if (pData->Load(loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}
TBitmap*    TBitmapManager::CreateLoad(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	TBitmap* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}


	pData = new TBitmap;
	pData->Init();	
	if (pData->CreateDIB(iWidth, iHeight, iBitCount))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}
TBitmap*    TBitmapManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool		TBitmapManager::Init()
{
	return true;
}
bool		TBitmapManager::Frame()
{
	return true;
}
bool		TBitmapManager::Render()
{
	return true;
}
bool		TBitmapManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

TBitmapManager::TBitmapManager()
{
	m_szDefaultPath = L"../../data/bitmap/";
}

TBitmapManager::~TBitmapManager()
{
	Release();
}