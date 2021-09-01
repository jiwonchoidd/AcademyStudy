#include "TSoundMgr.h"

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		// 멀티바이트 => 유니코드 변환.
		TCHAR msg[256] = { 0, };
		const char* srcmsg = FMOD_ErrorString(result);
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars,
			msg, strlen(srcmsg),
			srcmsg,
			((size_t)-1));
		MessageBox(NULL, _T("FMod Error"),
			msg,
			MB_OK);
	}
}

bool TSoundMgr::Init()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_pSystem);	ERRCHECK(result);
	// 버전 확인
	unsigned int version;
	result = m_pSystem->getVersion(&version);	ERRCHECK(result);
	if (version < FMOD_VERSION) {
		return false;
	}
	result = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);	ERRCHECK(result);

	return true;
}
bool TSoundMgr::Frame()
{
	TSound* pSound;
	for (TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++)
	{
		pSound = (TSound*)(*itor).second;
		pSound->Frame();
	}
	m_pSystem->update();
	return true;
}
bool TSoundMgr::Render()
{
	TSound* pSound;
	for (TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++)
	{
		pSound = (TSound*)(*itor).second;
		pSound->Render();
	}
	return true;
};
bool TSoundMgr::Release()
{
	TSound* pSound;
	for (TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++)
	{
		pSound = (TSound*)(*itor).second;
		pSound->Release();
		SAFE_DEL(pSound);
	}

	m_pSystem->close();
	m_pSystem->release();
	return true;
}
int TSoundMgr::Load(const char* pFileName) // 사운드 추가
{
	// 멀티바이트 => 유니코드 변환.
	TCHAR msg[256] = { 0, };
	//const char* srcmsg = FMOD_ErrorString(result);
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars,
		msg, strlen(pFileName) + 1,
		pFileName,
		((size_t)-1));

	FMOD_RESULT result;

	// 중복제거
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };
	if (pFileName)
	{
		_tsplitpath_s(msg, Dirve, Dir, FileName, FileExt);
		_stprintf_s(szFileName, _T("%s%s"), FileName, FileExt);
		TSound* pSound;
		for (TItor itor = m_Map.begin();
			itor != m_Map.end();
			itor++)
		{
			pSound = (TSound*)(*itor).second;
			if (!_tcsicmp(pSound->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	TSound* pSound;
	SAFE_ZERO(pSound);
	SAFE_NEW(pSound, TSound);
	pSound->Init();
	// 사운드 로드
	result = m_pSystem->createSound(pFileName,FMOD_DEFAULT, 0, &pSound->m_pSound);
	pSound->m_szName = szFileName;
	result = pSound->m_pSound->setMode(FMOD_LOOP_OFF);
	//pSound->m_pChannel->setVolume(m_fVolume);
	pSound->m_iIndex = ++m_iCurIndex;
	// std::map에 추가 1
	m_Map.insert(make_pair(m_iCurIndex, pSound));
	return m_iCurIndex;
}
TSound* TSoundMgr::GetPtr(int iIndex)
{
	TItor itor;
	itor = m_Map.find(iIndex);
	if (itor == m_Map.end()) return NULL;
	TSound* pSound = (*itor).second;
	return pSound;
}
void TSoundMgr::LoadDir(T_STR path)
{
	int checkDirFile = 0;
	T_STR dirPath = path + L"\\*.*";
	struct _wfinddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
	intptr_t handle;
	list<_wfinddata_t> fdlist;//디렉토리, 파일 정보 객체 리스트

	if ((handle = _wfindfirst(dirPath.c_str(), &fd)) == -1L) //fd 구조체 초기화.
	{
		//파일이나 디렉토리가 없을 경우.
		return;
	}

	do //폴더 탐색 반복 시작
	{
		checkDirFile = isFileOrDir(fd);//현재 객체 종류 확인(파일 or 디렉토리)
		if (checkDirFile == 0 && fd.name[0] != '.') {
			//디렉토리일 때의 로직
			LoadDir(path + L"\\" + fd.name);//재귀적 호출
		}
		else if (checkDirFile == 1 && fd.name[0] != '.') {
			//파일일 때의 로직
			fdlist.push_back(fd);
		}

	} while (_wfindnext(handle, &fd) == 0);
	_findclose(handle);
}

int TSoundMgr::isFileOrDir(_wfinddata_t fd)//파일인지 디렉토리인지 판별
{

	if (fd.attrib & _A_SUBDIR)
		return 0; // 디렉토리면 0 반환
	else
		return 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

}
TSoundMgr::TSoundMgr()
{
	m_iCurIndex = 0;
}
TSoundMgr::~TSoundMgr()
{
	Release();
}
bool TSound::Release()
{
	if (m_pSound)
	{
		m_pSound->release();
		m_pSound = NULL;
	}

	return true;
}

bool TSound::Init()
{
	return true;
}
void TSound::Play(System*	pSystem)
{
	FMOD_RESULT result;
	bool		playing = false;
	bool		paused = false;
	if (m_pChannel != NULL)
	{
		m_pChannel->isPlaying(&playing);
	}
	// 사운드 실행
	if (!playing)
	{
		result = pSystem->playSound(m_pSound, false, false, &m_pChannel);
		ERRCHECK(result);

	}
}
void TSound::Paused()
{
	bool		playing = false;
	bool		paused = false;
	m_pChannel->isPlaying(&playing);

	// 잠깐 멈춤
	if (playing)
	{
		bool paused;
		m_pChannel->getPaused(&paused);
		m_pChannel->setPaused(!paused);
	}
}
void TSound::Stop()
{
	m_pChannel->stop();
}
void TSound::VolumeUp()
{
	float fVolume = 1.0f;
	m_pChannel->getVolume(&fVolume);
	m_fVolume = fVolume + 0.1f;
	m_pChannel->setVolume(m_fVolume);
}
void TSound::VolumeDown()
{
	float fVolume = 1.0f;
	m_pChannel->getVolume(&fVolume);
	m_fVolume = fVolume - 0.1f;
	m_pChannel->setVolume(m_fVolume);
}
void TSound::SetMode(DWORD dwMode)
{
	FMOD_RESULT result;
	result = m_pSound->setMode(dwMode);
}
bool TSound::Frame()
{
	unsigned int ms = 0;
	unsigned int lenms = 0;
	m_pChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
	Sound*	currentsound = 0;
	m_pChannel->getCurrentSound(&currentsound);
	currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);

	memset(m_strBuffer, 0, sizeof(TCHAR) * MAX_PATH);
	_stprintf_s(m_strBuffer, _T("Time[%02d] :%02d:%02d:%02d lenms :%02d:%02d:%02d"),
		m_iIndex,
		ms / 1000 / 60,
		ms / 1000 % 60,
		ms / 10 % 100,
		lenms / 1000 / 60,
		lenms / 1000 % 60,
		lenms / 10 % 100);
	return true;
}
bool TSound::Render()
{
	HDC hScreenDC = GetDC(g_hWnd);
	HFONT font, oldfont;
	font = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"궁서");
	oldfont = (HFONT)SelectObject(hScreenDC, font);
	TextOut(hScreenDC, 0, m_iIndex * 15, m_strBuffer, lstrlen(m_strBuffer));
	SelectObject(hScreenDC, oldfont);
	DeleteObject(font);
	DeleteDC(hScreenDC);
	return true;
}


TSound::TSound(void)
{
	m_fVolume = 0.5f;
	m_pSound = NULL;
	SAFE_ZERO(m_pChannel);
}

TSound::~TSound(void)
{
}
