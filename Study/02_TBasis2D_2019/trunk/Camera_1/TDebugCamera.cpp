#include "TDebugCamera.h"
#include "TSys.h"

void TDebugCamera::Left()
{
	m_vEye -= m_vRight * m_fSpeed * g_fSecPerFrame;	
	return;
};
void TDebugCamera::Right() {
	m_vEye += m_vRight * m_fSpeed * g_fSecPerFrame;
	return;
};
void TDebugCamera::Up() {
	m_vEye += m_vUp * m_fSpeed * g_fSecPerFrame;
	return;
};
void TDebugCamera::Down() {
	m_vEye -= m_vUp * m_fSpeed * g_fSecPerFrame;
	return;
};
void TDebugCamera::Front() {
	m_vEye += m_vLook * m_fSpeed * g_fSecPerFrame;
	return;
};
void TDebugCamera::Back() {
	m_vEye -= m_vLook * m_fSpeed * g_fSecPerFrame;
	return;
};
void TDebugCamera::Update(D3DXVECTOR4& vDir)
{	
	m_fPitch += vDir.y * g_fSecPerFrame * 0.1f;
	m_fYaw += vDir.x * g_fSecPerFrame * 0.1;
	m_fRoll += vDir.z * g_fSecPerFrame * 0.1f;
	m_vEye += m_vLook * vDir.w * m_fSpeed * m_fWheelSpeed;

	D3DXMATRIX matY, matX, matZ, matRotation;
	D3DXQUATERNION qRatation;// 사원수(회전)	
	D3DXQuaternionRotationYawPitchRoll(&qRatation, m_fYaw,m_fPitch,m_fRoll);
	D3DXMatrixAffineTransformation(&matRotation, 
		1.0f, 
		NULL,
		&qRatation, 
		&m_vEye );
	D3DXMatrixInverse( &m_matView, NULL,  &matRotation);

	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;
}
TDebugCamera::TDebugCamera()
{
}


TDebugCamera::~TDebugCamera()
{
}
