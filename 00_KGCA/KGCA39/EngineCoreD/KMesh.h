#pragma once
#include "KModel.h"
#include "KTexture.h"
const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

struct KMtrl
{
	FbxNode* m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	KTexture			m_Texture;
	vector<KMtrl*> m_pSubMtrl;
	KMtrl() {}
	//�����ڷ� ���� ���͸��� ���� 
	KMtrl(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		m_pFbxNode = pFbxNode;
		m_pFbxSurfaceMtrl = pFbxMtrl;
	}
	void Release()
	{
		m_Texture.Release();
		for (auto data : m_pSubMtrl)
		{
			data->Release();
			delete data;
		}
	}
};
//���̾�� �ϳ� �� ���̴�.
struct KLayer
{
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementUV* pUV;
	FbxLayerElementMaterial* pMaterial;
};

struct KWeight
{
	std::vector<int>     m_IndexList;
	std::vector<float>   m_WeightList;
};

struct KSkinData
{
	std::vector<FbxNode*>  m_MatrixList;
	//������ ����ġ, �ε���
	std::vector<KWeight>   m_VertexList;
};
//��ӹ��� PNCT
struct PNCTIW_VERTEX : public PNCT_VERTEX
{
	float   index[4];
	float   weight[4];
};
struct KAnimMatrix
{
	KMatrix   matAnimation[255]; //�ִ� 255
};
class KMesh :public KModel
{
public:
	//�ε��� ���ĵ�
	vector<int>				m_iBoneList;
	KAnimMatrix				m_matAnimMatrix;
	ID3D11Buffer*			m_pAnimCB;
	ID3D11Buffer*			m_pIWVertrexBuffer;
	//������ ����ġ �ε��� 4���� ���̴��� �ֱ� ���� 
	vector<PNCTIW_VERTEX>	m_iwList;
	FbxNode* m_pFbxNode;

	KMesh* m_pParent; // �θ�
	wstring				m_szName;
	wstring				m_szParentName;
	OBJECTCLASSTYPE     m_ClassType;
	vector<KLayer>		m_LayerList;
	int					m_iNumLayer;
	//�ش��ϴ� ����Ž��� ���� ���͸��� �����ؾ���
	int					m_iMtrlRef;
	KMatrix				m_matWorld;
	vector<KMatrix>		m_AnimationTrack;
	vector<KMesh*>		m_pSubMesh;
public:
	virtual HRESULT		CreateVertexLayout() override;
	virtual HRESULT		CreateConstantBuffer()override;
	virtual HRESULT		CreateVertexBuffer()override;
	virtual bool		PreRender(ID3D11DeviceContext* pContext)override;
	bool Release() override
	{
		//�θ� ����
		KModel::Release();
		for (auto data : m_pSubMesh)
		{
			data->Release();
			delete data;
		}
		SAFE_RELEASE(m_pIWVertrexBuffer);
		return true;
	}
	KMesh()
	{
		m_ClassType = CLASS_GEOM;
		m_iMtrlRef = -1;
	}
};

