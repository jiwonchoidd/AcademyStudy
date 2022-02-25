#include "KUI.h"

#pragma region 기본 UI
bool KUI::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_pContext = context;
	m_CollisonType = Ignore;
	m_SelectType = Select_Overlap;
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

bool KUI::Frame()
{
	KObject::Frame();
	return true;
}

bool KUI::Render(ID3D11DeviceContext* pContext)
{
	KObject::Render(pContext);
	return true;
}
// 9등분, 한 이미지를 안깨지게 활용하기 위함
bool KUI::SetVertexData()
{
	float value = m_image_ratio;

	KVector2 left_top = { value, value };
	KVector2 right_top = { value, value };
	KVector2 left_bottom = { value, value };
	KVector2 right_bottom = { value, value };

	KVector2 tLT = { left_top.x / 196.0f, left_top.y / 124.0f };
	KVector2 tRT = { right_top.x / 196.0f, right_top.y / 124.0f };
	KVector2 tLB = { left_bottom.x / 196.0f, left_bottom.y / 124.0f };
	KVector2 tRB = { right_top.x / 196.0f, right_top.y / 124.0f };

	std::vector<PNCT_VERTEX> list(16);
	float halfWidth = m_rtSize.width / 2.0f;
	float halfHeight = m_rtSize.height / 2.0f;
	list[0].pos = { m_pos.x - halfWidth, m_pos.y - halfHeight };
	list[0].tex = { 0,0 };
	list[3].pos = { m_pos.x + halfWidth, m_pos.y - halfHeight };
	list[3].tex = { 1,0 };
	list[12].pos = { m_pos.x - halfWidth, m_pos.y + halfHeight };
	list[12].tex = { 0, 1 };
	list[15].pos = { m_pos.x + halfWidth, m_pos.y + halfHeight };
	list[15].tex = { 1,1 };
	list[5].pos = { list[0].pos.x + left_top.x, list[0].pos.y + left_top.y };
	list[5].tex = { list[0].tex.x + tLT.x, list[0].tex.y + tLT.y };
	list[6].pos = { list[3].pos.x - right_top.x, list[3].pos.y + right_top.y };
	list[6].tex = { list[3].tex.x - tRT.x, list[3].tex.y + tRT.y };
	list[9].pos = { list[12].pos.x + left_bottom.x, list[12].pos.y - left_bottom.y };
	list[9].tex = { list[12].tex.x + tLB.x, list[12].tex.y - tLB.y };
	list[10].pos = { list[15].pos.x - right_bottom.x, list[15].pos.y - right_bottom.y };
	list[10].tex = { list[15].tex.x - tRB.x, list[15].tex.y - tRB.y };

	list[1].pos = { list[5].pos.x, list[0].pos.y };
	list[1].tex = { list[5].tex.x, list[0].tex.y };
	list[2].pos = { list[6].pos.x, list[3].pos.y };
	list[2].tex = { list[6].tex.x, list[3].tex.y };

	list[4].pos = { list[0].pos.x, list[5].pos.y };
	list[4].tex = { list[0].tex.x, list[5].tex.y };
	list[7].pos = { list[3].pos.x, list[6].pos.y };
	list[7].tex = { list[3].tex.x, list[6].tex.y };

	list[8].pos = { list[12].pos.x, list[9].pos.y };
	list[8].tex = { list[12].tex.x, list[9].tex.y };
	list[11].pos = { list[15].pos.x, list[10].pos.y };
	list[11].tex = { list[15].tex.x, list[10].tex.y };

	list[13].pos = { list[9].pos.x, list[12].pos.y };
	list[13].tex = { list[9].tex.x, list[12].tex.y };
	list[14].pos = { list[10].pos.x, list[15].pos.y };
	list[14].tex = { list[10].tex.x, list[15].tex.y };

	for (int i = 0; i < list.size(); i++)
	{
		list[i].pos.x = list[i].pos.x / g_rtClient.right;
		list[i].pos.y = list[i].pos.y / g_rtClient.bottom;
		list[i].pos.x = list[i].pos.x * 2.0f - 1.0f;
		list[i].pos.y = -1.0f * (list[i].pos.y * 2.0f - 1.0f);
	}
	m_VertexList = list;
	return true;
}

bool KUI::SetIndexData()
{
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(4);
	m_IndexList.push_back(4); m_IndexList.push_back(1); m_IndexList.push_back(5);
	m_IndexList.push_back(1); m_IndexList.push_back(2); m_IndexList.push_back(5);
	m_IndexList.push_back(5); m_IndexList.push_back(2); m_IndexList.push_back(6);
	m_IndexList.push_back(2); m_IndexList.push_back(3); m_IndexList.push_back(6);
	m_IndexList.push_back(6); m_IndexList.push_back(3); m_IndexList.push_back(7);

	m_IndexList.push_back(4); m_IndexList.push_back(5); m_IndexList.push_back(8);
	m_IndexList.push_back(8); m_IndexList.push_back(5); m_IndexList.push_back(9);
	m_IndexList.push_back(5); m_IndexList.push_back(6); m_IndexList.push_back(9);
	m_IndexList.push_back(9); m_IndexList.push_back(6); m_IndexList.push_back(10);
	m_IndexList.push_back(6); m_IndexList.push_back(7); m_IndexList.push_back(10);
	m_IndexList.push_back(10); m_IndexList.push_back(7); m_IndexList.push_back(11);


	m_IndexList.push_back(8); m_IndexList.push_back(9); m_IndexList.push_back(12);
	m_IndexList.push_back(12); m_IndexList.push_back(9); m_IndexList.push_back(13);
	m_IndexList.push_back(9); m_IndexList.push_back(10); m_IndexList.push_back(13);
	m_IndexList.push_back(13); m_IndexList.push_back(10); m_IndexList.push_back(14);
	m_IndexList.push_back(10); m_IndexList.push_back(11); m_IndexList.push_back(14);
	m_IndexList.push_back(14); m_IndexList.push_back(11); m_IndexList.push_back(15);
	return true;
}

KUI::KUI()
{
	m_pContext = nullptr;
	m_image_ratio = 0.0f;
}

KUI::~KUI()
{
}
#pragma endregion

#pragma region 이미지
bool KImage::Frame()
{
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	m_cbData.vLightDir = KVector4(
		m_fAlpha,
		m_fAlpha,
		m_fAlpha, 1.0f);
	m_cbData.vValue = KVector4(
		g_fSecTimer,
		0,
		0,
		1.0f);
	return true;
}

void KImage::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 0.3f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}
}

void KImage::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.3f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
#pragma endregion
//버튼 클릭
void KButton::SelectOverlap(KCollider* pObj, DWORD dwState)
{
	if (m_PreSelectState == m_SelectState)
	{
		return;
	}
	if (m_datalist.size() <= 0) return;

	switch (m_SelectState)
	{
		case KSelectState::S_SELECTED:
		{
			//클릭 활성화
			m_btn_active = true;
			if(m_datalist[2].pSound!=nullptr)
			m_datalist[2].pSound->SoundPlay_Once();
			if (m_datalist[2].pTex != nullptr)
			m_pColorTex = m_datalist[2].pTex;
		}break;
		case KSelectState::S_HOVER:
		{
			if (m_datalist[1].pSound != nullptr)
			m_datalist[1].pSound->SoundPlay_Once();
			if (m_datalist[1].pTex != nullptr)
			m_pColorTex = m_datalist[1].pTex;
		}break;
		case KSelectState::S_ACTIVE:
		{
			if (m_datalist[2].pSound != nullptr)
			m_datalist[2].pSound->SoundPlay_Once();
			if (m_datalist[2].pTex != nullptr)
			m_pColorTex = m_datalist[2].pTex;
		}break;
		case KSelectState::S_FOCUS:
		{
			if (m_datalist[0].pTex != nullptr)
			m_pColorTex = m_datalist[0].pTex;
		}break;
		default:
		{
			m_btn_active = false;
		}
	}
	m_PreSelectState = m_SelectState;
}

bool KButton::Frame()
{
	return true;
}


bool KPlayerMenu::Load(ID3D11DeviceContext* context)
{
	m_pContext = context;
	img_background.m_image_ratio = 50.0f;
	img_background.SetRectDraw({ 0, 0, g_rtClient.right / (LONG)2.5f, g_rtClient.bottom / (LONG)1.2f });
	img_background.SetPosition(KVector2(g_rtClient.right, g_rtClient.top));
	img_background.Init(m_pContext, L"../../data/shader/VSPS_UI_0.txt",
		L"../../data/shader/VSPS_UI_0.txt",
		L"../../data/texture/menu_background.png", L"");

	for (int i = 0; i < 5; i++)
	{
		KButton btn;
		btn.m_image_ratio = 50.0f;
		btn.SetRectDraw({ 0, 0, g_rtClient.right / (LONG)2.5f, g_rtClient.bottom / (LONG)1.2f });
		btn.SetPosition(KVector2{ img_background.m_pos.x, img_background.m_pos.y + i * 3 });

		KTexture* pTex = g_TextureMananger.Load(L"../../data/texture/blank.bmp");
		KSound* pSound = g_SoundManager.LoadSound(L"../../data/sound/menu_open.mp3");
		// 가변인자를 통해서 생성자 직접 호출
		btn.m_datalist.emplace_back(pTex, pSound);

		pTex = g_TextureMananger.Load(L"../../data/texture/menu_hover.png");
		pSound = g_SoundManager.LoadSound(L"../../data/sound/menu_hover.mp3");
		btn.m_datalist.emplace_back(pTex, pSound);

		pTex = g_TextureMananger.Load(L"../../data/ui/menu_hover.png");
		pSound = g_SoundManager.LoadSound(L"../../data/sound/menu_select.mp3");
		btn.m_datalist.emplace_back(pTex, pSound);


		btn.Init(m_pContext, L"../../data/shader/VSPS_UI_0.txt",
			L"../../data/shader/VSPS_UI_0.txt",
			L"../../data/texture/blank.bmp", L"");
		btn_list.emplace_back(btn);
	}
	return true;
}

bool KPlayerMenu::Frame()
{
	return true;
}

bool KPlayerMenu::Render(ID3D11DeviceContext* context)
{
	img_background.Render(context);
	for (auto src : btn_list)
	{
		src.Render(context);
	}
	for (auto src : img_list)
	{
		src.Render(context);
	}
	return true;
}

bool KPlayerMenu::Release()
{
	img_background.Release();
	for (auto src : btn_list)
	{
		src.Release();
	}
	for (auto src : img_list)
	{
		src.Release();
	}
	btn_list.clear();
	img_list.clear();
	return true;
}
