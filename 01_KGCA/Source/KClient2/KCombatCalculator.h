#pragma once
#include "KStd.h"
struct PoketmonInfo
{
	int	id;
	int level;
	int	hp;
};
class KCombatCalculator
{
public:
	int	ReturnStat(int level);
	int ReturnRandom(int start, int end);
	//int	Attack(int level, )
};

