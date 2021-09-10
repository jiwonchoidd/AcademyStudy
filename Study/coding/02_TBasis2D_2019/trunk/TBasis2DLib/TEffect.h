#pragma once
#include "TObject.h"

class TEffect : public TObject
{
public:
	std::vector<RECT> m_rtList;
	int		m_iRectIndex;
	float	m_fTimer;
	float	m_fStep;
public:
	void   SetSprite(std::vector<RECT>& list);
	virtual TObject* Clone() override;
	bool	Init();
	bool	Frame();
public:
	TEffect();
	virtual ~TEffect();
};

