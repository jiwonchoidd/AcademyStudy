#pragma once
#include "ModelObj.h"
struct MapInfo
{
    int m_iNumRow; 
    int m_iNumCol;
    int m_iNumRowCell;
    int m_iNumColCell;
    int m_iNumVertex;
    float m_fCellDistance;
};
class MapObj : public ModelObj
{
public:
    MapInfo m_info;
public:
    bool    Load(MapInfo& info);
    bool	CreateVertexData() override;
    bool	CreateIndexData() override;
};

