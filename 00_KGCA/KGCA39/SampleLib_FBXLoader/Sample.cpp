#include "Sample.h"
bool		Sample::Init()
{
	KVector3 vCamPos = { 0,20,-40 };
	m_DebugCamera.InitCameraSet(vCamPos, vCamPos);
	m_Texture.LoadTexture(L"../../data/texture_grass.jpg");
	m_FbxObj.LoadObject("../../data/box.fbx");
	return true;
}
bool		Sample::Frame() {
	return true;
}
bool		Sample::Render() {
	m_pImmediateContext->PSSetSamplers(0, 1, &m_Texture.m_pSampler);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Texture.m_pTextureSRV);
	m_FbxObj.SetMatrix(nullptr, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObj.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release() {
	m_FbxObj.Release();
	m_Texture.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(FBX·Î´õ, 800, 600);