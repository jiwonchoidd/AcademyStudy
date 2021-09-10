#include "TSprite.h"
bool  TSprite::Init()
{
	return true;
}
bool  TSprite::Set(TObjectInfo info, int id, int mask)
{
	m_info = info;
	m_info.bLoop = false;
	m_DeskRect = m_info.rtClient;

	if (info.fLifeTime == 0.0f)
	{
		m_info.bLoop = true;
	}
	if (m_info.iBitmapID == -1)
	{
		m_pImage = I_ImageMgr.GetPtr(id);
	}
	else
	{
		m_pImage = I_ImageMgr.GetPtr(m_info.iBitmapID);
	}
	if (m_info.iMaskID == -1)
	{
		m_pImageMask = I_ImageMgr.GetPtr(mask);
	}
	else
	{
		m_pImageMask = I_ImageMgr.GetPtr(m_info.iMaskID);
	}
	m_fDeltaTime = m_info.fTime / m_info.iNumFrame;
	SetPos(m_info.InitPos);
	return true;
}
bool  TSprite::Frame()
{
	if (m_info.bLoop == false)
	{
		m_info.fLifeTime -= g_fSecPerFrame;
		if (m_info.fLifeTime <= 0) m_bDead = true;
	}
	if (m_bDead == true) return true;
	if (m_info.iNumFrame <= 1) return true;

	m_fTmpTime += g_fSecPerFrame;
	if (m_fTmpTime > m_fDeltaTime)
	{
		m_iCurrentFrame++;
		if (m_iCurrentFrame >= m_info.iNumFrame)
		{
			m_iCurrentFrame = 0;
		}
		m_fTmpTime = 0.0f;
	}
	return true;
}
bool  TSprite::Render()
{
	if (m_bDead == true) return true;

	RECT rt = m_info.rtList[m_iCurrentFrame];
	rt.left = m_vPos.x - (rt.right / 2);
	rt.top = m_vPos.y - (rt.bottom / 2);

	if (m_pImageMask != nullptr)
	{
		m_pImageMask->Draw(rt.left, rt.top,
			m_info.rtList[m_iCurrentFrame], SRCAND);
		m_pImage->Draw(rt.left, rt.top,
			m_info.rtList[m_iCurrentFrame], SRCINVERT);
		m_pImageMask->Draw(rt.left, rt.top,
			m_info.rtList[m_iCurrentFrame], SRCINVERT);
	}
	else
	{
		m_pImage->Draw(rt.left, rt.top,	m_info.rtList[m_iCurrentFrame], MERGEPAINT);
	}
	return true;
};
void  TSprite::Draw(DWORD dwMode)
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = m_fAlpha;
	if (m_pImage == nullptr) return;
	if (m_pImage->m_bmpInfo.bmBitsPixel == 32)
	{
		bf.AlphaFormat = AC_SRC_ALPHA;
		m_pImage->Draw(m_DeskRect.left, m_DeskRect.top,	m_info.rtList[m_iCurrentFrame], bf);
	}
	else
	{
		bf.AlphaFormat = AC_SRC_OVER;
		m_pImage->Draw(m_DeskRect.left, m_DeskRect.top,	m_info.rtList[m_iCurrentFrame], dwMode);
	}
}
void  TSprite::DrawCenter(DWORD dwMode)
{
	RECT rt = m_info.rtList[m_iCurrentFrame];
	rt.left = m_vPos.x - (rt.right / 2);
	rt.top = m_vPos.y - (rt.bottom / 2);
	m_pImage->Draw(rt.left, rt.top,	m_info.rtList[m_iCurrentFrame], dwMode);
}
void  TSprite::Draw(int x, int y, DWORD dwMode)
{
	m_pImage->Draw(x, y,m_info.rtList[m_iCurrentFrame], dwMode);
}
bool  TSprite::Release()
{
	m_info.rtList.clear();
	return true;
}

TSprite::TSprite()
{
	m_fTmpTime = 0.0f;
	m_fDeltaTime = 0.0f;
	m_iCurrentFrame = 0;
	m_iSpriteID = 0;
	m_bDead = false;
	m_bSceneChange = false;
}


TSprite::~TSprite()
{
}
