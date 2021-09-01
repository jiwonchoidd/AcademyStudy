#include "TSoundManager.h"
TSound*    TSoundManager::Load(const char* filename)
{
	CHAR drive[MAX_PATH] = { 0, };
	CHAR dir[MAX_PATH] = { 0, };
	CHAR name[MAX_PATH] = { 0, };
	CHAR ext[MAX_PATH] = { 0, };
	_splitpath_s(filename, drive, dir, name, ext);

	std::string Dir = dir;
	std::string key;
	std::string loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	TSound* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}


	pData = new TSound;
	pData->Init();	
	if (pData->Load(m_pSystem, loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	return nullptr;
}
bool   TSoundManager::PlayEffect(const char* filename, bool bLoop)
{
	TSound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->PlayEffect(bLoop);
		return true;
	}
	return false;
}
TSound*    TSoundManager::GetPtr(string filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool		TSoundManager::Init()
{
	FMOD_RESULT hr = FMOD::System_Create(&m_pSystem);
	if (hr != FMOD_OK)
	{
		return false;
	}
	if (m_pSystem != nullptr)
	{
		hr = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
		if (hr != FMOD_OK)
		{
			return false;
		}
	}
	return true;
}
bool		TSoundManager::Frame() 
{
	m_pSystem->update();
	return true;
}
bool		TSoundManager::Render() 
{
	return true;
}
bool		TSoundManager::Release() 
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();

	m_pSystem->close();
	m_pSystem->release();
	return true;
}

TSoundManager::TSoundManager()
{
	m_szDefaultPath = "../../data/sound/";
}

TSoundManager::~TSoundManager()
{

}