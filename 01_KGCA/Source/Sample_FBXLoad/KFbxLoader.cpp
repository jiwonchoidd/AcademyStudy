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
	//파일 - > 임포터 -> 씬 (트리) 해석 -> 매쉬 -> 정보 가져오기 pnct -> 출력

	std::string temp = to_wm(filename);
	bool bRet = m_pFbxImporter->Initialize(temp.c_str()); //파일명 넘김
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene); //마야 Z축 버젼 사용

	if (bRet)
	{
		//fbx는 트리 구조로 이어져있음
		//재귀호출로 전 순회 가능, N 트리여서 자식 수를 알아야함
		//N트리 : 자식 개수가 N개임
		m_pRootNode = m_pFbxScene->GetRootNode();

		//전 순회해서 씬에 저장되어 있는 트리구조에서 오브젝트를 찾아옴
		NodeProcess(nullptr, m_pRootNode);

		//N트리에서 찾아낸 오브젝트를 해석함
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
	// 카메라나 라이트 등 매쉬가 아니라면 리턴
	/*if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}*/
	//매쉬타입 이니까 매쉬를 얻어옴
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
	//현재 노드의 매쉬를 만듬, 버텍스 PNCT를 채워줘야함
	FbxMesh* pFbxMesh = pObject->m_pFbx_ThisNode->GetMesh();
	if (pFbxMesh)
	{
		//기하 행렬(FBX 위치 버텍스에서 -> 초기 정점 로컬 위치로 변환)
		FbxAMatrix  mat_Geo;
		FbxVector4	t = pObject->m_pFbx_ThisNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4	r = pObject->m_pFbx_ThisNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4	s = pObject->m_pFbx_ThisNode->GetGeometricScaling(FbxNode::eSourcePivot);

		mat_Geo.SetT(t);
		mat_Geo.SetR(r);
		mat_Geo.SetS(s);

		//노말 행렬, 기하행렬의 역행렬의 전치
		FbxAMatrix normalMatrix = mat_Geo;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();

		std::vector<FbxLayerElementMaterial*>	 MaterialSet; // 매터리얼
		std::vector<FbxLayerElementUV*>			 UVSet; // UV
		std::vector<FbxLayerElementVertexColor*> VertexColorSet;//

		// 노말맵을 위한 노말, 바이노말, 탄젠트
		std::vector<FbxLayerElementNormal*>		 NormalSet;  
		std::vector<FbxLayerElementBinormal*>    BinormalSet;
		std::vector<FbxLayerElementTangent*>     TangentSet; 

		int iLayerCount = pFbxMesh->GetLayerCount(); // 레이어 ( 1번에 랜더링, 여러번에 걸쳐서 랜더링 개념)

		for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
		{
			FbxLayer* pFbxLayer = pFbxMesh->GetLayer(iLayer);// 레이어에 UV 정보가 있음 필수적임

			//매터리얼, UV, 버텍스 컬러
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
			//노말값
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

		//매터리얼 개수 만큼 돌면서 읽어옴 
		//현재는 저장된 텍스쳐의 이름만 가져옴
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
		// 폴리곤, 면 개수 만큼 돌면서 위치를 저장
		// 삼각형, 사각형
		int iCurpolyIndex = 0; // 증가되는 폴리곤 인덱스
		int iNumPolyCount = pFbxMesh->GetPolygonCount(); //폴리곤 수
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); //정점 위치 
		int iNumFace = 0;
		for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
		{
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly); //4또는 3 삼각형이나 사각형이냐
			iNumFace = iPolySize - 2; // 한면
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
					v = mat_Geo.MultT(v); // 로컬 좌표로 행렬 곱 
					Vertex.pos.x = v.mData[0];
					Vertex.pos.y = v.mData[2];
					Vertex.pos.z = v.mData[1];

					// UV
					int u[3];
					u[0] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
					u[1] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
					u[2] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[2]);
					//UV 리스트에 값이 있다면
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
					//버텍스 컬러 값이 있다면
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
					//노말값이 있다면
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
	// 노말 획득 
	FbxVector4 result;
	// 노말 벡터를 저장할 벡터 
	switch (vertexNormal->GetMappingMode()) 	// 매핑 모드 
	{
		// 제어점 마다 1개의 매핑 좌표가 있다.
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
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// 정점 마다 1개의 매핑 좌표가 있다.
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
			// 인덱스를 얻어온다. 
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
