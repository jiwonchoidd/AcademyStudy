#pragma once
#include "KModel.h"
#include "TStd.h"
class KFbxObj : public KModel
{
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
	// 가상함수 리스트
public:
	bool	CreateModel(std::wstring vsFile, std::wstring psFile) override;
	bool	PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex) override;
	// 일반함수 리스트
public:
	bool	LoadObject(std::string filename);//유니코드를 지원하지 않아서 새로 정의해주었음
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
};

