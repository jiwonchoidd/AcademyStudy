#pragma once
#include <tchar.h>
#include "fmod.hpp"
#include "fmod_errors.h"
#if defined(_WIN64)
#pragma comment (lib, "fmod64_vc.lib")
#else
#pragma comment (lib, "fmod_vc.lib")
#endif
using namespace FMOD;

const int g_iMaxSound = 3;
class TSound
{
public:
	FMOD::System*	m_pSystem;
	FMOD::Sound*	m_pSound[g_iMaxSound];
	FMOD::Channel*  m_pChannel[g_iMaxSound];
	TCHAR			m_szBuffer[260];
	int				m_iSoundList;
public:
	int			Load(char* pLoadName, bool bPlay=false);
	void		Play(int iIndex, bool bPlay=true);
	void		Stop(int iIndex);
	void		Paused(int iIndex);
	void		Volume(int iIndex, float fVolume, bool bUp=true);
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	TSound();
	virtual ~TSound();
};

