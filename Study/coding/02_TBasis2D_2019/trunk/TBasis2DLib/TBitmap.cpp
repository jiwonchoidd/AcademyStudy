#include "TBitmap.h"
#include "TGraphicAPI.h"
static void Error(const CHAR* msg = 0, const char* lpData = 0)
{
	LPVOID* lpMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(CHAR*)&lpMsg, 0, NULL);

	std::string szBuffer = (lpData != nullptr) ? lpData : "";
	szBuffer += "\n";
	szBuffer += (CHAR*)lpMsg;

	MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
	LocalFree(lpMsg);
}
bool  TBitmap::Init()
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	
	return true;
}
HBITMAP  TBitmap::LoadDIB(wstring filename)
{
	// api file I/O
	HANDLE hFile = CreateFile(filename.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	BITMAPFILEHEADER header;
	DWORD dwRead;
	BOOL bRet = ReadFile(hFile, &header, sizeof(BITMAPFILEHEADER),
		&dwRead, NULL);

	DWORD dwDataSize = header.bfOffBits -
		sizeof(BITMAPFILEHEADER);

	BITMAPINFO* pBitInfo =
		(BITMAPINFO*)malloc(dwDataSize);

	bRet = ReadFile(hFile, pBitInfo, dwDataSize, &dwRead, NULL);
	// 24bit는 BI_RGB( 비압축RGB) 이외의 것은 지원하지 않는다.
	// 32bit는 이외의 속성도 지원한다.
	if (pBitInfo->bmiHeader.biBitCount == 24 &&
		pBitInfo->bmiHeader.biCompression != BI_RGB)
	{
		pBitInfo->bmiHeader.biCompression = BI_RGB;
	}
	PVOID pRaster = 0;
	m_hBitmap = CreateDIBSection(g_hScreenDC,
		pBitInfo, DIB_RGB_COLORS,
		(LPVOID*)(&m_pRaster), NULL, 0);
	if (m_hBitmap != nullptr)
	{
		ReadFile(hFile, m_pRaster,header.bfSize - header.bfOffBits,	&dwRead, NULL);
	}

	//// TODO : AC_SRC_ALPHA 처리시에는 미리곱해진알파(premultiplied alpha) 처리를 반드시 해야 한다.
	//// 그렇지 않으면 투명과 불투명처리는 되지만 반투명처리가 되지 않는다.
	if (pBitInfo->bmiHeader.biBitCount == 32)
	{
		for (int j = 0; j < pBitInfo->bmiHeader.biHeight; ++j)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[j * pBitInfo->bmiHeader.biWidth];
			for (int i = 0; i < pBitInfo->bmiHeader.biWidth; ++i)
			{
				pbSrcRGB[0] = (pbSrcRGB[0] * pbSrcRGB[3]) / 255;
				pbSrcRGB[1] = (pbSrcRGB[1] * pbSrcRGB[3]) / 255;
				pbSrcRGB[2] = (pbSrcRGB[2] * pbSrcRGB[3]) / 255;
				pbSrcRGB += 4;
			}
		}
	}

	free(pBitInfo);
	CloseHandle(hFile);	
	return m_hBitmap;
}
bool  TBitmap::Load(wstring filename)
{
	//m_hBitmap = LoadDIB(filename.c_str());
	CreateDIBFromFile(filename);

	if (m_hBitmap == NULL)
	{
		m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
			filename.c_str(),
			IMAGE_BITMAP,
			0,
			0,
			LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (m_hBitmap == nullptr)
		{
			//Error();
			return false;
		}
	}
	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;
}
bool TBitmap::CreateDIB(
	LONG iWidth, LONG iHeight,int iBitCount)
{
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = iBitCount;
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	m_hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi,DIB_RGB_COLORS, (LPVOID*)&m_pRaster, NULL, 0);

	/* 비트맵 픽셀에 원하는 값을 체운다.
	for (int j = 0; j < iHeight; ++j)
	{
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[j * iWidth];
		for (int i = 0; i < iWidth; ++i)
		{
			pbSrcRGB[0] = 255;
			pbSrcRGB[1] = 0;
			pbSrcRGB[2] = 0;
			pbSrcRGB[3] = 128;
			pbSrcRGB += 4;
		}
	}*/

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 255 );
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;

}
bool  TBitmap::Frame()
{
	return true;
}
bool  TBitmap::Release()
{
	SafeRelease(m_pOriginalBitmapSource);
	DeleteObject(m_hBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);	
	return true;
}
bool  TBitmap::Render()
{
	// 대상DC, 원본DC
	BitBlt(g_hOffScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom,
		m_hMemDC, 0, 0, SRCCOPY);
	return true;
}
bool  TBitmap::Draw( RECT desk, RECT src, DWORD op, int iType)
{
	// 대상DC, 원본DC
	//BitBlt( g_hOffScreenDC, desk.left, desk.top,
	//						desk.right, desk.bottom,
	//		m_hMemDC, src.left, src.top, op);
	switch (iType)
	{
		// 좌우
	case 1 :{
		StretchBlt(g_hOffScreenDC,
			desk.left+ desk.right, desk.top,
			-desk.right, desk.bottom,
			m_hMemDC, src.left, src.top,
			src.right, src.bottom, op);
	}break;
		// 상하
	case 2: {
		StretchBlt(g_hOffScreenDC,
			desk.left, desk.top+ desk.bottom,
			desk.right, -desk.bottom,
			m_hMemDC, src.left, src.top,
			src.right, src.bottom, op);
	}break;
		//좌우상하
	case 3: {
		StretchBlt(g_hOffScreenDC,
			desk.left+ desk.right, desk.top + desk.bottom,
			-desk.right, -desk.bottom,
			m_hMemDC, src.left, src.top,
			src.right, src.bottom, op);
	}break;
	default: {
		StretchBlt(g_hOffScreenDC,
			desk.left, desk.top,
			desk.right, desk.bottom,
			m_hMemDC, src.left, src.top,
			src.right, src.bottom, op);
	}break;
	}	
	return true;
}
bool  TBitmap::DrawColorKey(RECT desk, 
							RECT src, DWORD color)
{
	BOOL bRet = TransparentBlt(g_hOffScreenDC,
					desk.left, desk.top,
					desk.right, desk.bottom,
					m_hMemDC, src.left, src.top,
					src.right, src.bottom,
					color);

	return bRet;
}
bool  TBitmap::DrawColorKey(LONG x, LONG y, DWORD color)
{
	RECT desk = { x,y, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	RECT src = { 0,0,m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };

	BOOL bRet = TransparentBlt(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC, src.left, src.top,
		src.right, src.bottom,
		color);

	return bRet;
}
bool  TBitmap::DrawAlphaBlend(LONG x, LONG y, BYTE alpha)
{
	RECT desk = {x,y, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight};
	RECT src = { 0,0,m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	//(cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, m_BlendFunction);
	return true;
}
bool  TBitmap::DrawAlphaBlend(RECT desk, 
								RECT src,
								BYTE alpha)
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC, 
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC, 
		src.left, src.top,
		src.right, src.bottom, m_BlendFunction);
	return true;
}

bool  TBitmap::DrawAlphaBlend(RECT desk,
	RECT src,
	BLENDFUNCTION bf)
{
	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, bf);
	return true;
}
bool  TBitmap::DrawAlphaBlend(LONG x, LONG y,
	BYTE alpha,
	BLENDFUNCTION bf)
{
	RECT desk = { x,y, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	RECT src = { 0,0,m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	//(cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, bf);
	return true;
}

/******************************************************************
*  Load an image file and create an DIB Section                   *
******************************************************************/

HRESULT TBitmap::CreateDIBFromFile(T_STR szFileName)
{
	assert(TGraphicAPI::m_pIWICFactory);
	HRESULT hr = S_OK;
	IWICBitmapDecoder *pDecoder = NULL;
	// Step 2: Decode the source image to IWICBitmapSource
	// Create a decoder
	hr = TGraphicAPI::m_pIWICFactory->CreateDecoderFromFilename(
		szFileName.c_str(),                      // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&pDecoder                        // Pointer to the decoder
	);

	IWICBitmapFrameDecode *pFrame = NULL;

	// Retrieve the first frame of the image from the decoder
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pFrame);
	}

	// Retrieve IWICBitmapSource from the frame
	if (SUCCEEDED(hr))
	{
		SafeRelease(m_pOriginalBitmapSource);
		hr = pFrame->QueryInterface(
			IID_IWICBitmapSource,
			reinterpret_cast<void **>(&m_pOriginalBitmapSource));
	}

	IWICBitmapSource *pToRenderBitmapSource = NULL;

	// Step 3: Scale the original IWICBitmapSource to the client rect size
	// and convert the pixel format
	if (SUCCEEDED(hr))
	{
		hr = ConvertBitmapSource(g_hWnd, &pToRenderBitmapSource);
	}

	// Step 4: Create a DIB from the converted IWICBitmapSource
	if (SUCCEEDED(hr))
	{
		hr = CreateDIBSectionFromBitmapSource(pToRenderBitmapSource);
	}

	SafeRelease(pToRenderBitmapSource);
	SafeRelease(pDecoder);
	SafeRelease(pFrame);
	
	return hr;
}

/******************************************************************
* Creates an open file dialog box and locate the image to decode. *
******************************************************************/

BOOL TBitmap::LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName)
{
	pszFileName[0] = L'\0';

	OPENFILENAME ofn;
	RtlZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"All Image Files\0"              L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico\0"
		L"Windows Bitmap\0"               L"*.bmp;*.dib\0"
		L"High Definition Photo\0"        L"*.wdp;*.mdp;*.hdp\0"
		L"Graphics Interchange Format\0"  L"*.gif\0"
		L"Portable Network Graphics\0"    L"*.png\0"
		L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0"
		L"Tiff File\0"                    L"*.tif\0"
		L"Icon\0"                         L"*.ico\0"
		L"All Files\0"                    L"*.*\0"
		L"\0";
	ofn.lpstrFile = pszFileName;
	ofn.nMaxFile = cchFileName;
	ofn.lpstrTitle = L"Open Image";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	// Display the Open dialog box. 
	return GetOpenFileName(&ofn);
}


/*******************************************************************************
*  Scales original IWICBitmapSource to the client rect size  and convert       *
*  the pixel format. Store the converted bitmap as *ppToRenderBitmapSource     *
********************************************************************************/

HRESULT TBitmap::ConvertBitmapSource(HWND hWnd, IWICBitmapSource **ppToRenderBitmapSource)
{
	*ppToRenderBitmapSource = NULL;

	HRESULT hr = S_OK;

	// Get the client Rect
	RECT rcClient;
	/// TODO : TGraphicAPI::m_pIWICFactory
	hr = GetClientRect(hWnd, &rcClient) ? S_OK : E_FAIL;
	UINT w;
	UINT h;
	m_pOriginalBitmapSource->GetSize(&w, &h);
	if (SUCCEEDED(hr))
	{
		// Create a BitmapScaler
		IWICBitmapScaler *pScaler = NULL;

		hr = TGraphicAPI::m_pIWICFactory->CreateBitmapScaler(&pScaler);

		// Initialize the bitmap scaler from the original bitmap map bits
		if (SUCCEEDED(hr))
		{
			hr = pScaler->Initialize(
				m_pOriginalBitmapSource,
				w,//rcClient.right - rcClient.left,
				h,//rcClient.bottom - rcClient.top,
				WICBitmapInterpolationModeFant);
		}

		// Format convert the bitmap into 32bppBGR, a convenient 
		// pixel format for GDI rendering 
		if (SUCCEEDED(hr))
		{
			IWICFormatConverter *pConverter = NULL;

			hr = TGraphicAPI::m_pIWICFactory->CreateFormatConverter(&pConverter);

			// Format convert to 32bppBGR
			if (SUCCEEDED(hr))
			{
				hr = pConverter->Initialize(
					pScaler,                         // Input bitmap to convert
					GUID_WICPixelFormat32bppBGR,     // Destination pixel format
					WICBitmapDitherTypeNone,         // Specified dither patterm
					NULL,                            // Specify a particular palette 
					0.f,                             // Alpha threshold
					WICBitmapPaletteTypeCustom       // Palette translation type
				);

				// Store the converted bitmap as ppToRenderBitmapSource 
				if (SUCCEEDED(hr))
				{
					hr = pConverter->QueryInterface(IID_PPV_ARGS(ppToRenderBitmapSource));
				}
			}

			SafeRelease(pConverter);
		}

		SafeRelease(pScaler);
	}

	return hr;
}


/******************************************************************
*  Creates a DIB Section from the converted IWICBitmapSource      *
******************************************************************/

HRESULT TBitmap::CreateDIBSectionFromBitmapSource(IWICBitmapSource *pToRenderBitmapSource)
{
	HRESULT hr = S_OK;

	// Get image attributes and check for valid image
	UINT width = 0;
	UINT height = 0;

	//void *pvImageBits = NULL;

	// Check BitmapSource format
	WICPixelFormatGUID pixelFormat;
	hr = pToRenderBitmapSource->GetPixelFormat(&pixelFormat);

	if (SUCCEEDED(hr))
	{
		hr = (pixelFormat == GUID_WICPixelFormat32bppBGR) ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		hr = pToRenderBitmapSource->GetSize(&width, &height);
	}

	// Create a DIB section based on Bitmap Info
	// BITMAPINFO Struct must first be setup before a DIB can be created.
	// Note that the height is negative for top-down bitmaps
	if (SUCCEEDED(hr))
	{
		BITMAPINFO bminfo;
		ZeroMemory(&bminfo, sizeof(bminfo));
		bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bminfo.bmiHeader.biWidth = width;
		bminfo.bmiHeader.biHeight = -(LONG)height;
		bminfo.bmiHeader.biPlanes = 1;
		bminfo.bmiHeader.biBitCount = 32;
		bminfo.bmiHeader.biCompression = BI_RGB;

		// Get a DC for the full screen
		HDC hdcScreen = GetDC(NULL);

		hr = hdcScreen ? S_OK : E_FAIL;

		// Release the previously allocated bitmap 
		if (SUCCEEDED(hr))
		{
			if (m_hBitmap)
			{
				DeleteObject(m_hBitmap);
			}

			m_hBitmap = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS,
				(LPVOID*)(&m_pRaster), NULL, 0);

			ReleaseDC(NULL, hdcScreen);

			hr = m_hBitmap ? S_OK : E_FAIL;
		}
	}

	UINT cbStride = 0;
	if (SUCCEEDED(hr))
	{
		// Size of a scan line represented in bytes: 4 bytes each pixel
		hr = UIntMult(width, sizeof(DWORD), &cbStride);
	}

	UINT cbImage = 0;
	if (SUCCEEDED(hr))
	{
		// Size of the image, represented in bytes
		hr = UIntMult(cbStride, height, &cbImage);
	}

	// Extract the image into the HBITMAP    
	if (SUCCEEDED(hr))
	{
		hr = pToRenderBitmapSource->CopyPixels(
			NULL,
			cbStride,
			cbImage,
			reinterpret_cast<BYTE *> (m_pRaster));
	}

	//// TODO : AC_SRC_ALPHA 처리시에는 미리곱해진알파(premultiplied alpha) 처리를 반드시 해야 한다.
	//// 그렇지 않으면 투명과 불투명처리는 되지만 반투명처리가 되지 않는다.
	//if (pBitInfo->bmiHeader.biBitCount == 32)
	{
		for (int j = 0; j < height; ++j)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[j * width];
			for (int i = 0; i < width; ++i)
			{
				pbSrcRGB[0] = (pbSrcRGB[0] * pbSrcRGB[3]) / 255;
				pbSrcRGB[1] = (pbSrcRGB[1] * pbSrcRGB[3]) / 255;
				pbSrcRGB[2] = (pbSrcRGB[2] * pbSrcRGB[3]) / 255;
				pbSrcRGB += 4;
			}
		}
	}
	// Image Extraction failed, clear allocated memory
	if (FAILED(hr))
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	return hr;
}
TBitmap::TBitmap()
{

}

TBitmap::~TBitmap()
{
}