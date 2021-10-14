#pragma once
#include "KModel.h"
#include "TStd.h"
class KFbxObj : public KModel
{
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
	// �����Լ� ����Ʈ
public:
	bool	CreateModel(std::wstring vsFile, std::wstring psFile) override;
	bool	PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex) override;
	// �Ϲ��Լ� ����Ʈ
public:
	bool	LoadObject(std::string filename);//�����ڵ带 �������� �ʾƼ� ���� �������־���
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
};

