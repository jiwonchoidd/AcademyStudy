#include "KFbxLoader.h"

bool KFbxLoader::Init()
{
	m_pFbxManager = FbxManager::Create(); 
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	return true;
}
bool KFbxLoader::Load(std::wstring filename)
{
	//���� - > ������ -> �� (Ʈ��) �ؼ� -> �Ž� -> ���� �������� pnct -> ���

	std::string temp = to_wm(filename);
	bool bRet = m_pFbxImporter->Initialize(temp.c_str()); //���ϸ� �ѱ�
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene); //���� Z�� ���� ���

	if (bRet)
	{
		//fbx�� Ʈ�� ������ �̾�������
		//���ȣ��� �� ��ȸ ����, N Ʈ������ �ڽ� ���� �˾ƾ���
		//NƮ�� : �ڽ� ������ N����
		m_pRootNode = m_pFbxScene->GetRootNode();

		//�� ��ȸ�ؼ� ���� ����Ǿ� �ִ� Ʈ���������� ������Ʈ�� ã�ƿ�
		NodeProcess(nullptr, m_pRootNode);

		//NƮ������ ã�Ƴ� ������Ʈ�� �ؼ���
		for (int iobj = 0; iobj < m_ObjectList.size(); iobj++)
		{
			ParseMesh(m_ObjectList[iobj]);
		}
		return true;
	}
	return false;
}

void KFbxLoader::NodeProcess(FbxNode* pParentNode, FbxNode* pNode)
{
	// ī�޶� ����Ʈ �� �Ž��� �ƴ϶�� ����
	/*if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}*/
	//�Ž�Ÿ�� �̴ϱ� �Ž��� ����
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh)
	{
		KFBXObj* fbx = new KFBXObj;
		fbx->m_pFbx_ParentNode = pParentNode;
		fbx->m_pFbx_ThisNode = pNode;
		m_ObjectList.push_back(fbx);
	}

	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* child = pNode->GetChild(iNode);
		NodeProcess(pNode, child);
	}
}

void KFbxLoader::ParseMesh(KFBXObj* pObject)
{
	//���� ����� �Ž��� ����, ���ؽ� PNCT�� ä�������
	FbxMesh* pFbxMesh = pObject->m_pFbx_ThisNode->GetMesh();
	if (pFbxMesh)
	{
		//���� ���(FBX ��ġ ���ؽ����� -> �ʱ� ���� ���� ��ġ�� ��ȯ)
		FbxAMatrix  mat_Geo;
		FbxVector4	t = pObject->m_pFbx_ThisNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4	r = pObject->m_pFbx_ThisNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4	s = pObject->m_pFbx_ThisNode->GetGeometricScaling(FbxNode::eSourcePivot);

		mat_Geo.SetT(t);
		mat_Geo.SetR(r);
		mat_Geo.SetS(s);

		//�븻 ���, ��������� ������� ��ġ
		FbxAMatrix normalMatrix = mat_Geo;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();

		std::vector<FbxLayerElementMaterial*>	 MaterialSet; // ���͸���
		std::vector<FbxLayerElementUV*>			 UVSet; // UV
		std::vector<FbxLayerElementVertexColor*> VertexColorSet;//

		// �븻���� ���� �븻, ���̳븻, ź��Ʈ
		std::vector<FbxLayerElementNormal*>		 NormalSet;  
		std::vector<FbxLayerElementBinormal*>    BinormalSet;
		std::vector<FbxLayerElementTangent*>     TangentSet; 

		int iLayerCount = pFbxMesh->GetLayerCount(); // ���̾� ( 1���� ������, �������� ���ļ� ������ ����)

		for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
		{
			FbxLayer* pFbxLayer = pFbxMesh->GetLayer(iLayer);// ���̾ UV ������ ���� �ʼ�����

			//���͸���, UV, ���ؽ� �÷�
			if (pFbxLayer->GetMaterials() != nullptr)
			{
				MaterialSet.push_back(pFbxLayer->GetMaterials());
			}
			if (pFbxLayer->GetUVs() != nullptr)
			{
				UVSet.push_back(pFbxLayer->GetUVs());
			}
			if (pFbxLayer->GetVertexColors() != nullptr)
			{
				VertexColorSet.push_back(pFbxLayer->GetVertexColors());
			}
			//�븻��
			if (pFbxLayer->GetNormals() != nullptr)
			{
				NormalSet.push_back(pFbxLayer->GetNormals());
			}
			if (pFbxLayer->GetBinormals() != nullptr)
			{
				BinormalSet.push_back(pFbxLayer->GetBinormals());
			}
			if (pFbxLayer->GetTangents() != nullptr)
			{
				TangentSet.push_back(pFbxLayer->GetTangents());
			}
		}

		//���͸��� ���� ��ŭ ���鼭 �о�� 
		//����� ����� �ؽ����� �̸��� ������
		int iNumMtrl = pObject->m_pFbx_ThisNode->GetMaterialCount();
		for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
		{
			FbxSurfaceMaterial* pSurface = pObject->m_pFbx_ThisNode->GetMaterial(iMtrl);
			if (pSurface)
			{
				std::wstring texturename = to_mw(ParseMaterial(pSurface));
				if (!texturename.empty())
				{
					pObject->m_tex_name_diffuse = L"../../data/model/";
					pObject->m_tex_name_diffuse += texturename;
				}
			}
		}
		//----------------------------------------------------------
		// ������, �� ���� ��ŭ ���鼭 ��ġ�� ����
		// �ﰢ��, �簢��
		int iCurpolyIndex = 0; // �����Ǵ� ������ �ε���
		int iNumPolyCount = pFbxMesh->GetPolygonCount(); //������ ��
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); //���� ��ġ 
		int iNumFace = 0;
		for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
		{
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly); //4�Ǵ� 3 �ﰢ���̳� �簢���̳�
			iNumFace = iPolySize - 2; // �Ѹ�
			for (int iFace = 0; iFace < iNumFace; iFace++)
			{
				int VertexIndex[3] = { 0, iFace + 2, iFace + 1 };
				int CornerIndex[3];
				CornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[0]);
				CornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[1]);
				CornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[2]);
				for (int iIndex = 0; iIndex < 3; iIndex++)
				{
					PNCT_VERTEX Vertex;
					// Max(x,z,y) ->(dx)x,y,z    
					FbxVector4 v = pVertexPositions[CornerIndex[iIndex]];
					v = mat_Geo.MultT(v); // ���� ��ǥ�� ��� �� 
					Vertex.pos.x = v.mData[0];
					Vertex.pos.y = v.mData[2];
					Vertex.pos.z = v.mData[1];

					// UV
					int u[3];
					u[0] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
					u[1] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
					u[2] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[2]);
					//UV ����Ʈ�� ���� �ִٸ�
					if (UVSet.size() > 0)
					{
						FbxLayerElementUV* pUVSet = UVSet[0];
						FbxVector2 uv;
						ReadTextureCoord(
							pFbxMesh,
							pUVSet,
							CornerIndex[iIndex],
							u[iIndex],
							uv);
						Vertex.tex.x = uv.mData[0];
						Vertex.tex.y = 1.0f - uv.mData[1];
					}
					//----------------------------------------------------------
					//���ؽ� �÷� ���� �ִٸ�
					FbxColor color = FbxColor(1, 1, 1, 1);
					if (VertexColorSet.size() > 0)
					{
						color = ReadColor(pFbxMesh,
							VertexColorSet.size(),
							VertexColorSet[0],
							CornerIndex[iIndex],
							iCurpolyIndex + VertexIndex[iIndex]);
					}
					Vertex.color.x = color.mRed;
					Vertex.color.y = color.mGreen;
					Vertex.color.z = color.mBlue;
					Vertex.color.w = 1;
					//----------------------------------------------------------
					//�븻���� �ִٸ�
					if (NormalSet.size() > 0)
					{
						FbxVector4 normal = ReadNormal(pFbxMesh,
							CornerIndex[iIndex],
							iCurpolyIndex + VertexIndex[iIndex]);
						normal = normalMatrix.MultT(normal);
						Vertex.normal.x = normal.mData[0]; // x
						Vertex.normal.y = normal.mData[2]; // z
						Vertex.normal.z = normal.mData[1]; // y
					}

					if (BinormalSet.size() > 0)
					{

					}

					if (TangentSet.size() > 0)
					{

					}
					pObject->m_VertexList.push_back(Vertex);//36
				}
			}
			iCurpolyIndex += iPolySize;
		}
	}
}

std::string KFbxLoader::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	std::string mtl_name = pMtrl->GetName();
	auto dProperty = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	auto sProperty = pMtrl->FindProperty(FbxSurfaceMaterial::sSpecular);
	auto nProperty = pMtrl->FindProperty(FbxSurfaceMaterial::sNormalMap);
	if (dProperty.IsValid())
	{
		const FBXSDK_DLL::FbxFileTexture* tex = dProperty.GetSrcObject<FBXSDK_DLL::FbxFileTexture>(0);
		if (tex != nullptr)
		{
			const CHAR* szFileName = tex->GetFileName();
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath_s(szFileName, Drive, Dir, FName, Ext);
			std::string texName = FName;
			std::string ext = Ext;
			if (ext == ".tga" || ext == ".TGA")
			{
				ext.clear();
				ext = ".dds";
			}
			texName += ext;
			return texName;
		}
	}
	return std::string("");
}

void KFbxLoader::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex, FbxVector2& uv)
{
	FbxLayerElementUV* pFbxLayerElementUV = pUVSet;
	if (pFbxLayerElementUV == nullptr) {
		return;
	}

	switch (pFbxLayerElementUV->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
			// Always enters this part for the example model
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
}

FbxColor KFbxLoader::ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
		switch (pVertexColorSet->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}

FbxVector4 KFbxLoader::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// �븻 ȹ�� 
	FbxVector4 result;
	// �븻 ���͸� ������ ���� 
	switch (vertexNormal->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}

bool KFbxLoader::Frame()
{
	return true;
}

bool KFbxLoader::Render()
{
	return true;
}

bool KFbxLoader::Release()
{
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj]->Release();
	}
	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();
	return true;
}
