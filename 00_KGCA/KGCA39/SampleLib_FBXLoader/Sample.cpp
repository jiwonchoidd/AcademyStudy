#include "Sample.h"
bool		Sample::Init()
{
	m_FbxObj.LoadObject("../../data/object/ship.fbx");
	//m_FbxObj.LoadObject("../../data/object/Circle.fbx");
	return true;
}
bool		Sample::Frame()
{
	return true;
}
bool		Sample::Render()
{
	m_FbxObj.SetMatrix(nullptr, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObj.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release() 
{
	m_FbxObj.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(FBX·Î´õ, 800, 600);