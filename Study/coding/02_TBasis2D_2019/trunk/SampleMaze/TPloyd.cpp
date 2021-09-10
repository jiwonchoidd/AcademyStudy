#include "TPloyd.h"

bool TPloyd::MakePloydPass(MazeBoxInfo* pMazeBoxes, int iScreenWidth, int iScreenHeight)
{
	m_dwNumNodes = m_iWidth * m_iHeight;
	// ��ü ������ ������ �÷��̵� ����
	SAFE_NEW_ARRAY(m_pPloydVertex, TPloydVertex, m_dwNumNodes);
	// ��ü �÷��̵������� 2���� �迭
	SAFE_NEW_ARRAY(m_pPloydVertexArray, WORD, m_dwNumNodes *  m_dwNumNodes);
	memset(m_pPloydVertexArray, 0, sizeof(WORD) * m_dwNumNodes * m_dwNumNodes);
	// �÷��̵� ������ 2�����迭�� ����Ͽ� �������� ��ã�� ������ �����Ѵ�.
	SAFE_NEW_ARRAY(m_pPloydPathList, TPloydPath, m_dwNumNodes*m_dwNumNodes);

	int iOffSetWidth = iScreenWidth / m_iWidth;
	int iOffSetHeight = iScreenHeight / m_iHeight;

	int iPloydVertexCnt = 0;
	float x0, x1, y0, y1;
	for (int y = 0; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			// �� �÷��̵� ������ ��ġ�� �����Ѵ�.
			x0 = (float)(x * iOffSetWidth);
			y0 = (float)(y * iOffSetHeight);
			x1 = (float)((x + 1) * iOffSetWidth);
			y1 = (float)((y + 1) * iOffSetHeight);
			m_pPloydVertex[iPloydVertexCnt].m_vVertex.x = (x1 + x0) / 2.0f;
			m_pPloydVertex[iPloydVertexCnt++].m_vVertex.y = (y1 + y0) / 2.0f;

			// 4 ������ �̵� ���� ���θ� �����Ѵ�.
			int iIndex = y * m_iWidth + x;
			// iIndex�� �ش��ϴ� ��� 2���� �迭�� ü���.
			int iArrayIndex = iIndex * (m_iWidth*m_iHeight);
			MazeBoxInfo MazeBox = pMazeBoxes[iIndex];// m_pMazeBoxes[iIndex];

			if (x == y)
			{
				m_pPloydVertexArray[iArrayIndex] = 1;
			}
			if (MazeBox.GetTop() == 0)
			{
				// [ iArrayIndex �� ����] + [4���� �߿� ���� �ִ� �÷��̵� �����ε���] = 1 
				m_pPloydVertexArray[iArrayIndex + iIndex - m_iWidth] = 1;
			}
			////////////////////////////////////////////////////////////////////////////////////////		
			if (MazeBox.GetBottom() == 0)
			{
				m_pPloydVertexArray[iArrayIndex + iIndex + m_iWidth] = 1;
			}
			///////////////////////////////////////////////////////////////////////////////////////////
			if (MazeBox.getLeft() == 0)
			{
				m_pPloydVertexArray[iArrayIndex + iIndex - 1] = 1;
			}
			/////////////////////////////////////////////////////////////////////////////////////////
			if (MazeBox.GetRight() == 0)
			{
				m_pPloydVertexArray[iArrayIndex + iIndex + 1] = 1;
			}
		}
	}

	ComputePloydPass();
	return true;
}
bool TPloyd::ComputePloydPass()
{
	DWORD i, j, k;
	// �̵������� ��θ� �����Ѵ�.
	for (i = 0; i < m_dwNumNodes; i++)
	{
		for (j = 0; j < m_dwNumNodes; j++)
		{
			if (m_pPloydVertexArray[i * m_dwNumNodes + j] == 0)
			{
				m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost = 9999;
			}
			else
			{
				m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost = 1;
				m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.push_back(j);
			}
		}
	}

	for (k = 0; k < m_dwNumNodes; k++)
	{
		for (i = 0; i < m_dwNumNodes; i++)
		{
			for (j = 0; j < m_dwNumNodes; j++)
			{
				// ���� ���� ������ �� ���� �����Ѵ�.
				// ���� ���� ������ ���� ������ ����� ������ ������ �����Ѵ�.
				int iNewCost = m_pPloydPathList[i*m_dwNumNodes + k].m_dwCost + m_pPloydPathList[k*m_dwNumNodes + j].m_dwCost;
				if (iNewCost < m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost)
				{
					m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost = iNewCost;
					// �̵���ΰ� �����Ǿ� ����ȴ�.          
					m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.assign(
						m_pPloydPathList[i*m_dwNumNodes + k].m_VertexList.begin(),
						m_pPloydPathList[i*m_dwNumNodes + k].m_VertexList.end());

					m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.insert(
						m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.end(),
						m_pPloydPathList[k*m_dwNumNodes + j].m_VertexList.begin(),
						m_pPloydPathList[k*m_dwNumNodes + j].m_VertexList.end());
				}
			}
		}
	}
	return true;
}
TPloydPath*	TPloyd::GetPloydPathList(DWORD dwStart, DWORD dwEnd)
{
	return &m_pPloydPathList[dwStart*m_dwNumNodes + dwEnd];
}


TPloyd::TPloyd()
{
}


TPloyd::~TPloyd()
{
}
bool     TPloyd::IsRoad(TIndex s)
{
	if (!(s.x >= 0 && s.x < m_iWidth && s.y >= 0 && s.y < m_iHeight))
	{
		return false;
	}
	if( m_pPloydVertexArray[s.y* m_iWidth + s.x] == 1 )
	{
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////
//  ����� ���� Ÿ�� ����  �÷��̵� ���(2)
//////////////////////////////////////////////////////////////////
bool TPloyd::TilePolydPath()
{
	int iMaxVertex = m_iWidth * m_iHeight;
	// �ʱ� �迭 ����
	m_vertexArray.resize(iMaxVertex);
	for (int X = 0; X < iMaxVertex; X++)
	{
		m_vertexArray[X].resize(iMaxVertex);
	}

	for (int iRow = 0; iRow < m_iHeight; iRow++)
	{
		for (int iCol = 0; iCol < m_iWidth; iCol++)
		{
			int iTile = iRow * m_iWidth + iCol;
			TIndex index(iCol, iRow);
			if (IsRoad(index) == false)
			{
				for (int iCount = 0; iCount < iMaxVertex; iCount++)
				{
					m_vertexArray[iTile][iCount].iCost = -1;
				}
				continue;
			}
			for (int iCount = 0; iCount < iMaxVertex; iCount++)
			{
				int iY = iCount / m_iWidth;
				int iX = iCount % m_iWidth;
				int iTileSub = iY * m_iWidth + iX;
				m_vertexArray[iTile][iTileSub].iCost = 999999;
			}
			// ���� �ִ� Ÿ���� ��츸
			m_vertexArray[iTile][iTile].iCost = 0;

			// ��			
			index.x = iCol + 1;
			index.y = iRow;
			if (index.x >= 0 && index.x < m_iWidth &&
				index.y >= 0 && index.y < m_iHeight)
			{
				if (IsRoad(index))
				{
					int iTileSub = index.y * m_iWidth + index.x;
					m_vertexArray[iTile][iTileSub].iCost = 1;
					m_vertexArray[iTile][iTileSub].path.push_back(index);
				}
			}
			// ��			
			index.x = iCol - 1;
			index.y = iRow;
			if (index.x >= 0 && index.x < m_iWidth &&
				index.y >= 0 && index.y < m_iHeight)
			{
				if (IsRoad(index))
				{
					int iTileSub = index.y * m_iWidth + index.x;
					m_vertexArray[iTile][iTileSub].iCost = 1;
					m_vertexArray[iTile][iTileSub].path.push_back(index);
				}
			}
			// ��			
			index.x = iCol;
			index.y = iRow + 1;
			if (index.x >= 0 && index.x < m_iWidth &&
				index.y >= 0 && index.y < m_iHeight)
			{
				if (IsRoad(index))
				{
					int iTileSub = index.y * m_iWidth + index.x;
					m_vertexArray[iTile][iTileSub].iCost = 1;
					m_vertexArray[iTile][iTileSub].path.push_back(index);
				}
			}
			// ��			
			index.x = iCol;
			index.y = iRow - 1;
			if (index.x >= 0 && index.x < m_iWidth &&
				index.y >= 0 && index.y < m_iHeight)
			{
				if (IsRoad(index))
				{
					int iTileSub = index.y * m_iWidth + index.x;
					m_vertexArray[iTile][iTileSub].iCost = 1;
					m_vertexArray[iTile][iTileSub].path.push_back(index);
				}
			}
		}
	}
	// ���Ḯ��Ʈ �н�
	for (int k = 0; k < iMaxVertex; k++)
	{
		for (int i = 0; i < iMaxVertex; i++)
		{
			for (int j = 0; j < iMaxVertex; j++)
			{
				if (m_vertexArray[i][k].iCost == -1) continue;
				if (m_vertexArray[k][j].iCost == -1) continue;
				if (m_vertexArray[i][j].iCost == -1) continue;

				if (m_vertexArray[i][k].iCost + m_vertexArray[k][j].iCost < m_vertexArray[i][j].iCost)
				{
					m_vertexArray[i][j].iCost = m_vertexArray[i][k].iCost + m_vertexArray[k][j].iCost;
					// �̵���ΰ� �����Ǿ� ����ȴ�.          
					m_vertexArray[i][j].path.assign(
						m_vertexArray[i][k].path.begin(),
						m_vertexArray[i][k].path.end());

					m_vertexArray[i][j].path.insert(
						m_vertexArray[i][j].path.end(),
						m_vertexArray[k][j].path.begin(),
						m_vertexArray[k][j].path.end());
				}
			}
		}
	}

	return true;
}