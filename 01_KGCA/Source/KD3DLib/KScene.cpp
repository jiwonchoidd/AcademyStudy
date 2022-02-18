#include "KScene.h"
KScene* KScene::m_pCurrentScene = nullptr;

KScene::KScene()
{
	m_pd3dDevice = nullptr;
	m_pContext = nullptr;
}

KScene::~KScene()
{
}
