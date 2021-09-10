#include "Sample.h"
#include <list>
#include <io.h>
std::list<std::wstring>  g_fileList;
void LoadDir(std::wstring path)
{
	std::wstring dirPath = path + L"*.*";
	intptr_t handle; // fopen
	struct _wfinddata_t fd;
	handle = _wfindfirst(dirPath.c_str(), &fd);
	if (handle == -1L)
	{
		return;
	}
	do
	{
		// 디렉토리
		if ((fd.attrib & _A_SUBDIR) && fd.name[0] != '.')
		{
			LoadDir(path + fd.name + L"/");
		}
		else if(fd.name[0] != '.')// 파일, 
		{
			g_fileList.push_back(fd.name);
		}
	} while (_wfindnext(handle, &fd) == 0);
	_findclose(handle);
}
// 유니코드 버전
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstatnce, 
	LPWSTR lpCmdLine,
	int nCmdShow)
{
	Sample sample;
	//   [0] = key
	//   100 = value
	// '.'  : 현재폴터
	// '..' : 상위폴더
	LoadDir(L"../../data/sound/");
	for (std::wstring& data : g_fileList)
	{
		bool ret = sample.m_SoundMgr.Add(data);
		if (!ret)
		{
			OutputDebugString(L"중복\n");
		}
	}
	g_fileList.clear();

	LoadDir(L"../../data/bitmap/");
	for (std::wstring& data : g_fileList)
	{
		bool ret = sample.m_ImageMgr.Add(data);
		if (!ret)
		{
			OutputDebugString(L"중복\n");
		}
	}
	
	TImage* data1 = sample.m_ImageMgr.GetPtr(L"1KGCABK.bmp");
	if (data1 != nullptr)
	{
		OutputDebugString(data1->m_csName.c_str());
	}

	TSound* data2 = sample.m_SoundMgr.GetPtr(L"MyLove.mp3");
	if (data2 != nullptr)
	{
		OutputDebugString(data2->m_csName.c_str());
	}

	// 멀티바이트 사용
	char szBuffer[256] = { 0, };
	strcpy_s(szBuffer, "kgca게임\n");
	int iSize = strlen(szBuffer);
	std::string copyBuffer;
	copyBuffer = szBuffer;
	OutputDebugStringA(copyBuffer.c_str());
	// 유니코드 사용
	WCHAR szwBuffer[256] = { 0, };
	wcscpy_s(szwBuffer, L"KGCA게임\n");
	iSize = wcslen(szwBuffer);
	std::wstring copyWBuffer;
	copyWBuffer = szwBuffer;
	OutputDebugString(copyWBuffer.c_str());
}