#include "Sample.h"
bool		Sample::Init()
{
	m_FbxObjA.LoadObject("../../data/object/Plus.fbx");
	m_FbxObjB.LoadObject("../../data/object/BoxAnim.fbx");
	m_DebugCamera.CreateViewMatrix(KVector3(0, 15, -20), KVector3(0, 0, 0));
	m_DebugCamera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}
bool		Sample::Frame()
{
	if (g_Input.GetKey(VK_F3) == KEY_PUSH)
	{
		(m_FbxObjB.m_bAnimPlay) ? m_FbxObjB.m_bAnimPlay = false 
			: m_FbxObjB.m_bAnimPlay = true;
	}
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		(m_FbxObjA.m_bAnimPlay) ? m_FbxObjA.m_bAnimPlay = false
			: m_FbxObjA.m_bAnimPlay = true;
	}
	m_FbxObjA.Frame();
	m_FbxObjB.Frame();
	return true;
}
bool		Sample::Render()
{
	m_FbxObjA.SetMatrix(nullptr, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjA.Render(m_pImmediateContext);
	m_FbxObjB.SetMatrix(nullptr, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjB.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release() 
{
	m_FbxObjA.Release();
	m_FbxObjB.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(FBX애니메이션, 800, 600);