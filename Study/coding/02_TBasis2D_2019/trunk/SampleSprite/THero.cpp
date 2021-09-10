#include "THero.h"
#include "TInput.h"

void           THero::Movement(TSpriteRect&   key, 	float & pos)
{
	DWORD dwLeft = I_Input.KeyCheck(key.m_KeyValue[0]);
	DWORD dwA = I_Input.KeyCheck(key.m_KeyValue[1]);
	if (dwLeft == KEY_HOLD || dwA == KEY_HOLD)
	{
		// 시간의 동기화
		pos += key.fDirection * m_fSpeed * g_fSecPerFrame;
		if (key.bStartAnim == false)
		{
			key.fStartTime = g_fAccumulation;
		}
		key.bStartAnim = true;
	}
	else
	{
		key.bStartAnim = false;
	}
	if (dwLeft == KEY_UP || dwA == KEY_UP)
	{
		key.fStartTime = g_fAccumulation;
		key.bEndAnim = true;
	}

#pragma region bEndAnim
	if (key.bEndAnim)
	{
		key.iFrame = (key.rtSprite.size() - 1) -
			(g_fAccumulation - key.fStartTime) * key.fSpriteSpeed;
		if (key.iFrame < 0)
		{
			m_rtDraw = m_rtSrcPos;
			key.iFrame = 0;
			key.bEndAnim = false;
		}
		else
		{
			m_rtDraw = key.rtSprite[key.iFrame];
		}
	}
#pragma endregion
#pragma region bStartAnim
	if (key.bStartAnim &&
		key.iFrame < key.rtSprite.size() - 1)
	{
		key.iFrame = (g_fAccumulation - key.fStartTime) * key.fSpriteSpeed;
		if (key.iFrame >= key.rtSprite.size())
		{
			key.bStartAnim = false;
		}
		else
		{
			m_rtDraw = key.rtSprite[key.iFrame];
		}
	}
#pragma endregion
}
bool			THero::Update()
{
	Movement(m_LeftState, m_ObjPos.x);
	Movement(m_RightState, m_ObjPos.x);

	if (I_Input.KeyCheck(VK_UP) ||
		I_Input.KeyCheck('W'))
	{
		m_ObjPos.y -= m_fSpeed * g_fSecPerFrame;
	}
	if (I_Input.KeyCheck(VK_DOWN) ||
		I_Input.KeyCheck('S'))
	{
		m_ObjPos.y += m_fSpeed * g_fSecPerFrame;
	}


	SetPos(m_ObjPos);
	return true;
}

THero::THero()
{
}


THero::~THero()
{
}
