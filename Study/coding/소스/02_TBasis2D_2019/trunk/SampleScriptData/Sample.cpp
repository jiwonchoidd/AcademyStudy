#include "Sample.h"

bool Sample::LoadScript()
{
	int iKeyNumber1 = I_ScriptManager.Load(L"../../data/script/Number.txt");
	int iKeyNumber2 = I_ScriptManager.Load(L"../../data/script/sprite.txt");
	int iKeyNumber3 = I_ScriptManager.Load(L"../../data/script/ui.txt");

	TSpriteData* pData = I_ScriptManager.GetPtr(iKeyNumber1);
	for (int iSprite = 0; iSprite < pData->m_SpliteListData.size();
		iSprite++)
	{
		TObject s0;
		s0.Init();
		s0.Set(pData->m_SpliteListData[iSprite], pData->m_iBitmapID, pData->m_iMaskBitmapID);
		m_TimerObj.push_back(s0);
	}
	pData = I_ScriptManager.GetPtr(iKeyNumber2);
	for (int iSprite = 0; iSprite < pData->m_SpliteListData.size();
		iSprite++)
	{
		TObject s0;
		s0.Init();
		s0.Set(pData->m_SpliteListData[iSprite], pData->m_iBitmapID, pData->m_iMaskBitmapID);
		m_EffectObj.push_back(s0);
	}
	pData = I_ScriptManager.GetPtr(iKeyNumber3);
	for (int iSprite = 0; iSprite < pData->m_SpliteListData.size();
		iSprite++)
	{
		TObject s0;
		s0.Init();
		s0.Set(pData->m_SpliteListData[iSprite], pData->m_iBitmapID, pData->m_iMaskBitmapID);
		m_UIObj.push_back(s0);
	}
	return true;
}
bool Sample::Init()
{
	LoadScript();
	return true;
}

bool Sample::Render()
{
	if (m_UIObj.size() > 0)
	{
		m_UIObj[0].m_pImage->Render();// .Draw(SRCCOPY); // ¹è°æ
	}
	static float fAlpha = 0.0f;
	fAlpha += g_fSecPerFrame * 100;
	if (fAlpha > 255) fAlpha = 0.0f;

	if (m_UIObj.size() <= 0) return true;
	for (int iSprite = 1; iSprite < m_UIObj.size(); iSprite++)
	{
		m_UIObj[iSprite].Render();
	}

	for (int iSprite = 0; iSprite < m_EffectObj.size(); iSprite++)
	{
		m_EffectObj[iSprite].Render();
	}
	m_TimerObj[m_iTimerSprite].Draw(300, 0, SRCAND);
	m_TimerObj[m_iTimerSprite].Draw(350, 0, SRCAND);
	m_TimerObj[m_iTimerSprite].Draw(400, 0, SRCAND);

	for (int i = 0; i < m_SpliteListClick.size(); i++)
	{
		m_SpliteListClick[i].Render();
	}

	for (std::vector<TObject>::iterator iter =
		m_SpliteListClick.begin();
		iter != m_SpliteListClick.end();)
	{
		if ((*iter).m_bDead)
		{
			iter = m_SpliteListClick.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool Sample::Release()
{
	for (int i = 0; i < m_SpliteListClick.size(); i++)
	{
		m_SpliteListClick[i].Release();
	}
	for (auto& actor : m_TimerObj)
	{
		actor.Release();
	}
	for (auto& actor : m_EffectObj)
	{
		actor.Release();
	}
	for (auto& actor : m_UIObj)
	{
		actor.Release();
	}
	return true;
}
bool Sample::Frame()
{
	static float fTimer = 0.0f;
	fTimer += g_fSecPerFrame;
	if (fTimer >= 1.0f)
	{
		m_iTimerSprite++;
		if (m_iTimerSprite > 9)
		{
			m_iTimerSprite = 0;
		}
		fTimer = 0.0f;
	}
	for (int iSprite = 0; iSprite < m_EffectObj.size(); iSprite++)
	{
		m_EffectObj[iSprite].Frame();
	}
	for (int iSprite = 0; iSprite < m_UIObj.size(); iSprite++)
	{
		m_UIObj[iSprite].Frame();
	}
	//m_TimerObj[0].m_Pos = I_Input.m_MousePos;
	if (g_ActionInput.bRightClick == KEY_PUSH)
	{
		int iRand = rand() % 3;
		TObject s0 = m_EffectObj[iRand];
		s0.m_info.bLoop = false;
		s0.m_iCurrentFrame = 0;
		s0.m_bDead = false;
		s0.m_info.fLifeTime = rand() % 3;
		s0.SetPos(I_Input.m_MousePos);
		s0.m_info.rtList = m_EffectObj[iRand].m_info.rtList;
		m_SpliteListClick.push_back(s0);
	}
	for (auto& actor : m_SpliteListClick)
	{
		actor.Frame();
	}
	return true;
}
Sample::Sample()
{
	m_iTimerSprite = 0;
}


Sample::~Sample()
{
}


TCORE_RUN(UISetRender, 800, 600);