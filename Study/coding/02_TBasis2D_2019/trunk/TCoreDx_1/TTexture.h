#pragma once
#include "TUtils.h"

class TTexture
{
public:
	T_STR					m_szName;	
	ID3D11ShaderResourceView* m_pSRV;
public:
	HRESULT    Create(	ID3D11Device* pDevice,
		const TCHAR* pLoadFile);
	void	   Apply(ID3D11DeviceContext* pContext);
	bool	   Release();
public:
	TTexture();
	virtual ~TTexture();
};

class TTextureMgr : public TSingleton<TTextureMgr>
{
public:
	std::map< INT, TTexture*>    m_Map;
	INT							m_iCurIndex;
	INT    Load(ID3D11Device* pDevice, const TCHAR* pLoadFile);
	TTexture*  GetPtr(INT iIndex);
private:
	friend class TSingleton<TTextureMgr>;
	TTextureMgr();
	virtual ~TTextureMgr();
};
#define I_Texture TTextureMgr::GetInstance()