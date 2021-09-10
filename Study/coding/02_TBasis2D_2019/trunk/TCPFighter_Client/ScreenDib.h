
#ifndef __SCREEN_DIB__
#define __SCREEN_DIB__

/*---------------------------------------------------------------

DIB를 사용한 GDI용 스크린 버퍼.
윈도우의 HDC 에 DIB 를 사용하여 그림을 찍어준다.


---------------------------------------------------------------*/

class CScreenDib
{
public:

	////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	////////////////////////////////////////////////////////////////////
			CScreenDib(int iWidth, int iHeight, int iColorBit);
	virtual ~CScreenDib();

protected:

	///////////////////////////////////////////////////////
	// DibBuffer Create.
	//
	// Parameters: (int)Width. (int)Height. (int)ColorBit.
	// Return: 없음.
	///////////////////////////////////////////////////////
	void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);

	///////////////////////////////////////////////////////
	// DibBuffer Release.
	//
	// Parameters: 없음.
	// Return: 없음.
	///////////////////////////////////////////////////////
	void ReleaseDibBuffer(void);

public:

	///////////////////////////////////////////////////////
	// DC 에 찍는다.
	//
	// Parameters: (HWND)윈도우 핸들. (int)(int)위치.
	// Return: 없음.
	///////////////////////////////////////////////////////
	void DrawBuffer(HWND hWnd, int iX = 0, int iY = 0);

	///////////////////////////////////////////////////////
	// 버퍼포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (BYTE *)버퍼포인터.
	///////////////////////////////////////////////////////
	BYTE *GetDibBuffer(void);

	///////////////////////////////////////////////////////
	// 스크린 정보 얻기.
	//
	// Parameters: 없음.
	// Return: (int)Width, Height, Pitch.
	///////////////////////////////////////////////////////
	int GetWidth(void);
	int GetHeight(void);
	int GetPitch(void);


protected:


	//------------------------------------------------------------------
	// Dib 정보
	//------------------------------------------------------------------
	BITMAPINFO	m_stDibInfo;
	BYTE		*m_bypBuffer;

	int			m_iWidth;
	int			m_iHeight;
	int			m_iPitch;
	int			m_iColorBit;
	int			m_iBufferSize;
};

#endif

