#include "KFbxObj.h"
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
	FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();
	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		//Global not Parent.
		FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pNode, time);
		KMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
		pMesh->m_AnimationTrack.push_back(matGlobaDX);
		fCurrentTime += m_fSampleTime;
	}
}