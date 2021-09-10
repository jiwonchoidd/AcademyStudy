#include "Sample.h"
TPoint  TActor::Gen(float x, float y)
{
	TPoint vRet(x,y);		
	//// 0 ~ 800 - > 0 ~1 -> -1 ~ +1
	//vRet.x = x / 800;
	//vRet.y = y / 600;
	//vRet.x = vRet.x * 2.0f - 1.0f;
	//vRet.y = (vRet.y * 2 - 1.0f)*-1.0f;
	return vRet;
}
void    TActor::GenCenter()
{
	m_vCenter.x = 0.0f;
	m_vCenter.y = 0.0f;
	for (int iV = 0; iV < 6; iV++)
	{
		TPoint vertex = m_VertexList[iV].p;
		m_vCenter.x += vertex.x;
		m_vCenter.y += vertex.y;
	}
	m_vCenter.x /= 6.0f;
	m_vCenter.y /= 6.0f;
}
bool    TActor::SetData(float left, float top, float width, float height)
{
	m_rtInit.left = left;
	m_rtInit.right = width;
	m_rtInit.top = top;
	m_rtInit.bottom = height;
	SetVertexData();
	return true;
}
void   TActor::SetVertexData()
{
	TPoint pos = Gen(m_rtInit.left, m_rtInit.top);
	m_VertexList[0].p = TPoint(pos.x, pos.y);
	m_VertexList[0].t = TPoint(0.0f, 0.0f);
	pos = Gen(m_rtInit.left + m_rtInit.right,	m_rtInit.top);
	m_VertexList[1].p = TPoint(pos.x, pos.y);
	m_VertexList[1].t = TPoint(1.0f, 0.0f);
	pos = Gen(m_rtInit.left,	m_rtInit.top + m_rtInit.bottom);
	m_VertexList[2].p = TPoint(pos.x, pos.y);
	m_VertexList[2].t = TPoint(0.0f, 1.0f);
	m_VertexList[3] = m_VertexList[2];
	m_VertexList[4] = m_VertexList[1];
	pos = Gen(m_rtInit.left + m_rtInit.right,	m_rtInit.top + m_rtInit.bottom);
	m_VertexList[5].p = TPoint(pos.x, pos.y);
	m_VertexList[5].t = TPoint(1.0f, 1.0f);
	GenCenter();
}
bool    TDialogBox::SetData(RECT rt, float fScaleX0, float fScaleX1,
	float fScaleY0, float fScaleY1,
	float fOffsetU0, float fOffsetU1,
	float fOffsetV0, float fOffsetV1)
{
	m_rtInit = rt;
	m_tOffset.fScaleX0 = fScaleX0;
	m_tOffset.fScaleX1 = fScaleX1;
	m_tOffset.fScaleY0 = fScaleY0;
	m_tOffset.fScaleY1 = fScaleY1;
	m_tOffset.fOffsetU0 = fOffsetU0;
	m_tOffset.fOffsetU1 = fOffsetU1;
	m_tOffset.fOffsetV0 = fOffsetV1;
	m_tOffset.fOffsetV0 = fOffsetV1;
	SetVertexData();
	return true;
}

void   TDialogBox::SetVertexData()
{
	m_cpPoint[0] = { m_rtInit.left*1.0f, m_rtInit.top*1.0f };
	m_cpPoint[1] = { m_rtInit.left + m_rtInit.right * m_tOffset.fScaleX0, m_rtInit.top*1.0f };
	m_cpPoint[2] = { m_rtInit.left + m_rtInit.right * (1.0f - m_tOffset.fScaleX1), m_rtInit.top*1.0f };
	m_cpPoint[3] = { m_rtInit.left + m_rtInit.right*1.0f, m_rtInit.left*1.0f };

	m_cpPoint[4] = { m_cpPoint[0].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };
	m_cpPoint[5] = { m_cpPoint[1].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };
	m_cpPoint[6] = { m_cpPoint[2].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };
	m_cpPoint[7] = { m_cpPoint[3].x, m_rtInit.top + m_rtInit.bottom * m_tOffset.fScaleY0 };

	m_cpPoint[8] = { m_cpPoint[0].x, m_rtInit.top + m_rtInit.bottom * (1.0f - m_tOffset.fScaleY1) };
	m_cpPoint[9] = { m_cpPoint[1].x, m_rtInit.top + m_rtInit.bottom * (1.0f - m_tOffset.fScaleY1) };
	m_cpPoint[10] = { m_cpPoint[2].x, m_rtInit.top + m_rtInit.bottom * (1.0f - m_tOffset.fScaleY1) };
	m_cpPoint[11] = { m_cpPoint[3].x, m_rtInit.top + m_rtInit.bottom * (1.0f - m_tOffset.fScaleY1) };

	m_cpPoint[12] = { m_cpPoint[0].x, m_rtInit.top + m_rtInit.bottom*1.0f };
	m_cpPoint[13] = { m_cpPoint[1].x, m_rtInit.top + m_rtInit.bottom*1.0f };
	m_cpPoint[14] = { m_cpPoint[2].x, m_rtInit.top + m_rtInit.bottom*1.0f };
	m_cpPoint[15] = { m_cpPoint[3].x, m_rtInit.top + m_rtInit.bottom*1.0f };



	m_Actor[0].SetData(m_cpPoint[0].x, m_cpPoint[0].y, m_cpPoint[1].x - m_cpPoint[0].x, m_cpPoint[4].y - m_cpPoint[0].y);
	m_Actor[0].m_VertexList[1].t.x = m_tOffset.fOffsetU0;
	m_Actor[0].m_VertexList[1].t.y = 0.0f;
	m_Actor[0].m_VertexList[2].t.x = 0.0f;
	m_Actor[0].m_VertexList[2].t.y = m_tOffset.fOffsetV0;
	m_Actor[0].m_VertexList[3] = m_Actor[0].m_VertexList[2];
	m_Actor[0].m_VertexList[4] = m_Actor[0].m_VertexList[1];
	m_Actor[0].m_VertexList[5].t.x = m_tOffset.fOffsetU0;
	m_Actor[0].m_VertexList[5].t.y = m_tOffset.fOffsetV0;

	m_Actor[1].SetData(m_cpPoint[1].x, m_cpPoint[1].y, m_cpPoint[2].x - m_cpPoint[1].x, m_cpPoint[6].y - m_cpPoint[2].y);
	m_Actor[1].m_VertexList[0].t.x = m_tOffset.fOffsetU0;
	m_Actor[1].m_VertexList[0].t.y = 0.0f;
	m_Actor[1].m_VertexList[1].t.x = 1.0f - m_tOffset.fOffsetU0;
	m_Actor[1].m_VertexList[1].t.y = 0.0f;
	m_Actor[1].m_VertexList[2].t.x = m_Actor[1].m_VertexList[0].t.x;
	m_Actor[1].m_VertexList[2].t.y = m_tOffset.fOffsetV0;
	m_Actor[1].m_VertexList[3] = m_Actor[1].m_VertexList[2];
	m_Actor[1].m_VertexList[4] = m_Actor[1].m_VertexList[1];
	m_Actor[1].m_VertexList[5].t.x = m_Actor[1].m_VertexList[1].t.x;
	m_Actor[1].m_VertexList[5].t.y = m_Actor[1].m_VertexList[2].t.y;

	m_Actor[2].SetData(m_cpPoint[2].x, m_cpPoint[2].y, m_cpPoint[3].x - m_cpPoint[2].x, m_cpPoint[6].y - m_cpPoint[2].y);
	m_Actor[2].m_VertexList[0].t.x = 1.0f - m_tOffset.fOffsetU0;
	m_Actor[2].m_VertexList[0].t.y = 0.0f;
	m_Actor[2].m_VertexList[1].t.x = 1.0f;
	m_Actor[2].m_VertexList[1].t.y = 0.0f;
	m_Actor[2].m_VertexList[2].t.x = m_Actor[2].m_VertexList[0].t.x;
	m_Actor[2].m_VertexList[2].t.y = m_tOffset.fOffsetV0;
	m_Actor[2].m_VertexList[3] = m_Actor[2].m_VertexList[2];
	m_Actor[2].m_VertexList[4] = m_Actor[2].m_VertexList[1];
	m_Actor[2].m_VertexList[5].t.x = m_Actor[2].m_VertexList[1].t.x;
	m_Actor[2].m_VertexList[5].t.y = m_Actor[2].m_VertexList[2].t.y;
#pragma endregion
	/////////////////////////////////////////////////////
#pragma region 3~5
	m_Actor[3].SetData(m_cpPoint[4].x, m_cpPoint[4].y, m_cpPoint[1].x - m_cpPoint[0].x, m_cpPoint[8].y - m_cpPoint[4].y);
	m_Actor[3].m_VertexList[0].t = m_Actor[0].m_VertexList[2].t;
	m_Actor[3].m_VertexList[1].t = m_Actor[0].m_VertexList[5].t;
	m_Actor[3].m_VertexList[2].t.x = 0.0f;
	m_Actor[3].m_VertexList[2].t.y = 1.0f - (m_tOffset.fOffsetV0 - m_tOffset.fOffsetV1);
	m_Actor[3].m_VertexList[3] = m_Actor[3].m_VertexList[2];
	m_Actor[3].m_VertexList[4] = m_Actor[3].m_VertexList[1];
	m_Actor[3].m_VertexList[5].t.x = m_Actor[3].m_VertexList[1].t.x;
	m_Actor[3].m_VertexList[5].t.y = m_Actor[3].m_VertexList[2].t.y;

	m_Actor[4].SetData(m_cpPoint[5].x, m_cpPoint[5].y, m_cpPoint[2].x - m_cpPoint[1].x, m_cpPoint[8].y - m_cpPoint[4].y);
	m_Actor[4].m_VertexList[0].t = m_Actor[3].m_VertexList[1].t;
	m_Actor[4].m_VertexList[1].t.x = m_Actor[1].m_VertexList[1].t.x;
	m_Actor[4].m_VertexList[1].t.y = m_tOffset.fOffsetV0;
	m_Actor[4].m_VertexList[2].t.x = m_tOffset.fOffsetU0;
	m_Actor[4].m_VertexList[2].t.y = 1.0f - (m_tOffset.fOffsetV0 - m_tOffset.fOffsetV1);
	m_Actor[4].m_VertexList[3] = m_Actor[4].m_VertexList[2];
	m_Actor[4].m_VertexList[4] = m_Actor[4].m_VertexList[1];
	m_Actor[4].m_VertexList[5].t.x = m_Actor[4].m_VertexList[1].t.x;
	m_Actor[4].m_VertexList[5].t.y = m_Actor[4].m_VertexList[2].t.y;

	m_Actor[5].SetData(m_cpPoint[6].x, m_cpPoint[6].y, m_cpPoint[3].x - m_cpPoint[2].x, m_cpPoint[8].y - m_cpPoint[4].y);
	m_Actor[5].m_VertexList[0].t = m_Actor[4].m_VertexList[1].t;
	m_Actor[5].m_VertexList[1].t.x = 1.0f;
	m_Actor[5].m_VertexList[1].t.y = m_tOffset.fOffsetV0;
	m_Actor[5].m_VertexList[2].t = m_Actor[4].m_VertexList[5].t;
	m_Actor[5].m_VertexList[3] = m_Actor[5].m_VertexList[2];
	m_Actor[5].m_VertexList[4] = m_Actor[5].m_VertexList[1];
	m_Actor[5].m_VertexList[5].t.x = m_Actor[5].m_VertexList[1].t.x;
	m_Actor[5].m_VertexList[5].t.y = m_Actor[5].m_VertexList[2].t.y;
#pragma endregion 3~5
#pragma region 6~9
	m_Actor[6].SetData(m_cpPoint[8].x, m_cpPoint[8].y, m_cpPoint[1].x - m_cpPoint[0].y, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[6].m_VertexList[0].t = m_Actor[3].m_VertexList[2].t;
	m_Actor[6].m_VertexList[1].t = m_Actor[3].m_VertexList[5].t;
	m_Actor[6].m_VertexList[2].t.x = 0.0f;
	m_Actor[6].m_VertexList[2].t.y = 1.0f;
	m_Actor[6].m_VertexList[3] = m_Actor[6].m_VertexList[2];
	m_Actor[6].m_VertexList[4] = m_Actor[6].m_VertexList[1];
	m_Actor[6].m_VertexList[5].t.x = m_Actor[6].m_VertexList[1].t.x;
	m_Actor[6].m_VertexList[5].t.y = m_Actor[6].m_VertexList[2].t.y;

	m_Actor[7].SetData(m_cpPoint[9].x, m_cpPoint[9].y, m_cpPoint[2].x - m_cpPoint[1].x, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[7].m_VertexList[0].t = m_Actor[4].m_VertexList[3].t;
	m_Actor[7].m_VertexList[1].t = m_Actor[4].m_VertexList[5].t;
	m_Actor[7].m_VertexList[2].t.x = m_tOffset.fOffsetU0;
	m_Actor[7].m_VertexList[2].t.y = 1.0f;
	m_Actor[7].m_VertexList[3] = m_Actor[7].m_VertexList[2];
	m_Actor[7].m_VertexList[4] = m_Actor[7].m_VertexList[1];
	m_Actor[7].m_VertexList[5].t.x = m_Actor[7].m_VertexList[1].t.x;
	m_Actor[7].m_VertexList[5].t.y = m_Actor[7].m_VertexList[2].t.y;

	m_Actor[8].SetData(m_cpPoint[10].x, m_cpPoint[10].y, m_cpPoint[3].x - m_cpPoint[2].x, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[8].m_VertexList[0].t = m_Actor[5].m_VertexList[3].t;
	m_Actor[8].m_VertexList[1].t = m_Actor[5].m_VertexList[5].t;
	m_Actor[8].m_VertexList[2].t = m_Actor[7].m_VertexList[5].t;
	m_Actor[8].m_VertexList[3] = m_Actor[8].m_VertexList[2];
	m_Actor[8].m_VertexList[4] = m_Actor[8].m_VertexList[1];
	m_Actor[8].m_VertexList[5].t.x = m_Actor[8].m_VertexList[1].t.x;
	m_Actor[8].m_VertexList[5].t.y = m_Actor[8].m_VertexList[2].t.y;
	/////////////////////////////////////////////
#pragma endregion 3~5

	for (int iRect = 0; iRect < 9; iRect++)
	{
		for (int iV = 0; iV < 6; iV++)
		{
			m_tVertexList.push_back(m_Actor[iRect].m_VertexList[iV]);
		}
	}
}
bool Sample::LoadDIB(const TCHAR* pFileName)
{
	DWORD dwDataSize, dwRead;
	HANDLE hFile = CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	ReadFile(hFile, &m_bitFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	dwDataSize = m_bitFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
	
	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(dwDataSize);
	ReadFile(hFile, bitInfo, dwDataSize, &dwRead, NULL);

	PVOID  pRaster = nullptr;
	m_hBitmap = CreateDIBSection(g_hScreenDC, bitInfo, DIB_RGB_COLORS, &pRaster, NULL, 0);
	ReadFile(hFile, pRaster, m_bitFileHeader.bfSize- m_bitFileHeader.bfOffBits, &dwRead, NULL);
	free(bitInfo);
	CloseHandle(hFile);
		
	GetObject(m_hBitmap, sizeof(BITMAP), &m_bmpInfo);
	m_BitmapSize.x = m_bmpInfo.bmWidth;
	m_BitmapSize.y = m_bmpInfo.bmHeight;
	return true;
}
bool Sample::Init()
{
	m_ui.Init();
	m_ui.Load(L"../../data/Apopup_normal.bmp");
	//LoadDIB(L"../../data/popup_normal.bmp"); //
	LoadDIB(L"../../data/dxutcontrols.bmp");
	////////////////////////////////////// 원본소스 좌표계 ////////////////////////////////////////
	RECT rt = { 0, 0,m_ui.m_pImage->m_bmpInfo.bmWidth,m_ui.m_pImage->m_bmpInfo.bmHeight };
	// 위치값을 width  / 2 = x
	//          height / 2 = y;
	m_ui.SetSourceRect(m_ui.m_pImage->m_bmpInfo.bmWidth/2, m_ui.m_pImage->m_bmpInfo.bmHeight/2, rt);	
	////////////////////////////////////// 화면 좌표계 ////////////////////////////////////////////
	// 20 --> 좌(10), 우(w= 좌+우)
	RECT rtClient;
	rtClient.left	= 0;
	rtClient.top	= 0;
	rtClient.right	= m_rtClient.right - rtClient.left*2;
	rtClient.bottom	= m_rtClient.bottom - rtClient.top*2;

	m_ui.SetData(rtClient,	0.1f, 0.1f,     // 화면 좌우 스케일 00, 02
							0.1f, 0.1f,		// 화면 상하 스케일 00, 20
							0.2f, 0.2f,		// 원본 좌우 소스 스케일  00, 02
							0.33f, 0.33f);  // 원본 상하 소스 스케일  00, 20
	return true;
}
bool Sample::RenderDIB(int x, int y)
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 0xff;
	if (m_bmpInfo.bmBitsPixel == 32)
		bf.AlphaFormat = AC_SRC_ALPHA;
	else
		bf.AlphaFormat = AC_SRC_OVER;

	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, m_hBitmap);
	if (!AlphaBlend(g_hOffScreenDC, x, y, m_BitmapSize.x, m_BitmapSize.y,
		hdc, 0, 0, m_BitmapSize.x, m_BitmapSize.y, bf))
	{
		return false;
	}
	DeleteDC(hdc);
	return true;
}
bool Sample::Render()
{
	static float fAlpha = 0.0f;
	fAlpha += g_fSecPerFrame * 100;
	if (fAlpha > 255) fAlpha = 0.0f;

	for (int iRect = 0; iRect < 9; iRect++)
	{
		RECT rt;
		rt.left = m_ui.m_Actor[iRect].m_VertexList[0].t.x*m_ui.m_SrcRect.right;
		rt.top = m_ui.m_Actor[iRect].m_VertexList[0].t.y*m_ui.m_SrcRect.bottom;
		rt.right = m_ui.m_Actor[iRect].m_VertexList[5].t.x*m_ui.m_SrcRect.right - rt.left;
		rt.bottom = m_ui.m_Actor[iRect].m_VertexList[5].t.y*m_ui.m_SrcRect.bottom - rt.top;

		// 실수를 정수화 할 때 소수점 오류를 보완하기 위해서 처리함.(소수점 합산처리 후 정수화)
		TPoint size;
		size.x = m_ui.m_Actor[iRect].m_rtInit.left + m_ui.m_Actor[iRect].m_rtInit.right;
		size.y = m_ui.m_Actor[iRect].m_rtInit.top + m_ui.m_Actor[iRect].m_rtInit.bottom;

		m_ui.DrawAlphaBlend(m_ui.m_hDrawDC,
			m_ui.m_Actor[iRect].m_rtInit.left, m_ui.m_Actor[iRect].m_rtInit.top,
			size.x - (int)m_ui.m_Actor[iRect].m_rtInit.left,
			size.y - (int)m_ui.m_Actor[iRect].m_rtInit.top,
			m_ui.m_pImage->m_hMemDC, 
			rt.left,
			rt.top,
			rt.right,
			rt.bottom,
			fAlpha);
		if (m_ui.m_bDebugRect)
		{
			SetROP2(m_ui.m_hDrawDC, R2_NOTXORPEN);
			Rectangle(m_ui.m_hDrawDC, 
						m_ui.m_Actor[iRect].m_rtInit.left, m_ui.m_Actor[iRect].m_rtInit.top,
				m_ui.m_Actor[iRect].m_rtInit.left + m_ui.m_Actor[iRect].m_rtInit.right,
				m_ui.m_Actor[iRect].m_rtInit.top + m_ui.m_Actor[iRect].m_rtInit.bottom);
		}
	}
	RenderDIB(400, 300);
	return true;
}
bool Sample::Release()
{
	m_ui.Release();	
	return true;
}
bool Sample::Frame()
{
	return true;
}

Sample::Sample()
{
}


Sample::~Sample()
{
}


TCORE_RUN(UISetRender, 800, 600);