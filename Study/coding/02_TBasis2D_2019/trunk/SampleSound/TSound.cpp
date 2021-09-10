#include "TSound.h"
#include <math.h>
bool		TSound::Init()
{
	m_iSoundList = 0;
	// FMOD 시스템 초기화
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
	return true;
};
int			TSound::Load(char* pLoadName, bool bPlay)
{
	m_pSystem->createSound(pLoadName,
		FMOD_DEFAULT, 0, &m_pSound[m_iSoundList++]);
	if (bPlay == true)
	{
		m_pSystem->playSound(m_pSound[m_iSoundList-1],false, false, &m_pChannel[m_iSoundList-1]);
	}
	return m_iSoundList-1;
}
void		TSound::Stop(int iIndex)
{
	m_pChannel[iIndex]->stop();
}
void		TSound::Paused(int iIndex)
{
	bool paused;
	m_pChannel[iIndex]->getPaused(&paused);
	m_pChannel[iIndex]->setPaused(!paused);
}
// 최대(1.0f) ~ 최소(0.0f)
void		TSound::Volume(int iIndex, float fVolume, bool bUp)
{
	float fCurrentVolume;
	m_pChannel[iIndex]->getVolume(&fCurrentVolume);
	if (bUp == true)
	{
		fCurrentVolume += fVolume;		
		fCurrentVolume = tMin(1.0f, fCurrentVolume);
	}
	else
	{
		fCurrentVolume -= fVolume;
		fCurrentVolume = tMax(0.0f, fCurrentVolume);
	}
	m_pChannel[iIndex]->setVolume(fCurrentVolume);
}
void		TSound::PlayEffect(int iIndex)
{
	Channel*  pChannel;
	m_pSystem->playSound(m_pSound[iIndex], false, false, &pChannel);
	m_pChannel[iIndex] = pChannel;
}
void		TSound::Play(int iIndex, bool bPlay, bool bLoop)
{
	bool playing = false;
	if (bPlay == false)
	{
		if (m_pChannel[iIndex] != 0)
		{
			m_pChannel[iIndex]->isPlaying(&playing);
		}
	}
	if (playing == false)
	{
		if(bLoop)
			m_pSound[iIndex]->setMode(FMOD_LOOP_NORMAL);
		else
			m_pSound[iIndex]->setMode(FMOD_LOOP_OFF);

		m_pSystem->playSound(m_pSound[iIndex], false, false, &m_pChannel[iIndex]);
	}
}
bool		TSound::Frame() 
{
	m_pSystem->update();
	unsigned int ms = 0;
	unsigned int total = 0;
	m_pChannel[0]->getPosition(&ms, FMOD_TIMEUNIT_MS);
	m_pSound[0]->getLength(&total, FMOD_TIMEUNIT_MS);
	_stprintf_s(m_szBuffer, _T(" 경과시간[%02d:%02d:%02d], 파일크기[%02d:%02d:%02d]"),
		ms / 1000 / 60,
		ms / 1000 % 60,
		ms / 10 % 60,
		total / 1000 / 60,
		total / 1000 % 60,
		total / 10 % 60);
	return true;
};
bool		TSound::Render() 
{
	return true;
};
bool		TSound::Release() 
{
	for (int iSound = 0; iSound < g_iMaxSound; iSound++)
	{
		m_pSound[iSound]->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
};


TSound::TSound()
{
}


TSound::~TSound()
{
}
