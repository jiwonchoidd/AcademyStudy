#include "KFbxObj.h"
#define _CRT_SECURE_NO_WARNINGS

void	KFbxObj::SetMatrix(
	KMatrix* pMatWorld,
	KMatrix* pMatView, KMatrix* pMatProj)
{
	if (pMatWorld != nullptr)
	{
		m_cbData.matWorld = *pMatWorld;
	}
	if (pMatView != nullptr)
	{
		m_cbData.matView = *pMatView;
	}
	if (pMatProj != nullptr)
	{
		m_cbData.matProj = *pMatProj;
	}
}
//dx�� ����� �켱 ���� max�� ������ �켱���� �׸��� �൵ �ٸ��� ������ ������ ��ȯ���ش�  
KMatrix     KFbxObj::DxConvertMatrix(KMatrix m)
{
	KMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
//����
KMatrix     KFbxObj::ConvertMatrix(FbxMatrix& m)
{
	KMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}
void KFbxObj::LoadMaterial(KMtrl* pMtrl)
{
	//���� ���͸����϶��� ��Ʈ ���͸��� �ϳ� ������ ������
	if (pMtrl == nullptr) return;
	if (pMtrl->m_pSubMtrl.size() > 0)
	{
		//���� ��Ʈ���� ũ�� ��ŭ ���� ����
		for (int iSub = 0; iSub < pMtrl->m_pSubMtrl.size(); iSub++)
		{
			KMtrl* pSubMtrl = pMtrl->m_pSubMtrl[iSub];
			FbxSurfaceMaterial* pFbxMaterial = pSubMtrl->m_pFbxSurfaceMtrl;
			FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (prop.IsValid())
			{
				//�ؽ��� �� �������� �Լ�
				int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
				for (int iTex = 0; iTex < iTexCount; iTex++)
				{
					FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
					//�ؽ��İ� �������� ��������
					if (pTex == nullptr) continue;
					FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

					std::string szFileName;
					char Drive[MAX_PATH] = { 0, };
					char Dir[MAX_PATH] = { 0, };
					char FName[MAX_PATH] = { 0, };
					char Ext[MAX_PATH] = { 0, };
					//tga���� �������� �ʾƼ� Ȯ���� ȣȯ
					if (fileTexture->GetFileName())
					{
						_splitpath_s(fileTexture->GetFileName(), Drive, Dir, FName, Ext);
						Ext[4] = 0;
						szFileName = FName;
						if (_stricmp(Ext, ".tga") == 0)
						{
							szFileName += ".dds";
						}
						else
						{
							szFileName += Ext;
						}
					}

					pSubMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
					pSubMtrl->m_Texture.m_szFileName += L"object/";
					pSubMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
					pSubMtrl->m_Texture.LoadTexture(
						pSubMtrl->m_Texture.m_szFileName);
				}
			}
		}
	}
	else
	{
		//���͸����� ù��° �ּҰ��� �޾ƿ� 
		FbxSurfaceMaterial* pFbxMaterial = pMtrl->m_pFbxNode->GetMaterial(0);
		//��ǻ���� �Ӽ��� prop�� ����
		FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (prop.IsValid())
		{
			int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
			for (int iTex = 0; iTex < iTexCount; iTex++)
			{
				FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
				if (pTex == nullptr) continue;
				FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

				std::string szFileName;
				char Drive[MAX_PATH] = { 0, };
				char Dir[MAX_PATH] = { 0, };
				char FName[MAX_PATH] = { 0, };
				char Ext[MAX_PATH] = { 0, };
				if (fileTexture->GetFileName())
				{
					_splitpath_s(fileTexture->GetFileName(), Drive, Dir, FName, Ext);
					Ext[4] = 0;
					szFileName = FName;
					if (_stricmp(Ext, ".tga") == 0)
					{
						szFileName += ".dds";
					}
					else
					{
						szFileName += Ext;
					}
				}

				pMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
				pMtrl->m_Texture.m_szFileName += L"object/";
				pMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
				pMtrl->m_Texture.LoadTexture(pMtrl->m_Texture.m_szFileName);
			}
		}
	}
}
bool    KFbxObj::Render(ID3D11DeviceContext* pContext)
{
	// ������͸��� �����Ҷ� ���� �����ϰų� ��Ʈ���͸��� �ϳ��϶� �����ϴ� ���
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		KMesh* pMesh = m_pMeshList[iObj];
		KMtrl* pMtrl = nullptr;
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSub = 0; iSub < pMesh->m_pSubMesh.size(); iSub++)
			{
				KMtrl* pSubMtrl=m_pFbxMaterialList[pMesh->m_iMtrlRef]->m_pSubMtrl[iSub];
				pContext->PSSetSamplers(0, 1, &pSubMtrl->m_Texture.m_pSampler);
				pContext->PSSetShaderResources(1, 1, &pSubMtrl->m_Texture.m_pTextureSRV);
				pMesh->m_pSubMesh[iSub]->SetMatrix(&pMesh->m_matWorld, &m_cbData.matView, &m_cbData.matProj);
				pMesh->m_pSubMesh[iSub]->Render(pContext);
			}
		}
		else
		{
			if (pMesh->m_iMtrlRef >= 0)
			{
				pMtrl = m_pFbxMaterialList[pMesh->m_iMtrlRef];
			}
			if (pMtrl != nullptr)
			{
				pContext->PSSetSamplers(0, 1, &pMtrl->m_Texture.m_pSampler);
				pContext->PSSetShaderResources(1, 1, &pMtrl->m_Texture.m_pTextureSRV);
			}
			pMesh->SetMatrix(&pMesh->m_matWorld, &m_cbData.matView, &m_cbData.matProj);
			pMesh->Render(pContext);
		}
	}
	return true;
}
int     KFbxObj::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		if (m_pFbxMaterialList[iMtrl]->m_pFbxSurfaceMtrl == pFbxMaterial)
		{
			return iMtrl;
		}
	}
	return -1;
}

//��� �ؼ��ϴ� �Լ�
void	KFbxObj::ParseNode(FbxNode* pNode, KMesh* pMesh)
{
	//������ 0��° �ּҰ��� �����ͼ� ���Ѵ�.
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	pMesh->m_iMtrlRef = GetRootMtrl(pFbxMaterial);

	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	std::vector< std::string> fbxFileTexList;

	if (pFbxMesh != nullptr)
	{
		//���� ���� ���̾� ����ŭ 
		pMesh->m_iNumLayer = pFbxMesh->GetLayerCount();
		pMesh->m_LayerList.resize(pMesh->m_iNumLayer);
		// todo : �������� ���̾� ����Ʈ
		for (int iLayer = 0; iLayer < pMesh->m_iNumLayer; iLayer++)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
			if (pLayer->GetVertexColors() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pColor = pLayer->GetVertexColors();
			}
			if (pLayer->GetNormals() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pNormal = pLayer->GetNormals();
			}
			if (pLayer->GetUVs() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pUV = pLayer->GetUVs();
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pMaterial = pLayer->GetMaterials();
			}
		}
		//�ִϸ��̼� ��Ʈ����, �Ǻ��� ��� ������ ����
		FbxAMatrix matGeo;
		{
			FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
			matGeo.SetT(trans);
			matGeo.SetR(rot);
			matGeo.SetS(scale);
		}
		FbxMatrix matA = matGeo;
		pMesh->m_matWorld = DxConvertMatrix(ConvertMatrix(matA));
		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// ��������Ʈ �ּ�
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		int iBasePlayIndex = 0;
		int iNumFbxMaterial = pNode->GetMaterialCount();
		if (iNumFbxMaterial > 1)
		{
			pMesh->m_pSubMesh.resize(iNumFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				pMesh->m_pSubMesh[iSub] = new KMesh;
			}
		}

		FbxLayerElementMaterial* fbxSubMaterial =
			pMesh->m_LayerList[0].pMaterial;
		FbxLayerElementUV* VertexUVList =
			pMesh->m_LayerList[0].pUV;
		FbxLayerElementVertexColor* VertexColorList =
			pMesh->m_LayerList[0].pColor;
		FbxLayerElementNormal* VertexNormalList =
			pMesh->m_LayerList[0].pNormal;

		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			int iSubMtrlIndex = 0;
			if (fbxSubMaterial != nullptr)
			{
				switch (fbxSubMaterial->GetMappingMode())
				{
				case FbxLayerElement::eByPolygon:
				{
					switch (fbxSubMaterial->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
					{
						iSubMtrlIndex = iPoly;
					}break;
					case FbxLayerElement::eIndex:
					case FbxLayerElement::eIndexToDirect:
					{
						iSubMtrlIndex =
							fbxSubMaterial->GetIndexArray().GetAt(iPoly);
					}break;
					}
				}break;
				default:
				{
					iSubMtrlIndex = 0;
				}break;
				}
			}
			// �ﰢ��, �簢��
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iCornerIndex[3];
			for (int iTriangle = 0;
				iTriangle < m_iNumTriangle;
				iTriangle++)
			{
				// ��ġ �ε��� yz ��ǥ �ٲ㼭 �־����
				iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
				iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);
				// UV �ε��� yz ��ǥ �ٲ㼭 �־����
				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
				u[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
				u[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

				for (int iIndex = 0;
					iIndex < 3;
					iIndex++)
				{
					PNCT_VERTEX vertex;
					FbxVector4 pos = pVertexPositions[iCornerIndex[iIndex]];
					vertex.pos.x = pos.mData[0];
					vertex.pos.y = pos.mData[2];
					vertex.pos.z = pos.mData[1];
					if (VertexUVList != nullptr)
					{
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, VertexUVList,
							iCornerIndex[iIndex], u[iIndex]);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f - uv.mData[1];
					}
					if (VertexColorList != nullptr)
					{
						int iColorIndex[3];
						iColorIndex[0] = iBasePlayIndex + 0;
						iColorIndex[1] = iBasePlayIndex + iTriangle + 2;
						iColorIndex[2] = iBasePlayIndex + iTriangle + 1;

						FbxColor color = ReadColor(
							pFbxMesh, 1, VertexColorList,
							iCornerIndex[iIndex], iColorIndex[iIndex]);
						vertex.color.x = color.mRed;
						vertex.color.y = color.mGreen;
						vertex.color.z = color.mBlue;
						vertex.color.w = 1.0f;

					}
					if (VertexNormalList != nullptr)
					{
						int iNormalIndex[3];
						iNormalIndex[0] = iBasePlayIndex + 0;
						iNormalIndex[1] = iBasePlayIndex + iTriangle + 2;
						iNormalIndex[2] = iBasePlayIndex + iTriangle + 1;
						FbxVector4 normal = ReadNormal(
							pFbxMesh, 1, VertexNormalList,
							iCornerIndex[iIndex], iNormalIndex[iIndex]);
						vertex.normal.x = normal.mData[0];
						vertex.normal.y = normal.mData[2];
						vertex.normal.z = normal.mData[1];
					}
					if (iNumFbxMaterial > 1)
					{
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_pVertexList.push_back(vertex);
					}
					else
					{
						pMesh->m_pVertexList.push_back(vertex);
					}
				}
			}
			iBasePlayIndex += iPolySize;
		}
	}
}
//FbxVector2 KFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
//{
//	FbxVector2 uv(0, 0);
//	if (dwVertexTextureCount < 1 || pUVSet == nullptr)
//	{
//		return uv;
//	}
//	int iVertexTextureCountLayer = pFbxMesh->GetElementUVCount();
//	FbxLayerElementUV* pFbxLayerElementUV = pFbxMesh->GetElementUV(0);
//
//	// �������� ����� 4�� ����, ������ ������ 6�� ������ �����Ѵ�.
//	// �׷��� �ؽ�ó ��ǥ�� ���� ���̾� ���� ������ �Ǵ� �������� ���еȴ�.
//	switch (pUVSet->GetMappingMode())
//	{
//	case FbxLayerElementUV::eByControlPoint: // ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
//	{
//		switch (pUVSet->GetReferenceMode())
//		{
//		case FbxLayerElementUV::eDirect: // �迭���� ���� ��´�.
//		{
//			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
//			uv.mData[0] = fbxUv.mData[0];
//			uv.mData[1] = fbxUv.mData[1];
//			break;
//		}
//		case FbxLayerElementUV::eIndexToDirect: // �迭�� �ش��ϴ� �ε����� ���Ͽ� ��´�.
//		{
//			int id = pUVSet->GetIndexArray().GetAt(vertexIndex);
//			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(id);
//			uv.mData[0] = fbxUv.mData[0];
//			uv.mData[1] = fbxUv.mData[1];
//			break;
//		}
//		}
//		break;
//	}
//	case FbxLayerElementUV::eByPolygonVertex: // ���� �� 1���� ���� ��ǥ�� �ִ�.
//	{
//		switch (pUVSet->GetReferenceMode())
//		{
//		case FbxLayerElementUV::eDirect:
//		case FbxLayerElementUV::eIndexToDirect:
//		{
//			uv.mData[0] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[0];
//			uv.mData[1] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[1];
//			break;
//		}
//		}
//		break;
//	}
//	}
//	return uv;
//}
void	KFbxObj::PreProcess(FbxNode* pNode)
{
	// pNode ���� ���
	int iNumFbxMaterial = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	//������͸����� �ִٸ�
	if (iNumFbxMaterial > 1)
	{
		KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
		//������͸��� ����ŭ ���͸��� ��ü�� �־��ش�.
		for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
		{
			FbxSurfaceMaterial* pFbxSubMaterial = pNode->GetMaterial(iSub);
			_ASSERT(pFbxSubMaterial != nullptr);
			KMtrl* pSubMtrl = new KMtrl(pNode, pFbxSubMaterial);
			pMtrl->m_pSubMtrl.push_back(pSubMtrl);
		}
		m_pFbxMaterialList.push_back(pMtrl);
	}
	else
	{
		if (pFbxMaterial != nullptr)
		{
			KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
			m_pFbxMaterialList.push_back(pMtrl);
		}
	}

	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		FbxNodeAttribute::EType type =
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			m_pFbxNodeList.push_back(pChildNode);
		}
		PreProcess(pChildNode);
	}
}
bool	KFbxObj::LoadObject(std::string filename)
{
	//fbx �δ� ����
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	bool bRet = m_pFbxImporter->Initialize(filename.c_str());
	bRet = m_pFbxImporter->Import(m_pFbxScene);

	FbxNode* m_pRootNode = m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode);
	// todo : �ߺ�ó�� ���۾�
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		KMtrl* pMtrl = m_pFbxMaterialList[iMtrl];
		LoadMaterial(pMtrl);
	}
	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
	{
		FbxNode* pNode = m_pFbxNodeList[iNode];
		KMesh* pMesh = new KMesh;
		m_pMeshList.push_back(pMesh);
		ParseNode(pNode, pMesh);
	}
	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
	{
		KMesh* pMesh = m_pMeshList[iMesh];

		//���� ���͸��� ������ ���� �� ����
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				KMesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				// todo : ���̴� ��� �ߺ�ó�� ���۾�
				pSubMesh->CreateModel(L"FbxShader.hlsl", L"../../data/shader/DefaultShader.hlsl");
			}
		}
		else
		{
			pMesh->CreateModel(L"FbxShader.hlsl", L"../../data/shader/DefaultShader.hlsl");
		}
	}
	return bRet;
}
bool    KFbxObj::Release()
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		m_pMeshList[iObj]->Release();
		m_pMeshList[iObj]=nullptr;
	}
	for (int iObj = 0; iObj < m_pFbxMaterialList.size(); iObj++)
	{
		m_pFbxMaterialList[iObj]->Release();
		m_pFbxMaterialList[iObj]=nullptr;
	}
	return true;
}