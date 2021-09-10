#pragma once
#include "TSound.h"
class TSoundManager : public TSingleton<TSoundManager>
{
public:
	friend  class TSingleton<TSoundManager>;
private:
	FMOD::System*			m_pSystem;
	std::map<string, TSound*>			m_List;
	std::map<string, TSound*>::iterator	m_iter;
	std::string				m_szDefaultPath;
public:
	TSound*		Load(const char* filename);
	bool		PlayEffect(	const char* filename, 
							bool bLoop = false);
	TSound*		GetPtr(string filename);
	
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	TSoundManager();
public:
	~TSoundManager();
};
#define g_SoundMgr TSoundManager::GetInstance()
