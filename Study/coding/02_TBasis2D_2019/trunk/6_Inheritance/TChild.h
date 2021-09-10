#pragma once
#include "TParent.h"
class TChild : public TParent
{
public:
	void Humanity(void);		// 추가된 멤버함수
	void Character(void);		// 수정된 멤버함수
	void Appearance(void);		// 확장된 멤버함수
public:
	TChild();
	~TChild();
};

