#include "Sample.h"

bool Sample::Init()
{
	//���� �ʱ�ȭ 
	g_SoundManager.Init();
	//�� �Ŵ���
	g_SceneManager.Init(m_pImmediateContext.Get());
    return true;
}
// ������ �Լ�
bool Sample::Frame()
{
	g_SceneManager.Frame();
    return true;
}
bool Sample::Render()
{
	g_SceneManager.Render();
    return true;
}
bool Sample::Release()
{
	g_SceneManager.Release();
    return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{
}

WinMain_OPT(��������, 960, 540);
