#pragma once
#include <vector>

#include "EGameResult.h"


class Config
{
public:
	const int RoundsCount       = 10;
	const float DeathRateToLose = 0.45f;

	const int StartPersons = 100;
	const int StartWheat = 2800;
	const int StartGrounds = 1000;

	const int GroundCostMin = 17;
	const int GroundCostMax = 26;

	const int WheatForEachPerson           = 20;
	const float WheatForGround           = 0.5f;
	const float WheatForRatsRatioMin     = 0.0f;
	const float WheatForRatsRationMax    = 0.07f;
	const int PersonCultivatesGroundsMax = 10;
	
	const int WheatFromGroundMin  = 1;
	const int WheatFromGroundMax  = 6;

	const int ArrivedPersonsMin = 0;
	const int ArrivedPersonsMax = 50;

	const float PlagueChance    = 0.15f;
	const float PlagueDeathRate = 0.5f;

	class ConfigGameResult;
	const std::vector<ConfigGameResult> Results =
	{
		ConfigGameResult{Bad, 0.33f, 7},
		ConfigGameResult{Satisfactory, 0.1f, 9},
		ConfigGameResult{Good, 0.03f, 10}
	};


	class ConfigGameResult
	{
	public:
		const EGameResult Result;
		const float AvgDeathRateMoreThan;
		const float GroundsPerPersonLessThen;
	};
};
