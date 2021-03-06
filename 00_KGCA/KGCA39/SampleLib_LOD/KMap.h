#pragma once
#include "KModel.h"
struct KMapInfo
{
    int m_iNumRow; // 2^n+1
    int m_iNumCol;
    int m_iNumRowCell;
    int m_iNumColCell;
    int m_iNumVertex;
    float m_fCellDistance;
};
class KMap :public KModel
{
public: 
    KMapInfo m_info;
public:
    bool    Load(KMapInfo& info);
	bool	CreateVertexData() override;
    bool	CreateIndexData() override;
   
};

