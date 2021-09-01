
/*---------------------------------------------------------------

DIB를 사용한 GDI용 스크린 버퍼.
윈도우의 HDC 에 DIB 를 사용하여 그림을 찍어준다.





---------------------------------------------------------------*/
#include <windows.h>
#include "ScreenDib.h"

////////////////////////////////////////////////////////////////////
// 생성자, 파괴자.
//
////////////////////////////////////////////////////////////////////
CScreenDib::CScreenDib(int iWidth, int iHeight, int iColorBit)
{
	memset(&m_stDibInfo, 0, sizeof(BITMAPINFO));

	m_bypBuffer = NULL;

	m_iWidth = 0;
	m_iHeight = 0;
	m_iPitch = 0;
	m_iBufferSize = 0;
	m_iColorBit = 0;

	//------------------------------------------------------------------
	// 생성자의 인자로 버퍼를 만든다
	//------------------------------------------------------------------
	CreateDibBuffer(iWidth, iHeight, iColorBit);

}

CScreenDib::~CScreenDib()
{
	ReleaseDibBuffer();

}


///////////////////////////////////////////////////////
// DibBuffer Create.
//
// Parameters: (int)Width. (int)Height. (int)ColorBit.
// Return: 없음.
///////////////////////////////////////////////////////
void CScreenDib::CreateDibBuffer(int iWidth, int iHeight, int iColorBit)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iColorBit = iColorBit;

	m_iPitch = ((m_iWidth * (m_iColorBit / 8)) + 3) & ~3;
	m_iBufferSize = m_iPitch * m_iHeight;

	//------------------------------------------------------------------
	// DibInfo 헤더 생성
	//
	// DIB 출력시 뒤집어서 출력하기 위해 높이값을 - 로 입력하고 있다.
	//------------------------------------------------------------------
	m_stDibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stDibInfo.bmiHeader.biWidth = m_iWidth;
	m_stDibInfo.bmiHeader.biHeight = -m_iHeight;
	m_stDibInfo.bmiHeader.biPlanes = 1;
	m_stDibInfo.bmiHeader.biBitCount = m_iColorBit;
	m_stDibInfo.bmiHeader.biCompression = 0;
	m_stDibInfo.bmiHeader.biSizeImage = m_iBufferSize;
	m_stDibInfo.bmiHeader.biXPelsPerMeter = 0;
	m_stDibInfo.bmiHeader.biYPelsPerMeter = 0;
	m_stDibInfo.bmiHeader.biClrUsed = 0;
	m_stDibInfo.bmiHeader.biClrImportant = 0;

	//------------------------------------------------------------------
	// 버퍼 생성
	//------------------------------------------------------------------
	m_bypBuffer = new BYTE[m_iBufferSize];
	memset(m_bypBuffer, 0xff, m_iBufferSize);

}

///////////////////////////////////////////////////////
// DibBuffer Release.
//
// Parameters: 없음.
// Return: 없음.
///////////////////////////////////////////////////////
void CScreenDib::ReleaseDibBuffer(void)
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_iPitch = 0;
	m_iBufferSize = 0;

	memset(&m_stDibInfo, 0x00, sizeof(BITMAPINFO));

	if ( m_bypBuffer != NULL )
		delete[] m_bypBuffer;

	m_bypBuffer = NULL;
}


///////////////////////////////////////////////////////
// DC 에 찍는다.
//
// Parameters: (HWND)윈도우 핸들. (int)(int)위치.
// Return: 없음.
///////////////////////////////////////////////////////
void CScreenDib::DrawBuffer(HWND hWnd, int iX, int iY)
{
	if ( m_bypBuffer == NULL ) return;

	RECT Rect;
	HDC hDC;

	//------------------------------------------------------------------
	// 찍을 대상의 DC 를 얻는다.
	//------------------------------------------------------------------
	GetWindowRect(hWnd, &Rect);
	hDC = GetDC(hWnd);
/*
	bypDest = bypBufferTurn;
	bypSrc = bypBuffer + iBufferScreenX * 2 + (iBufferScreenY + Rect.bottom - Rect.top) * iPitch;

	for ( iCount = 0; iCount < iBufferScreenY + Rect.bottom - Rect.top; iCount++ )
	{
		memcpy(bypDest, bypSrc, iPitch);
		bypDest += iPitch;
		bypSrc -= iPitch;
	}
*/
//	m_stDibInfo.bmiHeader.biHeight = -m_stDibInfo.bmiHeader.biHeight;

	//------------------------------------------------------------------
	// GDI 함수를 사용하여 DC에 출력한다.
	// DIB 이므로 찍을때 뒤집어서 찍어줘야 한다.
	//------------------------------------------------------------------
	int i = SetDIBitsToDevice(hDC,
						0, 0,
						m_iWidth, m_iHeight,
						0, 0,
						0, m_iHeight,

						m_bypBuffer,
						&m_stDibInfo, DIB_RGB_COLORS);

//	m_stDibInfo.bmiHeader.biHeight = -m_stDibInfo.bmiHeader.biHeight;



	//------------------------------------------------------------------
	// 화면에 프레임 표시 부분.
	//------------------------------------------------------------------
	{
		static char		szFrame[5];
		static int		iFrame = 0;
		static DWORD	dwTick = 0;

		iFrame++;
		if ( dwTick + 1000 < timeGetTime() )
		{
			wsprintf(szFrame, "%d", iFrame);
			iFrame = 0;
			dwTick = timeGetTime();
		}
		
		TextOut(hDC, 0, 0, szFrame, (int)strlen(szFrame));
	}

	ReleaseDC(hWnd, hDC);

}

///////////////////////////////////////////////////////
// 스크린 정보 얻기.
//
// Parameters: 없음.
// Return: (int)Width, Height, Pitch.
///////////////////////////////////////////////////////
BYTE * CScreenDib::GetDibBuffer(void)
{
	return m_bypBuffer;
}


int CScreenDib::GetWidth(void)
{
	return m_iWidth;
}

int CScreenDib::GetHeight(void)
{
	return m_iHeight;
}

int CScreenDib::GetPitch(void)
{
    return m_iPitch;
}

