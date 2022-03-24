#pragma once
#include "KQuadTree.h"
#include "KMap.h"
struct KLodPatch
{
	UINT   iLodLevel;
	std::vector<DWORD>	IndexList[16];
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
	KMap* m_pMap;
	int						m_iNumCell;
	int						m_iPatchLodCount;
	std::vector<KLodPatch>   m_LodPatchList;
	wrl::ComPtr<ID3D11Buffer> m_pLodIndexBuffer;
	std::vector<DWORD>			 m_IndexList;
public:
	virtual bool	Build(KMap* pmap);
	virtual bool	SetLOD();
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
	virtual bool	Render(ID3D11DeviceContext* pContext, KVector3* vCamera);
	virtual bool	Release();
public:
	KMapLOD();
	virtual ~KMapLOD();
};

