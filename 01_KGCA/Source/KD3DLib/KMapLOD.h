#pragma once
#include "KQuadTree.h"
#include "KMap.h"
#include "KCamera.h"
struct KLodPatch
{
	UINT   iLodLevel;
	std::vector<DWORD>	IndexList[16];
	std::vector<KNode*> g_DrawLeafNodes;
	wrl::ComPtr<ID3D11Buffer> IndexBufferList[16] = { nullptr, };
	void Release()
	{
		for (int iBuffer = 0; iBuffer < 16; iBuffer++)
		{
			if (IndexBufferList[iBuffer].Get())
			{
				IndexBufferList[iBuffer].Reset();
				IndexBufferList[iBuffer] = nullptr;
			}
		}
	}
	KLodPatch() {}
	~KLodPatch()
	{
	}
};
class KMapLOD : public KQuadTree
{
	KMap*						m_pMap;
	KCamera*					m_pCamera;
public:
	int							m_iNumCell;
	int							m_iPatchLodCount;
	std::vector<KLodPatch>		m_LodPatchList;
	wrl::ComPtr<ID3D11Buffer>	m_pLodIndexBuffer;
	std::vector<DWORD>			m_IndexList;
public:								//오브젝트 리스트
	std::list<KMapObject*>		m_ObjectList;
	std::list<KMapObject*>		m_ObjectList_Dynamic;
public:
	std::vector<KNode*>		m_pDrawableLeafList;//프로스텀에 보이는 리프노드
public:
	void   DrawableUpdate();
	void   RenderTile(KNode* pNode);
	bool   AddObject(KMapObject* obj);
	bool   AddDynamicObject(KMapObject* obj);
public:
	virtual bool	Build(KMap* pmap, KCamera* pCamera);
	virtual bool	SetLOD(KVector3* vCamera);
	virtual bool	ComputeStaticLodIndex(int numcell);
public:
	virtual KNode* CreateNode(KNode* pParent, float x, float y, float w, float h)override;
	virtual bool      UpdateVertexList(KNode* pNode)override;
	virtual HRESULT   CreateVertexBuffer(KNode* pNode)override;
	virtual bool      UpdateIndexList(KNode* pNode);
	virtual HRESULT   CreateIndexBuffer(KLodPatch& patch, int iCode);
	virtual HRESULT   CreateIndexBuffer(KNode* pNode);
public:
	bool LoadLODFile(std::wstring filename);
	template <typename OutputIterator>
	void Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render(ID3D11DeviceContext* pContext);
	virtual bool	Release();
public:
	KMapLOD();
	virtual ~KMapLOD();
};

