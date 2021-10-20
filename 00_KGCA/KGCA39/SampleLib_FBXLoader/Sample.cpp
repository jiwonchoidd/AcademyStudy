#include "Sample.h"
bool		Sample::Init()
{
	//m_FbxObj.LoadObject("../../data/object/ship.fbx");
	//m_FbxObj.LoadObject("../../data/object/Circle.fbx");
	//m_FbxObjA.LoadObject("../../data/object/multiCamera.fbx");
	m_FbxObjA.LoadObject("../../data/object/multiCamera.fbx");
	//m_FbxObjB.LoadObject("../../data/object/Turret.fbx");
	m_DebugCamera.CreateViewMatrix(KVector3(0, 20, -50), KVector3(0, 0, 0));
	m_DebugCamera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}
bool		Sample::Frame()
{
	return true;
}
bool		Sample::Render()
{
	m_FbxObjA.SetMatrix(nullptr, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjA.Render(m_pImmediateContext);
	//m_FbxObjB.SetMatrix(nullptr, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	//m_FbxObjB.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release() 
{
	m_FbxObjA.Release();
	//m_FbxObjB.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(FBX·Î´õ, 800, 600);