#pragma once
#include <iostream>
using namespace std;

class TParent
{
public:
	TParent(void);			// 생성자
	~TParent(void);			// 소멸자
	TParent(int money);

	void Character(void);	// 성품 출력
	void Appearance(void);	// 외모 출력
	void Wealth(void);		// 재산 출력

private:
	int money;			// 돈 저장

};

