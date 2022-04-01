#include "KScene_Intro.h"
#include "KSceneManager.h"
#include "KSkyBox.h"
#include "KState.h"
#include "ImGuiManager.h"
void KScene_Intro::SetupMapObject()
{
	srand(time(NULL));

	m_pBoxObj = std::shared_ptr<KBoxObj>(new KBoxObj);
	m_pBoxObj->Init(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/texture/brick.jpg",
		L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");

	KMatrix matScale;
	KMatrix matRotateObj;
	for (int iObj = 0; iObj < 30; iObj++)
	{
		KMapObject* pObj= new KMapObject;
		for (int iv = 0; iv < 8; iv++)
		{
			pObj->obj_box.List[iv] = m_pBoxObj.get()->m_BoxCollision.List[iv];
		}
		pObj->obj_pos = KVector3(
			randstep(m_Terrian.m_BoxCollision.min.x, m_Terrian.m_BoxCollision.max.x),
			0.0f,
			randstep(m_Terrian.m_BoxCollision.min.z, m_Terrian.m_BoxCollision.max.z));

		D3DKMatrixScaling(&matScale, randstep(1.0f, 4.0f),
			randstep(1.0f, 4.0f),
			randstep(1.5f, 4.0f));
		D3DKMatrixRotationYawPitchRoll(&matRotateObj,
			cosf(randstep(0.0f, 360.0f)) * XM_PI,
			sinf(randstep(0.0f, 360.0f)) * XM_PI,
			1.0f);
		pObj->obj_matWorld = matScale * matRotateObj;
		pObj->obj_pos.y = m_Terrian.GetHeight(pObj->obj_pos.x, pObj->obj_pos.z);
		pObj->obj_matWorld._41 = pObj->obj_pos.x;
		pObj->obj_matWorld._42 = pObj->obj_pos.y;
		pObj->obj_matWorld._43 = pObj->obj_pos.z;
		pObj->UpdateData();
		pObj->UpdateCollision();
		pObj->obj_pObject = static_cast<K3DAsset*>(m_pBoxObj.get());
		m_pObjList.push_back(pObj);
	}
}

bool KScene_Intro::PointInPolygon(KVector3 vert, KVector3 faceNormal,
	KVector3 v0, KVector3 v1, KVector3 v2)
{
	KVector3 e0, e1, e2, iInter, vNormal;
	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	iInter = vert - v0;
	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	iInter = vert - v1;
	D3DXVec3Cross(&vNormal, &e1, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	iInter = vert - v2;
	D3DXVec3Cross(&vNormal, &e2, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;
	return true;
}

//페이스와 레이보다는 레이와 구 레이와 박스 형태로 계싼을 줄임
bool KScene_Intro::GetIntersect(KVector3 vStart, KVector3 vEnd,
	KVector3 v0, KVector3 v1, KVector3 v2,
	KVector3 vNormal)
{
	KVector3 vDirection = vEnd - vStart;
	KVector3 v0toStart = v0 - vStart;
	float A = D3DXVec3Dot(&vNormal, &vDirection);
	float a = D3DXVec3Dot(&vNormal, &v0toStart);
	float t = a / A;
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}
	m_vIntersect = vStart + vDirection * t;
	return true;
}

bool KScene_Intro::IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	KVector3 edge1 = v1 - v0;
	KVector3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	KVector3 pvec;
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);

	KVector3 tvec; // 내적이 양수가 될 수 있도록 det 방향을 뒤집는다.
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u < 0.0f || *u > det)
		return false;

	// Prepare to test V parameter
	KVector3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot(&dir, &qvec);
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;
	return true;
}

bool KScene_Intro::Load(std::wstring file)
{
	return true;
}

bool KScene_Intro::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_INTRO;

	m_SkyBox.Init(context, L"../../data/shader/Skybox.hlsl",L"../../data/texture/Skybox_Miramar.dds");

	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/map/baseColor.jpg",
		L"../../data/map/Ground_Grass_001_ROUGH.jpg", L"../../data/map/Ground_Grass_001_NORM.jpg");
	m_Lod.Build(&m_Terrian, &m_Camera);

	m_Lod.DrawDebugInit(m_pContext);

	m_Box.Init(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/texture/brick.jpg",
		L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");
	m_Box.m_matWorld._42 = 1.0f;

	//카메라 초기화
	m_Camera.m_pVS = g_ShaderManager.CreateVertexShader(L"../../data/shader/VSPS_Frustum.hlsl", "VS");
	m_Camera.m_pPS = g_ShaderManager.CreatePixelShader(L"../../data/shader/VSPS_Frustum.hlsl", "PS");
	m_Camera.Init(m_pContext);
	m_Camera.CreateViewMatrix(KVector3(-2, 3, -8), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, 
		static_cast<float>(g_rtClient.right)/ static_cast<float>(g_rtClient.bottom));
	
	m_TopView.Init(m_pContext);
	m_TopView.CreateViewMatrix(KVector3(0, 3000.0f, -1),KVector3(0, 0, 0));
	m_TopView.CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.45f,
		static_cast<float>(g_rtClient.right) / static_cast<float>(g_rtClient.bottom));

	SetupMapObject();

	for (int iObj = 0; iObj < m_pObjList.size(); iObj++)
	{
		m_Lod.AddObject(m_pObjList[iObj]);
	}
	//Fbx 파일 로드
	m_FbxLoader.Init();
	m_FbxLoader.Load(L"../../data/model/Sphere.fbx");
	for (int iObj = 0; iObj < m_FbxLoader.m_ObjectList.size(); iObj++)
	{
		if (m_FbxLoader.m_ObjectList[iObj]->m_tex_name_diffuse.empty())
		{
			m_FbxLoader.m_ObjectList[iObj]->m_pTexture_Diffuse = g_TextureMananger.Load(L"../../data/texture/brick.jpg");
		}
		else
		{
			m_FbxLoader.m_ObjectList[iObj]->m_pTexture_Diffuse = g_TextureMananger.Load(m_FbxLoader.m_ObjectList[iObj]->m_tex_name_diffuse);
		}
		
		m_FbxLoader.m_ObjectList[iObj]->SetPosition(KVector3(0.0f, 1.0f, 0.0f));
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._11 = 10.0f;
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._22 = 10.0f;
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._33 = 10.0f;
		if (!m_FbxLoader.m_ObjectList[iObj]->CreateObject(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"",
			L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg"))
		{
			return false;
		}
	}
	return true;
}

bool KScene_Intro::Frame()
{
	m_Camera.Frame();
	m_Box.Frame();

#pragma region  오브젝트 이동
	if (g_InputData.bUpKey)
	{
		m_Box.m_pos += m_Box.m_vLook * g_fSecPerFrame * 10.0f;
	}
	if (g_InputData.bDownKey)
	{
		m_Box.m_pos -= m_Box.m_vLook * g_fSecPerFrame * 10.0f;
	}
	if (g_InputData.bRightKey)
	{
		m_Box.m_rot.y += g_fSecPerFrame * 4.0f;
	}
	if (g_InputData.bLeftKey)
	{
		m_Box.m_rot.y -= g_fSecPerFrame * 4.0f;
	}
	m_Box.m_pos.y = m_Terrian.GetHeight(m_Box.m_pos.x, m_Box.m_pos.z) + 1.0f;
	m_Box.SetRotation(m_Box.m_rot);
	m_Box.SetPosition(m_Box.m_pos);
#pragma endregion

	//마우스 피킹
	if (g_InputData.bMouseState[0])
	{
		//화면 좌표계이기때문에, y를 음수
		//화면 크기
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);
		KVector3 vView, vProj;
		//Direction 계산
		//현재 아래 계산으로는 미니맵 클릭시 어려움, 무조건 전체 화면 크기 기준으로
		//제작해서..근데 미니맵에서 클릭할 일이 있을까싶어서
		vProj.x = (((2.0f * ptCursor.x - 2.0f * g_rtClient.left) / g_rtClient.right) - 1);
		vProj.y = -(((2.0f * ptCursor.y - 2.0f * g_rtClient.top) / g_rtClient.bottom) - 1);
		vProj.z = 1.0f;
		vView.x = vProj.x / m_Camera.m_matProj._11;
		vView.y = vProj.y / m_Camera.m_matProj._22;
		vView.z = vProj.z;

		//뷰죄표계에서는 시작은 무조건 0,0,0
		KMatrix matInverse;
		D3DKMatrixInverse(&matInverse, nullptr, &m_Camera.m_matView);

		//카메라의 월드 좌표의 레이가 만든다.
		TRay ray;
		//ray.position = KVector3(0, 0, 0);
		//ray.direction = v;
		//ray.position = ray.position * matInverse;
		//ray.direction= ray.direction * matInverse;
		ray.direction.x = vView.x * matInverse._11 + vView.y * matInverse._21 + vView.z * matInverse._31;
		ray.direction.y = vView.x * matInverse._12 + vView.y * matInverse._22 + vView.z * matInverse._32;
		ray.direction.z = vView.x * matInverse._13 + vView.y * matInverse._23 + vView.z * matInverse._33;

		ray.position.x = matInverse._41;
		ray.position.y = matInverse._42;
		ray.position.z = matInverse._43;
		//정규화
		D3DXVec3Normalize(&ray.direction, &ray.direction);
		KVector3  vStart = ray.position;
		KVector3  vEnd = ray.position + ray.direction * m_Camera.m_fFar;

		for (int iNode = 0; iNode < m_Lod.m_pDrawableLeafList.size(); iNode++)
		{
			KNode* pNode = m_Lod.m_pDrawableLeafList[iNode];

			//for (int i = 0; i < m_Lod.m_LodPatchList[1].IndexList->size(); i += 3)
			//{
			//	KVector3 v0, v1, v2;
			//	DWORD i0 = m_Lod.m_LodPatchList[1].IndexList->at(i + 0);
			//	DWORD i1 = m_Lod.m_LodPatchList[1].IndexList->at(i + 1);
			//	DWORD i2 = m_Lod.m_LodPatchList[1].IndexList->at(i + 2);
			//	v0 = pNode->m_VertexList[i0].pos;
			//	v1 = pNode->m_VertexList[i1].pos;
			//	v2 = pNode->m_VertexList[i2].pos;

			//	//2번 교점
			//	float t, u, v;
			//	if (IntersectTriangle(ray.position, ray.direction,
			//		v0, v1, v2, &t, &u, &v))
			//	{
			//		m_vIntersect = ray.position + ray.direction * t;
			//		pNode->m_VertexList[i0].color = KVector4(1, 0, 0, 1);
			//		pNode->m_VertexList[i1].color = KVector4(1, 0, 0, 1);
			//		pNode->m_VertexList[i2].color = KVector4(1, 0, 0, 1);
			//		m_pContext->UpdateSubresource(
			//			pNode->m_pVertexBuffer.Get(), 0, NULL,
			//			&pNode->m_VertexList.at(0), 0, 0);
			//		//m_vIntersectionList.push_back(m_vIntersection);
			//	}
			for (int i = 0; i < m_Lod.m_IndexList.size(); i += 3)
			{
				KVector3 v0, v1, v2;
				DWORD i0 = m_Lod.m_IndexList[i + 0];
				DWORD i1 = m_Lod.m_IndexList[i + 1];
				DWORD i2 = m_Lod.m_IndexList[i + 2];
				v0 = pNode->m_VertexList[i0].pos;
				v1 = pNode->m_VertexList[i1].pos;
				v2 = pNode->m_VertexList[i2].pos;

				//2번 교점
				float t, u, v;
				if (IntersectTriangle(ray.position, ray.direction,
					v0, v1, v2, &t, &u, &v))
				{
					m_vIntersect = ray.position + ray.direction * t;
					pNode->m_VertexList[i0].color = KVector4(1, 0, 0, 1);
					pNode->m_VertexList[i1].color = KVector4(1, 0, 0, 1);
					pNode->m_VertexList[i2].color = KVector4(1, 0, 0, 1);
					m_pContext->UpdateSubresource(
						pNode->m_pVertexBuffer.Get(), 0, NULL,
						&pNode->m_VertexList.at(0), 0, 0);
				}
			}

		}
	}
		if (ImGui::Begin(u8"디버깅 인스펙터"))
		{
			ImGui::Text(u8"Light Position"); ImGui::SameLine();
			ImGui::InputFloat3("##lightpos", m_LightPos, 1, 0);
			ImGui::Text(u8"Light Color"); ImGui::SameLine();
			ImGui::InputFloat3("##lightcolor", m_LightColor, 1, 1);
		}
		ImGui::End();

		KScene::Frame();
		return true;
}


bool KScene_Intro::Render()
{
	m_SkyBox.m_matSkyView = m_Camera.m_matView;
	m_SkyBox.m_matSkyView._41 = 0;
	m_SkyBox.m_matSkyView._42 = 0;
	m_SkyBox.m_matSkyView._43 = 0;

	m_SkyBox.SetMatrix(&m_SkyBox.m_matWorld, &m_SkyBox.m_matSkyView, &m_Camera.m_matProj);
	m_SkyBox.Render(m_pContext);

	m_Terrian.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Terrian.m_cbData.vLightColor = { m_LightColor[0],m_LightColor[1],m_LightColor[2],1 };
	m_Terrian.m_cbData.vLightPos = { m_LightPos[0],m_LightPos[1],m_LightPos[2]};
	m_Terrian.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x, m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };
	//m_Terrian.Render(m_pContext);
	m_Lod.Render(m_pContext);

	m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Box.m_cbData.vLightColor = { m_LightColor[0],m_LightColor[1],m_LightColor[2],1 };
	m_Box.m_cbData.vLightPos = { m_LightPos[0],m_LightPos[1],m_LightPos[2] };
	m_Box.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f};
	m_Box.Render(m_pContext);

	//FBX Render
	for (int iObj = 0; iObj < m_FbxLoader.m_ObjectList.size(); iObj++)
	{
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vLightColor = { m_LightColor[0],m_LightColor[1],m_LightColor[2],1 }; 
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vLightPos = { m_LightPos[0],m_LightPos[1],m_LightPos[2] };
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };

		m_FbxLoader.m_ObjectList[iObj]->SetMatrix(&m_FbxLoader.m_ObjectList[iObj]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
		m_FbxLoader.m_ObjectList[iObj]->Render(m_pContext);
	}

	//미니맵
	m_Camera.SetMatrix(nullptr, &m_TopView.m_matView,
		&m_TopView.m_matProj);
	m_Camera.Render(m_pContext);

	KScene::Render();
	return true;
}

bool KScene_Intro::Release()
{
	m_FbxLoader.Release();
	m_SkyBox.Release();
	m_Lod.Release();
	m_Terrian.Release();
	m_Camera.Release();
	m_TopView.Release();
	m_Box.Release();
	KScene::Release();
	return true;
}
