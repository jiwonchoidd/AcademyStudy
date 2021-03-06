#include "MapObj.h"

bool MapObj::Load(MapInfo& info)
{
    m_info = info;
    m_info.m_iNumColCell = m_info.m_iNumCol - 1;
    m_info.m_iNumRowCell = m_info.m_iNumRow - 1;
    m_info.m_iNumVertex = m_info.m_iNumColCell * m_info.m_iNumRowCell * 6;
    return true;
}

bool MapObj::CreateVertexData()
{
    // 스스로 여러 삼각형을 만들어 준다.
    m_VertexList.resize(m_info.m_iNumVertex);

    int iIndex = 0;
    for (int iRow = 0; iRow < m_info.m_iNumRowCell; iRow++)
    {
        for (int iCol = 0; iCol < m_info.m_iNumColCell; iCol++)
        {
            m_VertexList[iIndex + 0].pos.x = m_info.m_fCellDistance * iCol;
            m_VertexList[iIndex + 0].pos.y = 0.0f;
            m_VertexList[iIndex + 0].pos.z = -m_info.m_fCellDistance * iRow;

            m_VertexList[iIndex + 1].pos.x =
                m_VertexList[iIndex + 0].pos.x + m_info.m_fCellDistance;
            m_VertexList[iIndex + 1].pos.y = 0.0f;
            m_VertexList[iIndex + 1].pos.z =
                m_VertexList[iIndex + 0].pos.z;

            m_VertexList[iIndex + 2].pos.x =
                m_VertexList[iIndex + 0].pos.x;
            m_VertexList[iIndex + 2].pos.y = 0.0f;
            m_VertexList[iIndex + 2].pos.z =
                m_VertexList[iIndex + 0].pos.z - m_info.m_fCellDistance;

            m_VertexList[iIndex + 3].pos.x =
                m_VertexList[iIndex + 0].pos.x;
            m_VertexList[iIndex + 3].pos.y = 0.0f;
            m_VertexList[iIndex + 3].pos.z =
                m_VertexList[iIndex + 0].pos.z - m_info.m_fCellDistance;
            m_VertexList[iIndex + 4].pos.x =
                m_VertexList[iIndex + 0].pos.x + m_info.m_fCellDistance;
            m_VertexList[iIndex + 4].pos.y = 0.0f;
            m_VertexList[iIndex + 4].pos.z =
                m_VertexList[iIndex + 0].pos.z;
            m_VertexList[iIndex + 5].pos.x =
                m_VertexList[iIndex + 0].pos.x + m_info.m_fCellDistance;
            m_VertexList[iIndex + 5].pos.y = 0.0f;
            m_VertexList[iIndex + 5].pos.z =
                m_VertexList[iIndex + 0].pos.z - m_info.m_fCellDistance;

            m_VertexList[iIndex + 5].color =
                KVector4(
                    0,
                    0,
                    1, 1.0f);
            iIndex += 6;

        }
    }
    if (m_VertexList.size() > 0) return true;
    return false;
}
