#include "KFBXAsset.h"

bool KFBXAsset::Init()
{
	return false;
}

bool KFBXAsset::Frame()
{
	return false;
}

bool KFBXAsset::Render(ID3D11DeviceContext* pContext)
{
	//���࿡ �ִԷδ��� ������� ��쿡�� �Ϲ� �δ��� ��ü����
	if (m_pAnimLoader == nullptr)
	{
		m_pAnimLoader = m_pLoader;
	}

	m_fAnimTime += g_fSecPerFrame * m_pAnimLoader->m_Scene.iFrameSpeed * m_fAnimDir* m_fAnimSpeed;
	if (m_fAnimTime >= m_pAnimLoader->m_Scene.iEnd)
	{
		m_fAnimTime = m_pAnimLoader->m_Scene.iStart;
	}
	int iFrame = m_fAnimTime;
	iFrame = max(0, min(m_pAnimLoader->m_Scene.iEnd-1, iFrame));

	for (int iObj = 0; iObj < m_pLoader->m_MeshList.size(); iObj++)
	{
		KFBXObj* pFbxObj = m_pLoader->m_MeshList[iObj];

		//���࿡ ��Ű�� ĳ���� ������Ʈ���
		//Ʈ���� Ÿ�� �� ��ǥ��� ����������
		if (pFbxObj->m_bSkinned)
		{
			for (int inode = 0; inode < m_pLoader->m_FBXTreeList.size(); inode++)
			{
				std::wstring name = m_pLoader->m_FBXTreeList[iObj]->m_ObjName;
				KFBXObj* pFbxtree = m_pLoader->m_FBXTreeList[iObj];
				KFBXObj* pAnimTrack = nullptr;

				auto model = m_pAnimLoader->m_pFbxObjMap.find(name);
				if (model != m_pAnimLoader->m_pFbxObjMap.end())
				{
					pAnimTrack = model->second;

					//�� ������ �˻��ϴ°� ���� �ʾƼ� ���� ���.
					auto binepose = pFbxObj->m_MatrixBindPoseMap.find(name);
					if (binepose != pFbxObj->m_MatrixBindPoseMap.end() && pAnimTrack)
					{
						KMatrix matInverseBindpose = binepose->second;
						m_matBoneArray.matBoneWorld[inode] =
							matInverseBindpose *
							pAnimTrack->m_AnimTrack[iFrame].matTrack;
					}
				}
				D3DKMatrixTranspose(&m_matBoneArray.matBoneWorld[inode],
					&m_matBoneArray.matBoneWorld[inode]);
			}
		}
		else
		{
			for (int inode = 0; inode < m_pLoader->m_FBXTreeList.size(); inode++)
			{
				KFBXObj* pFBXTree = m_pLoader->m_FBXTreeList[inode];
				if (pFBXTree->m_AnimTrack.size() > 0)
				{
					m_matBoneArray.matBoneWorld[inode] =
						pFBXTree->m_AnimTrack[iFrame].matTrack;

				}
				D3DKMatrixTranspose(&m_matBoneArray.matBoneWorld[inode],
					&m_matBoneArray.matBoneWorld[inode]);
			}
		}

		pContext->UpdateSubresource(m_pLoader->m_pBoneCB, 0, NULL, &m_matBoneArray, 0, 0);
		pContext->VSSetConstantBuffers(1, 1, &m_pLoader->m_pBoneCB);
		pFbxObj->m_cbData.vLightColor = { m_cbData.vLightColor.x,m_cbData.vLightColor.y,m_cbData.vLightColor.z,1.0f };
		pFbxObj->m_cbData.vLightPos = { m_cbData.vLightPos.x,m_cbData.vLightPos.y,m_cbData.vLightPos.z };
		pFbxObj->m_cbData.vCamPos = { m_cbData.vCamPos.x, m_cbData.vCamPos.y, m_cbData.vCamPos.z, 1.0f };

		pFbxObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		pFbxObj->Render(pContext);
	}
	return true;
}

bool KFBXAsset::Release()
{
	return false;
}

//�ִϸ��̼� ����ÿ� ����, Ȥ�� ���ο���(����������)
KMatrix KFBXAsset::Interpolation(KFbxLoader* pAnimLoader, KFBXObj* pFbxObj, float fTime)
{
	KMatrix  matAnim;
	KScene_Animation scene_Anim = pAnimLoader->m_Scene;
	int iStart = max(scene_Anim.iStart, fTime);
	int iEnd = min(scene_Anim.iEnd, fTime + 1);
	// ���� = A ~ 7.5f ~ B
	//       9.5f <=10   ~     20 -> 20.1
	KTrack A = pFbxObj->m_AnimTrack[iStart];
	KTrack B = pFbxObj->m_AnimTrack[iEnd];
	float s = fTime - (float)iStart; // 0~1
	KVector3 pos;
	//��ġ �� ���� ���� 
	D3DXVec3Lerp(&pos, &A.t, &B.t, s);
	KVector3 scale;
	//������ �� ���� ����
	D3DXVec3Lerp(&scale, &A.s, &B.s, s);
	KQuaternion rotation;
	//ȸ�� �� ���� ����
	D3DXQuaternionSlerp(&rotation, &A.r, &B.r, s);
	KMatrix matScale;
	D3DKMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	KMatrix matRotation;
	D3DKMatrixRotationQuaternion(&matRotation, &rotation);

	matAnim = matScale * matRotation;
	matAnim._41 = pos.x;
	matAnim._42 = pos.y;
	matAnim._43 = pos.z;
	return matAnim;
}
