#pragma once
#include "TShaderBuffer.h"
class TShader
{
public:
	T_STR					m_szName;
	ID3D11VertexShader*    m_pVS;
	ID3D11PixelShader*     m_pPS;
	ID3D11GeometryShader*  m_pGS;
	ID3DBlob*			   m_pBlob;
	TShaderBuffer*		   m_pShaderBuffer;
public:
	HRESULT    Create(ID3D11Device* pDevice,
		const TCHAR* pLoadFile,
		CHAR* pEntry, CHAR* pCompiler,
		SHADER_TYPE type);
	HRESULT    Create(ID3D11Device* pDevice,
		TShaderBuffer* pBuffer,
		CHAR* pEntry, CHAR* pCompiler,
		SHADER_TYPE type);
	void	   ApplyVS(ID3D11DeviceContext* pContext);
	void	   ApplyPS(ID3D11DeviceContext* pContext);
	void	   ApplyGS(ID3D11DeviceContext* pContext);
	bool	   Release();
public:
	TShader();
	virtual ~TShader();
};

class TShaderMgr : public TSingleton<TShaderMgr>
{
public:
	std::map< INT, TShader*>    m_Map;
	INT							m_iCurIndex;
	INT    Load(ID3D11Device* pDevice,
		const TCHAR* pLoadFile,
		CHAR* pEntry, CHAR* pCompiler,
		SHADER_TYPE type);
	TShader*  GetPtr(INT iIndex);
private:
	friend class TSingleton<TShaderMgr>;
	TShaderMgr();
	virtual ~TShaderMgr();
};
#define I_Shader TShaderMgr::GetInstance()