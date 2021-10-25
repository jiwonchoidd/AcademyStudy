#include "KFbxObj.h"
//값에 맞는 인덱스로 바꿔준다.
int  KFbxObj::GetFindIndex(FbxNode* pNode)
{
	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
	{
		if (m_pFbxNodeList[iNode] == pNode)
		{
			return iNode;
		}
	}
	return -1;
}

bool KFbxObj::ParseMeshSkinning(FbxMesh* pFbxMesh, KMesh* pMesh, KSkinData* pSkindata)
{
	int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iNumDeformer == 0)
	{
		return false;
	}
	int iNumVertexCount = pFbxMesh->GetControlPointsCount();
	pSkindata->m_VertexList.resize(iNumVertexCount);

	for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
	{
		//디포머에 스킨인 것들만 받아온다.
		FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		//디포머를 스킨으로 캐스팅
		FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
		//특정 정점에 영향을 미치는 개수
		int iNumCluster = pSkin->GetClusterCount();
		// 영향을 미치는 행렬이 iNumCluster 있다.
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			//정점들이 원을 그리는데 일부의 정점만 포함되는 경우가 있을때
			//클러스터 특정 정점에 영향을 미치는 행렬이 있는 구조체
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);

			FbxAMatrix matXBindPose;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			FbxAMatrix matInitPostion;
			pCluster->GetTransformMatrix(matInitPostion);
			FbxAMatrix matBoneBindPos = matInitPostion.Inverse() *
				matXBindPose;
			KMatrix matBinePos = DxConvertMatrix(ConvertAMatrix(matBoneBindPos));
			// 영향을 미치는 행렬이 iClusterSize 정점에 영향을 미친다.
			int iNumVertex = pCluster->GetControlPointIndicesCount();

			FbxNode* pLinkNode = pCluster->GetLink();
			pSkindata->m_MatrixList.push_back(pLinkNode);
			int iBone = GetFindIndex(pLinkNode);
			_ASSERT(iBone >= 0);
			pMesh->m_iBoneList.push_back(iBone);
			D3DKMatrixInverse(&matBinePos, NULL, &matBinePos);
			m_matBindPoseList[iBone] = matBinePos;

			int iMatrixIndex = pSkindata->m_MatrixList.size() - 1;
			//ControlPoint(제어점) 정점리스트
			int* iIndex = pCluster->GetControlPointIndices();
			// 가중치리스트
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iMatrixIndex);
				pSkindata->m_VertexList[iIndex[i]].m_WeightList.push_back(pWeight[i]);
				//iIndex[i] 정점은  iMatrixIndex행렬이 pWeight[i]=1 가중치로 영향을 미친다.				
			}
		}
	}
	return true;
}
void    KFbxObj::ParseAnimStack(FbxString* szData)
{
	FbxTakeInfo* pTakeInfo = m_pFbxScene->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0,
		m_pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fFrameStep = 1;
	m_fSampleTime = fFrameTime * fFrameStep;
	//읽어진 info가 있을 경우에 로컬 타임을 넣음
	if (pTakeInfo)
	{
		m_fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		m_fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
}
// Parse Animation Total Frame, Begin, End
void	KFbxObj::ParseAnimation()
{
	FbxArray<FbxString*> AnimStackNameArray;
	m_pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int iStack = 0; iStack < iAnimStackCount; iStack++)
	{
		ParseAnimStack(AnimStackNameArray.GetAt(iStack));
	}
}
// Get Animation Data All Mesh Parse except Camera, Light 
void	KFbxObj::ParseAnimationNode(FbxNode* pNode,
	KMesh* pMesh)
{
	//애니메이션 데이터 저장
	FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();
	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
		{
			KMesh* pMesh = m_pMeshList[iMesh];
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
			KMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDX);
		}
		fCurrentTime += m_fSampleTime;
	}
}
/*for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
{
	float fCurrentTime = m_fStartTime;
	TMesh* pMesh = m_pMeshList[iMesh];
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
		TMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
		pMesh->m_AnimationTrack.push_back(matGlobaDX);
		fCurrentTime += m_fSampleTime;
	}
}*/