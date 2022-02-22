#include "KUI.h"

bool KUI::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	return true;
}

bool KUI::Frame()
{
	KObject::Frame();
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	m_cbData.vLightDir= KVector4(
		m_fAlpha,
		m_fAlpha,
		m_fAlpha, 1.0f);
	m_cbData.vValue = KVector4(
		g_fSecTimer,
		0,
		0,
		1.0f);
	return true;
}

bool KUI::Render(ID3D11DeviceContext* pContext)
{
	KObject::Render(pContext);
	return true;
}

void KUI::SelectOverlap(KCollider* pObj, DWORD dwState)
{
	//¼±ÅÃ
}

void KUI::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 0.5f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}
}

void KUI::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}

KUI::KUI()
{
	m_pContext = nullptr;
}

KUI::~KUI()
{
}
