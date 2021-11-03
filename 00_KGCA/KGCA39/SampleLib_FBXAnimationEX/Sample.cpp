#include "Sample.h"
bool		Sample::Init()
{
	m_FbxObjB.LoadObject("../../data/object/multiCamera.FBX", "CharacterShader.hlsl");
	m_FbxObjA.LoadObject("../../data/object/Man.fbx", "CharacterShader.hlsl");
	m_DebugCamera.CreateViewMatrix(KVector3(0, 30, -100), KVector3(0, 0, 0));
	m_DebugCamera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 2.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}
bool		Sample::Frame()
{
	//전체 애니메이션
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		(m_FbxObjA.m_bAnimPlay) ? m_FbxObjA.m_bAnimPlay = false : m_FbxObjA.m_bAnimPlay = true;
	}
	m_FbxObjA.Frame();
	return true;
}
bool		Sample::Render()
{
	m_FbxObjB.SetMatrix(nullptr, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjB.Render(m_pImmediateContext);
	m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjA.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release()
{
	m_FbxObjB.Release();
	m_FbxObjA.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(FBX애니메이션, 1200, 675);