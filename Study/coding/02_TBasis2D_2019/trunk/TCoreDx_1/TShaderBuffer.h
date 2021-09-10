#pragma once
#include "TUtils.h"
#include <d3dcompiler.h>
enum SHADER_TYPE { t_VS, t_GS, t_PS, };
class TShaderBuffer
{
public:
	T_STR m_szName;
	BYTE* m_pByteData;
	LARGE_INTEGER  m_FileSize;
	ID3DBlob*  m_pDataBlob;
	HRESULT    Load(const TCHAR* pLoadFile);
	bool	   Release();
public:
	TShaderBuffer();
	~TShaderBuffer();
};
class TShaderBufferMgr : public TSingleton<TShaderBufferMgr>
{
public:
	std::map< INT, TShaderBuffer*>    m_Map;
	INT							m_iCurIndex;
	INT    Load(const TCHAR* pLoadFile);
	TShaderBuffer*  GetPtr(INT iIndex);
private:
	TShaderBufferMgr();
	virtual ~TShaderBufferMgr();
	friend class TSingleton<TShaderBufferMgr>;
};
#define I_ShaderBuffer TShaderBufferMgr::GetInstance()