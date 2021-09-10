#include "TObject.h"
#include "TObjectMgr.h"

DWORD TObject::Overlap(int iIndex)
{
	auto iter = m_OverlapState.find(iIndex);
	if (iter == m_OverlapState.end())
	{
		return OVERLAP_NONE;
	}
	return iter->second;
}
void TObject::HitOverlap(TObject * pDesk, DWORD dwState)
{
#ifdef _DEBUG
	T_STR szMsg = this->m_szName;
	szMsg += L"<->";
	if (pDesk != nullptr)
	{
		szMsg += pDesk->m_szName;
	}
	else
	{
		szMsg += L"none";
	}
	if (m_dwOverlapState == TOverlapState::OVERLAP_BEGIN)
	{
		szMsg += L" OVERLAP_BEGIN ";
		log(1, szMsg.c_str());
	}
	/*if (m_dwOverlapState == TOverlapState::OVERLAP_STAY)
	{
		szMsg += L" OVERLAP_STAY ";
	}	*/
	if (m_dwOverlapState == TOverlapState::OVERLAP_END)
	{
		szMsg += L" OVERLAP_END\n ";
		log(1, szMsg.c_str());
	}
	//szMsg += L"\n";
	//log(1, szMsg.c_str());
#endif
}
void TObject::MouseOverlap(POINT ptMouse, DWORD dwState)
{
	if (dwState == TSelectState::T_SELECTED)
	{
		m_bSelect = true;
	}
#ifdef _DEBUG
	//T_STR szMsg = this->m_szName;
	//DWORD dwSelect = m_dwSelectState;
	///*if( dwSelect == TSelectState::T_DEFAULT)
	//{
	//	szMsg += L" Default ";
	//}
	//if (m_dwSelectState & TSelectState::T_HOVER)
	//{
	//	szMsg += L" Hover ";
	//}*/
	//if (m_dwSelectState & TSelectState::T_ACTIVE)
	//{
	//	szMsg += L" Active\n";		
	//	log(1, szMsg.c_str());
	//}
	////if (m_dwSelectState & TSelectState::T_FOCUS)
	////{
	////	szMsg += L" Focus ";
	////}	
	//if (m_dwSelectState & TSelectState::T_SELECTED)
	//{
	//	szMsg += L" Selected\n";
	//	log(1, szMsg.c_str());
	//}
	////szMsg += L"\n";
	////log(1, szMsg.c_str());
#endif
}
TObject* TObject::Clone()
{
	return new TObject(*this);
}
bool  TObject::Reset()
{
	return true;
}
void  TObject::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}
void  TObject::SetDir(float* p)
{
	m_ptDirection.x = p[0];
	m_ptDirection.y = p[1];
}
void  TObject::SetPos(float* d)
{
	m_ptPos.x = d[0];
	m_ptPos.y = d[1];
}
void  TObject::SetDir(TPoint p)
{
	m_ptDirection = p;
}
// 영역의 중앙이 원점이 되며 화면 밖을 벗어 나지 못하도록 한다.
void  TObject::SetPos(TPoint p)
{
	if (p.x < g_rtClient.left+ (m_rtSrc.right / 2))
	{
		p.x = g_rtClient.left+ (m_rtSrc.right / 2);
	}
	if (p.y < g_rtClient.top+(m_rtSrc.bottom / 2))
	{
		p.y = g_rtClient.top+(m_rtSrc.bottom / 2);
	}
	if (p.x > g_rtClient.right - (m_rtSrc.right / 2))
	{
		p.x = g_rtClient.right - (m_rtSrc.right / 2);
	}
	if (p.y > g_rtClient.bottom - (m_rtSrc.bottom / 2))
	{
		p.y = g_rtClient.bottom - (m_rtSrc.bottom / 2);
	}
	m_ptPos = p;
	Set(p, m_rtSrc);
}
// 상단-좌측 코너가 원점이 된다.
void  TObject::SetPosTL(TPoint p)
{
	m_ptPos = p;
	m_rtDesk.left = p.x;
	m_rtDesk.top = p.y;

	m_rtDraw = m_rtDesk;
	m_rtCollide = m_rtDesk;
	m_rtCollide.right  += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
bool  TObject::Init()
{
	m_pMaskBmp		= nullptr;
	m_pColorBmp		= nullptr;
	g_ObjectMgr.AddCollisionExecute(this, bind(&TObject::HitOverlap, this, std::placeholders::_1, std::placeholders::_2));
	g_ObjectMgr.AddSelectExecute(this, bind(&TObject::MouseOverlap, this, std::placeholders::_1, std::placeholders::_2));
	//g_ObjectMgr.m_fnExecute[m_iCollisionObjectID] = bind(&TObject::HitOverlap, this, std::placeholders::_1);
	return true;
}
void  TObject::Update()
{
}
// 상단-좌측 코너가 원점이 된다.
void  TObject::Set(RECT rtSrc, RECT rtDesk)
{
	if( rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	if (rtDesk.left >= 0) m_rtDesk.left = rtDesk.left;
	if (rtDesk.top >= 0) m_rtDesk.top = rtDesk.top;
	if (rtDesk.right >= 0) m_rtDesk.right = rtDesk.right;
	if (rtDesk.bottom >= 0) m_rtDesk.bottom = rtDesk.bottom;
	m_ptPos.x = m_rtDesk.left;
	m_ptPos.y = m_rtDesk.top;

	m_rtDraw = m_rtDesk;
	m_rtCollide = m_rtDesk;
	m_rtCollide.right += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
// 영역의 중앙이 원점이 된다.
void  TObject::Set(TPoint p, RECT rtSrc)
{
	m_ptPos = p;
	if (rtSrc.left >= 0) m_rtSrc.left = rtSrc.left;
	if (rtSrc.top >= 0) m_rtSrc.top = rtSrc.top;
	if (rtSrc.right >= 0) m_rtSrc.right = rtSrc.right;
	if (rtSrc.bottom >= 0) m_rtSrc.bottom = rtSrc.bottom;
	m_rtDesk.left = m_ptPos.x - (m_rtSrc.right / 2);
	m_rtDesk.top = m_ptPos.y - (m_rtSrc.bottom / 2);
	m_rtDesk.right = m_rtSrc.right;
	m_rtDesk.bottom = m_rtSrc.bottom;

	m_rtDraw = m_rtDesk;
	m_rtCollide = m_rtDesk;
	m_rtCollide.right += m_rtDesk.left;
	m_rtCollide.bottom += m_rtDesk.top;
}
bool  TObject::Load(const TCHAR* color,	const TCHAR* mask,DWORD dwColor)
{
	if (color != nullptr)
	{
		m_pColorBmp = g_BitmapMgr.Load(color);
		if (m_pColorBmp != nullptr)
		{
			Set({ 0, 0,
				m_pColorBmp->m_BitmapInfo.bmWidth,
				m_pColorBmp->m_BitmapInfo.bmHeight },
				{ 0,0,
				m_pColorBmp->m_BitmapInfo.bmWidth,
				m_pColorBmp->m_BitmapInfo.bmHeight });
		}
	}
	if (mask != nullptr)
	{
		m_pMaskBmp = g_BitmapMgr.Load(mask);
	}
	if (mask == nullptr)
	{
		m_bColorKey = true;
	}
	m_dwColorKey = dwColor;
	return true;
}
bool  TObject::PreFrame()
{
	return true;
}
bool  TObject::Frame()
{
	PreFrame();
	for (int iChild = 0; iChild < m_pChildObjects.size(); iChild++)
	{		
		m_pChildObjects[iChild]->Frame();
	}
	PostFrame();
	return true;
}
bool  TObject::PostFrame()
{
	return true;
}
bool  TObject::Release()
{
	for (int iChild = 0; iChild < m_pChildObjects.size(); iChild++)
	{
		m_pChildObjects[iChild]->Release();
		delete m_pChildObjects[iChild];
	}
	m_pChildObjects.clear();
	return true;
}
bool TObject::PreRender()
{
	return true;
}
bool  TObject::Render()
{	
	// 부모의 위치에서 상대적으로 위치하게 된다. 
	m_rtDraw = m_rtDesk;
	if (m_pParentObject != nullptr)
	{
		m_rtDraw.left += m_pParentObject->m_rtDesk.left;
		m_rtDraw.top += m_pParentObject->m_rtDesk.top;
	}
	if (m_pColorBmp)
	{
		if (m_bColorKey == true)
		{
			DrawColorKey();
		}
		else
		{
			if (m_pColorBmp->m_BitmapInfo.bmBitsPixel == 32 && m_pMaskBmp == nullptr)
			{
				m_pColorBmp->DrawAlphaBlend(m_rtDraw, m_rtSrc);
			}
			else
			{
				if (m_pMaskBmp != nullptr)
				{
					m_pMaskBmp->Draw(m_rtDraw, m_rtSrc, SRCAND, 0);
					m_pColorBmp->Draw(m_rtDraw, m_rtSrc, SRCINVERT, 0);
					m_pMaskBmp->Draw(m_rtDraw, m_rtSrc, SRCINVERT, 0);
				}
				else
				{
					m_pColorBmp->Draw(m_rtDraw, m_rtSrc, SRCCOPY, 0);
				}
			}
		}
	}
	return PostRender();	
}
bool TObject::PostRender()
{
	for (int iChild = 0; iChild < m_pChildObjects.size(); iChild++)
	{
		m_pChildObjects[iChild]->Render();
	}
	return true;
}
void  TObject::DrawColorKey()
{
	m_pColorBmp->DrawColorKey(
		m_rtDesk, 
		m_rtSrc,
		m_dwColorKey);
}

TObject::TObject()
{
	m_iCollisionObjectID = -1;
	m_bCollisionEnabled = false;
	m_iCollisionType = TCollisionType::T_IGNORE;
	m_dwOverlapState = TOverlapState::OVERLAP_NONE;
	m_fAlphaBlend = 255.0f;
	m_fLifeTime = -1.0f;
	m_iObjectType = -1;
	m_bDead = false;
	m_ptDirection.x = 0.0f;
	m_ptDirection.y = 0.0f;
	m_fSpeed = 50.0f;
	m_bColorKey  = false;
	m_dwColorKey = RGB(255, 255, 255);
	m_bSelect = false;
	m_bDrawDebugRect = false;
	m_iSortValue = 0;
}
TObject::~TObject()
{	
}