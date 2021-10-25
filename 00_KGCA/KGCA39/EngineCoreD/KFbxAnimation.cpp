#include "KFbxObj.h"
//���� �´� �ε����� �ٲ��ش�.
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
		//�����ӿ� ��Ų�� �͵鸸 �޾ƿ´�.
		FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		//�����Ӹ� ��Ų���� ĳ����
		FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
		//Ư�� ������ ������ ��ġ�� ����
		int iNumCluster = pSkin->GetClusterCount();
		// ������ ��ġ�� ����� iNumCluster �ִ�.
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			//�������� ���� �׸��µ� �Ϻ��� ������ ���ԵǴ� ��찡 ������
			//Ŭ������ Ư�� ������ ������ ��ġ�� ����� �ִ� ����ü
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);

			FbxAMatrix matXBindPose;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			FbxAMatrix matInitPostion;
			pCluster->GetTransformMatrix(matInitPostion);
			FbxAMatrix matBoneBindPos = matInitPostion.Inverse() *
				matXBindPose;
			KMatrix matBinePos = DxConvertMatrix(ConvertAMatrix(matBoneBindPos));
			// ������ ��ġ�� ����� iClusterSize ������ ������ ��ģ��.
			int iNumVertex = pCluster->GetControlPointIndicesCount();

			FbxNode* pLinkNode = pCluster->GetLink();
			pSkindata->m_MatrixList.push_back(pLinkNode);
			int iBone = GetFindIndex(pLinkNode);
			_ASSERT(iBone >= 0);
			pMesh->m_iBoneList.push_back(iBone);
			D3DKMatrixInverse(&matBinePos, NULL, &matBinePos);
			m_matBindPoseList[iBone] = matBinePos;

			int iMatrixIndex = pSkindata->m_MatrixList.size() - 1;
			//ControlPoint(������) ��������Ʈ
			int* iIndex = pCluster->GetControlPointIndices();
			// ����ġ����Ʈ
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iMatrixIndex);
				pSkindata->m_VertexList[iIndex[i]].m_WeightList.push_back(pWeight[i]);
				//iIndex[i] ������  iMatrixIndex����� pWeight[i]=1 ����ġ�� ������ ��ģ��.				
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
	//�о��� info�� ���� ��쿡 ���� Ÿ���� ����
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
	//�ִϸ��̼� ������ ����
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