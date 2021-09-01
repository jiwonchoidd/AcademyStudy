#include "TUserInterface.h"
#include "TScene.h"
#include "TWrite.h"
int TEdit::g_iControlID = 1000;
LRESULT	 TUserInterface::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (auto iter : m_pChildObjects)
	{
		TUserInterface* pUI = (TUserInterface*)iter;
		int iRet = pUI->MsgProc(hWnd, msg, wParam, lParam);
		if (iRet >= 0) return iRet;
	}
	return -1;
}
TObject* TUserInterface::Clone()
{
	return new TUserInterface(*this);
}
bool  TUserInterface::PostFrame()
{
	return true;
}
TPoint  TActor::Gen(float x, float y)
{
	TPoint vRet(x, y);
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
	pos = Gen(m_rtInit.left + m_rtInit.right, m_rtInit.top);
	m_VertexList[1].p = TPoint(pos.x, pos.y);
	m_VertexList[1].t = TPoint(1.0f, 0.0f);
	pos = Gen(m_rtInit.left, m_rtInit.top + m_rtInit.bottom);
	m_VertexList[2].p = TPoint(pos.x, pos.y);
	m_VertexList[2].t = TPoint(0.0f, 1.0f);
	m_VertexList[3] = m_VertexList[2];
	m_VertexList[4] = m_VertexList[1];
	pos = Gen(m_rtInit.left + m_rtInit.right, m_rtInit.top + m_rtInit.bottom);
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
	m_Actor[6].SetData(m_cpPoint[8].x, m_cpPoint[8].y, m_cpPoint[9].x - m_cpPoint[8].x, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[6].m_VertexList[0].t = m_Actor[3].m_VertexList[2].t;
	m_Actor[6].m_VertexList[1].t = m_Actor[3].m_VertexList[5].t;
	m_Actor[6].m_VertexList[2].t.x = 0.0f;
	m_Actor[6].m_VertexList[2].t.y = 1.0f;
	m_Actor[6].m_VertexList[3] = m_Actor[6].m_VertexList[2];
	m_Actor[6].m_VertexList[4] = m_Actor[6].m_VertexList[1];
	m_Actor[6].m_VertexList[5].t.x = m_Actor[6].m_VertexList[1].t.x;
	m_Actor[6].m_VertexList[5].t.y = m_Actor[6].m_VertexList[2].t.y;

	m_Actor[7].SetData(m_cpPoint[9].x, m_cpPoint[9].y, m_cpPoint[10].x - m_cpPoint[9].x, m_cpPoint[12].y - m_cpPoint[8].y);
	m_Actor[7].m_VertexList[0].t = m_Actor[4].m_VertexList[3].t;
	m_Actor[7].m_VertexList[1].t = m_Actor[4].m_VertexList[5].t;
	m_Actor[7].m_VertexList[2].t.x = m_tOffset.fOffsetU0;
	m_Actor[7].m_VertexList[2].t.y = 1.0f;
	m_Actor[7].m_VertexList[3] = m_Actor[7].m_VertexList[2];
	m_Actor[7].m_VertexList[4] = m_Actor[7].m_VertexList[1];
	m_Actor[7].m_VertexList[5].t.x = m_Actor[7].m_VertexList[1].t.x;
	m_Actor[7].m_VertexList[5].t.y = m_Actor[7].m_VertexList[2].t.y;

	m_Actor[8].SetData(m_cpPoint[10].x, m_cpPoint[10].y, m_cpPoint[11].x - m_cpPoint[10].x, m_cpPoint[12].y - m_cpPoint[8].y);
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
bool  TDialogBox::Init()
{
	TObject::Init();
	////////////////////////////////////// 원본소스 좌표계 ////////////////////////////////////////
	//RECT rt = { 0, 0,m_pImage->m_bmpInfo.bmWidth,m_pImage->m_bmpInfo.bmHeight };
	//// 위치값을 width  / 2 = x
	////          height / 2 = y;
	//SetSourceRect(m_pImage->m_bmpInfo.bmWidth / 2, m_pImage->m_bmpInfo.bmHeight / 2, rt);
	////////////////////////////////////// 화면 좌표계 ////////////////////////////////////////////
	// 20 --> 좌(10), 우(w= 좌+우)
	RECT rtClient;
	rtClient.left = 0;
	rtClient.top = 0;
	rtClient.right = g_rtClient.right - rtClient.left * 2;
	rtClient.bottom = g_rtClient.bottom - rtClient.top * 2;

	SetData(rtClient,	0.1f, 0.1f,     // 화면 좌우 스케일 00, 02
						0.1f, 0.1f,		// 화면 상하 스케일 00, 20
						0.2f, 0.2f,		// 원본 좌우 소스 스케일  00, 02
						0.33f, 0.33f);  // 원본 상하 소스 스케일  00, 20
	return true;
}

void  TDialogBox::Set(RECT rtSrc, RECT rtDesk)
{
	TObject::Set(rtSrc, rtDesk);
	SetData(m_rtDesk, 0.1f, 0.1f,     // 화면 좌우 스케일 00, 02
		0.1f, 0.1f,		// 화면 상하 스케일 00, 20
		0.2f, 0.2f,		// 원본 좌우 소스 스케일  00, 02
		0.33f, 0.33f);  // 원본 상하 소스 스케일  00, 20

}
void  TDialogBox::Set(TPoint p, RECT rtSrc)
{
	TObject::Set(p, rtSrc);
	SetData(m_rtDesk, 0.1f, 0.1f,     // 화면 좌우 스케일 00, 02
		0.1f, 0.1f,		// 화면 상하 스케일 00, 20
		0.2f, 0.2f,		// 원본 좌우 소스 스케일  00, 02
		0.33f, 0.33f);  // 원본 상하 소스 스케일  00, 20
}
bool  TDialogBox::Render()
{
	for (int iRect = 0; iRect < 9; iRect++)
	{
		RECT rt;
		rt.left = m_Actor[iRect].m_VertexList[0].t.x*m_rtSrc.right;
		rt.top = m_Actor[iRect].m_VertexList[0].t.y*m_rtSrc.bottom;
		rt.right = m_Actor[iRect].m_VertexList[5].t.x*m_rtSrc.right - rt.left;
		rt.bottom = m_Actor[iRect].m_VertexList[5].t.y*m_rtSrc.bottom - rt.top;

		// 실수를 정수화 할 때 소수점 오류를 보완하기 위해서 처리함.(소수점 합산처리 후 정수화)
		TPoint size;
		size.x = m_Actor[iRect].m_rtInit.left + m_Actor[iRect].m_rtInit.right;
		size.y = m_Actor[iRect].m_rtInit.top + m_Actor[iRect].m_rtInit.bottom;

		// 부모의 위치에서 상대적으로 위치하게 된다. 		
		RECT rtDraw = { m_Actor[iRect].m_rtInit.left,m_Actor[iRect].m_rtInit.top };
		if (m_pParentObject != nullptr)
		{
			rtDraw.left += m_pParentObject->m_rtDesk.left;
			rtDraw.top += m_pParentObject->m_rtDesk.top;
		}
		m_Actor[iRect].m_rtDraw = rtDraw;
		m_Actor[iRect].m_rtCollide = rtDraw;
		m_Actor[iRect].m_rtCollide.right += m_Actor[iRect].m_rtCollide.left;
		m_Actor[iRect].m_rtCollide.bottom += m_Actor[iRect].m_rtCollide.top;

		RECT rtDesk = { rtDraw.left, rtDraw.top,m_Actor[iRect].m_rtInit.right,m_Actor[iRect].m_rtInit.bottom };
		m_pColorBmp->DrawAlphaBlend(rtDesk, rt, (BYTE)m_fAlphaBlend);

		if (m_bDrawDebugRect)
		{
			SetROP2(g_hOffScreenDC, R2_NOTXORPEN);
			Rectangle(g_hOffScreenDC,rtDesk.left, rtDesk.top, rtDesk.left + rtDesk.right, rtDesk.top + rtDesk.bottom);
		}
	}
	PostRender();
	return true;
}
TObject* TButton::Clone()
{
	return new TButton(*this);
}
bool   TButton::Frame()
{
	return true;
}
bool   TButton::Render()
{
	RECT rtDraw = m_rtDesk;
	if (m_pParentObject != nullptr)
	{
		rtDraw.left += m_pParentObject->m_rtDesk.left;
		rtDraw.top += m_pParentObject->m_rtDesk.top;
		m_rtCollide = rtDraw;
		m_rtCollide.right += m_rtCollide.left;
		m_rtCollide.bottom += m_rtCollide.top;
	}
	if (m_pColorBmp == nullptr) return true;

	int iCurrentState = m_iImageState;
	if (m_StateBitmap[iCurrentState] == nullptr)
	{
		iCurrentState = 0;
	}
	if (m_bColorKey == true)
	{
		TBitmap* pCurrent = m_pColorBmp;	
		(m_StateBitmap[iCurrentState] != nullptr) ?
			pCurrent = m_StateBitmap[iCurrentState] :
			pCurrent = nullptr;	
		if (pCurrent != nullptr)
		{
			pCurrent->DrawColorKey(
				rtDraw,
				m_rtSrc,
				m_dwColorKey);
		}
		return PostRender();
	}
	if (m_pColorBmp &&
		m_pColorBmp->m_BitmapInfo.bmBitsPixel == 32)
	{
		switch (iCurrentState)
		{
		case 0: {m_pColorBmp->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		case 1: {
			m_StateBitmap[1]->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		case 2: {
			m_StateBitmap[2]->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		case 3: {
			m_StateBitmap[3]->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		}
		return true;
	}
	if (m_pMaskBmp != nullptr)
	{
		m_pMaskBmp->Draw(rtDraw, m_rtSrc, SRCAND, 0);
		m_pColorBmp->Draw(rtDraw, m_rtSrc, SRCINVERT, 0);
		m_pMaskBmp->Draw(rtDraw, m_rtSrc, SRCINVERT, 0);
	}
	else
	{
		switch (iCurrentState)
		{			
		case 0: {m_pColorBmp->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
		case 1: {m_StateBitmap[1]->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
		case 2: {m_StateBitmap[2]->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
		case 3: {m_StateBitmap[3]->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
		}

	}
	return PostRender();
}
LRESULT	 TEdit::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			int  iID = LOWORD(wParam);
			if (TCollision::RectInPt(g_rtClient, g_Input.GetPos()))
			{
				if (!TCollision::RectInPt(m_rtWndCtrl, g_Input.GetPos()))
				{
					::SetFocus(g_hWnd);
					m_bEditFocus = false;
					g_Input.SetEnable();
				}
			}
		}break;
		case WM_COMMAND:
		{
			HWND hWndCtrl = (HWND)lParam;
			int  iID = LOWORD(wParam);
			int  iNotify = HIWORD(wParam);
			if(m_iWindowCtrl == iID)
			{
				switch (iNotify)
				{
				case EN_CHANGE:
				{
					TCHAR szBuffer[256] = { 0, };
					GetWindowText(m_hWndEdit, szBuffer, MAX_PATH);
					m_szEdit = szBuffer;
				}break;
				case EN_SETFOCUS:
				{
					m_bEditFocus = true;
					g_Input.SetEnable(false);
				}break;
				case EN_KILLFOCUS:
				{
					m_bEditFocus = false;
					g_Input.SetEnable();
				}break;
				}
			}		
		}break;
	}
	return -1;// m_Network.MsgProc(hWnd, msg, wParam, lParam);
}
TObject* TEdit::Clone()
{
	return new TEdit(*this);
}
void   TEdit::CreateEdit()
{
	DWORD style = WS_CHILD | WS_VISIBLE;
	if (m_hWndEdit == NULL)
	{
		g_iControlID += 100;
		m_iWindowCtrl = g_iControlID;
		m_hWndEdit = CreateWindow(L"kgcaEdit", NULL, style,
			m_rtWndCtrl.left, m_rtWndCtrl.top, m_rtWndCtrl.right, m_rtWndCtrl.bottom,
			g_hWnd, (HMENU)g_iControlID, g_hInstance, NULL);
		SendMessage(m_hWndEdit, WM_SETFONT, (WPARAM)g_Write.m_hFont, 0);
		if (this->m_szName != L"PASSEDIT")
		{
			SendMessage(m_hWndEdit, WM_SETTEXT, 0, (LPARAM)L"kgca");
			m_szEdit = L"kgca";
		}
		else
		{
			SendMessage(m_hWndEdit, WM_SETTEXT, 0, (LPARAM)L"game");
			m_szEdit = L"game";
		}
		
	}
}
bool   TEdit::Init()
{
	TObject::Init();
	m_hWndEdit = NULL;
	return true;
}
bool   TEdit::Frame()
{
	return true;
}

bool   TEdit::Render()
{
	RECT rtDraw = m_rtDesk;
	if (m_pParentObject != nullptr)
	{
		rtDraw.left += m_pParentObject->m_rtDesk.left;
		rtDraw.top += m_pParentObject->m_rtDesk.top;
		m_rtCollide = rtDraw;
		m_rtCollide.right += m_rtCollide.left;
		m_rtCollide.bottom += m_rtCollide.top;
	}
	int iCurrentState = m_iImageState;
	if (m_StateBitmap[iCurrentState] == nullptr )
	{
		iCurrentState = 0;
	}
	if (m_bColorKey == true)
	{
		TBitmap* pCurrent = m_pColorBmp;
		
		(m_StateBitmap[iCurrentState] != nullptr) ?
			pCurrent = m_StateBitmap[iCurrentState] :
			pCurrent = nullptr;
		if (pCurrent != nullptr)
		{
			pCurrent->DrawColorKey(rtDraw,m_rtSrc,m_dwColorKey);
		}
		
		return PostRender();
	}
	if (m_pColorBmp &&
		m_pColorBmp->m_BitmapInfo.bmBitsPixel == 32)
	{
		switch (iCurrentState)
		{
		case 0: {m_pColorBmp->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		case 1: {
			m_StateBitmap[1]->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		case 2: {
			m_StateBitmap[2]->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		case 3: {
			m_StateBitmap[3]->DrawAlphaBlend(rtDraw, m_rtSrc); }break;
		}		
	}
	else
	{
		if (m_pMaskBmp != nullptr)
		{
			m_pMaskBmp->Draw(rtDraw, m_rtSrc, SRCAND, 0);
			m_pColorBmp->Draw(rtDraw, m_rtSrc, SRCINVERT, 0);
			m_pMaskBmp->Draw(rtDraw, m_rtSrc, SRCINVERT, 0);
		}
		else
		{
			switch (iCurrentState)
			{
			case 0: {m_pColorBmp->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
			case 1: {m_StateBitmap[1]->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
			case 2: {m_StateBitmap[2]->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
			case 3: {m_StateBitmap[3]->Draw(rtDraw, m_rtSrc, SRCCOPY, 0); }break;
			}
		}
	}

	g_Write.Draw({ m_rtWndCtrl.left, m_rtWndCtrl.top }, m_szEdit);
	return PostRender();
}