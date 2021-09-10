#include "TCamera.h"
#include "TSys.h"

bool TCamera::Init()
{
	m_vEye = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	m_vAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXVec3Normalize( &m_vLook, &(m_vAt - m_vEye));
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	
	m_fRadius = D3DXVec3Length(&(m_vEye - m_vAt));
	m_fFov = (float)D3DX_PI * 0.25f;
	m_fAspect = (g_rtClient.right- g_rtClient.left) / (g_rtClient.bottom- g_rtClient.top);
	m_fNearPlane = 1.0f;
	m_fFarPlane = 100.0f;
	D3DXMatrixPerspectiveFovLH(&m_matProj,
		m_fFov,
		m_fAspect,
		m_fNearPlane,
		m_fFarPlane);
	return true;
}
// degree = radian * ( 180.0f / D3DXPI);
// D3DXToDegree(radian)
// radian = degreen * ( D3DXPI / 180.0f);
// D3DXToRadian(degree)

// 360 = x * ( 360.0f /  800.0f)
void TCamera::Update(D3DXVECTOR4& vDir)
{
	m_fPitch += vDir.y * g_fSecPerFrame * 0.01f;
	m_fYaw += vDir.x * g_fSecPerFrame * 0.01f;
	m_fRoll += vDir.z * g_fSecPerFrame * 0.01f;
	m_fRadius += vDir.w;
	
	TCHAR pbuffer[256] = { 0, };
	_stprintf_s(pbuffer, L"\n%10.4f %10.4f %10.4f %10.4f", 
		m_fPitch, m_fYaw, m_fRoll, m_fRadius);
	OutputDebugString(pbuffer);

	D3DXMATRIX matY, matX, matZ, matRotation;
	D3DXMatrixRotationYawPitchRoll(&matRotation,
		m_fYaw,
		m_fPitch,
		m_fRoll);

	D3DXVECTOR3 vWorldLook;
	D3DXVECTOR3 vWorldUp;
	D3DXVECTOR3 vLocalLook = g_vUnitZ;
	D3DXVECTOR3 vLocalUp = g_vUnitY;

	D3DXVec3TransformCoord(&vWorldLook, &vLocalLook, &matRotation);
	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &matRotation);
	
	m_fRadius = __max(1.0f , m_fRadius);
	m_fRadius = __min(10.0f, m_fRadius);
	m_vEye = m_vAt + (-vWorldLook * m_fRadius);
	m_vUp  = vWorldUp;	
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
}


bool TCamera::Frame()
{
	
	return true;
}
bool TCamera::SetViewMatrix(	D3DXVECTOR3 vEye,
						D3DXVECTOR3 vAt,
						D3DXVECTOR3 vUp)
{
	m_vEye = vEye;
	m_vAt = vAt;
	m_vUp = vUp;
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	return true;

}
bool TCamera::SetProjMatrix(float fFov, float fAspect, float fNear, float fFar)
{
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNearPlane = fNear;
	m_fFarPlane = fFar;
	D3DXMatrixPerspectiveFovLH(&m_matProj,m_fFov,m_fAspect,	m_fNearPlane,m_fFarPlane );
	
	return true;
}
void TCamera::Left()
{
	D3DXVECTOR3 vDir= -g_vUnitX * m_fSpeed * g_fSecPerFrame;
	m_vEye = m_vEye + vDir;
	m_vAt  = m_vAt + vDir;
	return;
};
void TCamera::Right() {
	D3DXVECTOR3 vDir= g_vUnitX * m_fSpeed * g_fSecPerFrame;
	m_vEye = m_vEye + vDir;
	m_vAt = m_vAt + vDir;
	return;
};
void TCamera::Up() {
	D3DXVECTOR3 vDir = g_vUnitY * m_fSpeed * g_fSecPerFrame;
	m_vEye = m_vEye + vDir;
	m_vAt = m_vAt + vDir;
	return;
};
void TCamera::Down() {
	D3DXVECTOR3 vDir = -g_vUnitY * m_fSpeed * g_fSecPerFrame;
	m_vEye = m_vEye + vDir;
	m_vAt = m_vAt + vDir;
	return;
};
void TCamera::Front() {
	D3DXVECTOR3 vDir = g_vUnitZ * m_fSpeed * g_fSecPerFrame;
	m_vEye = m_vEye + vDir;
	m_vAt = m_vAt + vDir;
	return;
};
void TCamera::Back() {
	D3DXVECTOR3 vDir = -g_vUnitZ * m_fSpeed * g_fSecPerFrame;
	m_vEye = m_vEye + vDir;
	m_vAt = m_vAt + vDir;
	return;
};
TCamera::TCamera()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_fSpeed = 5.0f;
	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fWheelSpeed = 0.1f;
}


TCamera::~TCamera()
{
}
