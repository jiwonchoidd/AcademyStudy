#pragma once
#include "TImageManager.h"
#include "TSoundManager.h"
class Sample
{
public:
	TImageManager<TImage>  m_ImageMgr;
	TSoundManager<TSound>  m_SoundMgr;
};

