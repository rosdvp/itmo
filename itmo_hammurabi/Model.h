#pragma once

#include <vector>

#include "EGameResult.h"

class Model
{
public:
	bool IsEnded;

	int Round;
	int Persons;
	int Wheat;
	int Grounds;
	int GroundCostWheat;

	bool IsPlague;
	int DiedPersons;
	int ArrivedPersons;
	int WheatFromEachGround;
	int SummaryWheatFromGrounds;
	int WheatSpentAtRats;

	std::vector<float>* RoundsDeathRates;

	EGameResult Result;
	float ResultAvgDeathRate;
	float ResultGroundsPerPerson;


	Model()
	{
		RoundsDeathRates = new std::vector<float>();
	}
	~Model()
	{
		delete RoundsDeathRates;
	}
};
