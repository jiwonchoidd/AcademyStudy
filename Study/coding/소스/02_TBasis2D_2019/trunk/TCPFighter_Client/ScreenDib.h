
#ifndef __SCREEN_DIB__
#define __SCREEN_DIB__

/*---------------------------------------------------------------

DIB�� ����� GDI�� ��ũ�� ����.
�������� HDC �� DIB �� ����Ͽ� �׸��� ����ش�.


---------------------------------------------------------------*/

class CScreenDib
{
public:

	////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	////////////////////////////////////////////////////////////////////
			CScreenDib(int iWidth, int iHeight, int iColorBit);
	virtual ~CScreenDib();

protected:

	///////////////////////////////////////////////////////
	// DibBuffer Create.
	//
	// Parameters: (int)Width. (int)Height. (int)ColorBit.
	// Return: ����.
	///////////////////////////////////////////////////////
	void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);

	///////////////////////////////////////////////////////
	// DibBuffer Release.
	//
	// Parameters: ����.
	// Return: ����.
	///////////////////////////////////////////////////////
	void ReleaseDibBuffer(void);

public:

	///////////////////////////////////////////////////////
	// DC �� ��´�.
	//
	// Parameters: (HWND)������ �ڵ�. (int)(int)��ġ.
	// Return: ����.
	///////////////////////////////////////////////////////
	void DrawBuffer(HWND hWnd, int iX = 0, int iY = 0);

	///////////////////////////////////////////////////////
	// ���������� ���.
	//
	// Parameters: ����.
	// Return: (BYTE *)����������.
	///////////////////////////////////////////////////////
	BYTE *GetDibBuffer(void);

	///////////////////////////////////////////////////////
	// ��ũ�� ���� ���.
	//
	// Parameters: ����.
	// Return: (int)Width, Height, Pitch.
	///////////////////////////////////////////////////////
	int GetWidth(void);
	int GetHeight(void);
	int GetPitch(void);


protected:


	//------------------------------------------------------------------
	// Dib ����
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

