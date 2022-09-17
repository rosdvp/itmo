#include "GameplayLogic.h"

#include <iostream>
#include <random>

GameplayLogic::GameplayLogic(Config* config, Model* model, View* view)
	: _config(config), _model(model), _view(view)
{
	srand(time(nullptr));
}

void GameplayLogic::FillModelByDefault() const
{
	_model->Persons = _config->StartPersons;
	_model->Wheat = _config->StartWheat;
	_model->Grounds = _config->StartGrounds;
	_model->GroundCostWheat = GetNewGroundCost();
}

bool GameplayLogic::PlayRoundUntilGameEnd() const
{
	if (_model->IsEnded)
	{
		_view->ShowRoundHeader(_model);
		_view->ShowGameResult(_model->Result, _model->ResultAvgDeathRate, _model->ResultGroundsPerPerson);
		return false;
	}

	if (_model->Round == 0)
		_view->ShowFirstRoundHeader(_model);
	else
		_view->ShowRoundHeader(_model);
	CalculateNextRound();
	return true;
}

void GameplayLogic::CalculateNextRound() const
{
	//--------Change grounds and wheat from player input-------
	int wheat = 0;
	int grounds = 0;
	int wheatForPersons = 0;
	int groundsToCultivate = 0;
	GetValidPlayerInput(_model->Grounds, _model->Wheat,
		grounds, wheat,
		wheatForPersons, groundsToCultivate);
	_model->Wheat = wheat;
	_model->Grounds = grounds;

	//--------People starving-------
	const int rawSurvivedPersons = wheatForPersons == 0 ? 0 : wheatForPersons / _config->WheatForEachPerson;
	_model->DiedPersons = rawSurvivedPersons >= _model->Persons ? 0 : _model->Persons - rawSurvivedPersons;
	const float deathRate = static_cast<float>(_model->DiedPersons) / _model->Persons;
	_model->RoundsDeathRates->push_back(deathRate);
	_model->Persons -= _model->DiedPersons;

	//--------Collecting wheat--------
	_model->WheatFromEachGround = GetRandom(_config->WheatFromGroundMin, _config->WheatFromGroundMax);
	_model->SummaryWheatFromGrounds = _model->WheatFromEachGround * groundsToCultivate;
	_model->Wheat += _model->SummaryWheatFromGrounds;

	//--------Rats eat wheat--------
	const float rawWheatSpentAtRats = _model->Wheat * GetRandom(_config->WheatForRatsRatioMin, _config->WheatForRatsRationMax);
	_model->WheatSpentAtRats = static_cast<int>(std::ceilf(rawWheatSpentAtRats));
	_model->Wheat -= _model->WheatSpentAtRats;

	//--------Persons arriving--------
	const float rawArrivedPersons = _model->DiedPersons / 2.0f +
		(5.0f - _model->WheatFromEachGround) * _model->SummaryWheatFromGrounds / 600.0f + 1;

	_model->ArrivedPersons = std::max(std::min(static_cast<int>(rawArrivedPersons),
		_config->ArrivedPersonsMax),
		_config->ArrivedPersonsMin);
	_model->Persons += _model->ArrivedPersons;

	//--------Ground cost--------
	_model->GroundCostWheat = GetNewGroundCost();

	//--------Plague--------
	_model->IsPlague = GetRandom(0.0f, 1.0f) <= _config->PlagueChance;
	if (_model->IsPlague)
		_model->Persons /= 2;

	//--------Check for game end--------
	if (_model->RoundsDeathRates->back() >= _config->DeathRateToLose)
	{
		_model->IsEnded = true;
		_model->Result = Bad;
	}
	if (++_model->Round == _config->RoundsCount)
	{
		_model->IsEnded = true;
		CalculateResult();
	}
}

int GameplayLogic::GetNewGroundCost() const
{
	return GetRandom(_config->GroundCostMin, _config->GroundCostMax);
}

void GameplayLogic::GetValidPlayerInput(
	const int currGrounds, const int currWheat,
	int& outGroundsLeft, int& outWheatLeft,
	int& outWheatForPersons, int& outGroundsToCultivate) const
{
	_view->ShowRequestHeader();
	while (true)
	{
		outGroundsLeft = currGrounds;
		outWheatLeft   = currWheat;

		const int groundsDelta = _view->RequestGroundsDelta();
		outGroundsLeft += groundsDelta;
		if (outGroundsLeft < 0)
		{
			_view->ShowInvalidInput(View::GroundsDeltaNotEnoughGrounds);
			continue;
		}
		//'-' below, since negative groundsDelta should produce positive wheatDelta
		outWheatLeft += _model->GroundCostWheat * (-groundsDelta);
		if (outWheatLeft < 0)
		{
			_view->ShowInvalidInput(View::GroundsDeltaNotEnoughWheat);
			continue;
		}

		outWheatForPersons = _view->RequestWheatForPersons();
		if (outWheatForPersons < 0)
		{
			_view->ShowInvalidInput(View::WheatForPersonsBelowZero);
			continue;
		}
		outWheatLeft -= outWheatForPersons;
		if (outWheatLeft < 0)
		{
			_view->ShowInvalidInput(View::WheatForPersonsNotEnough);
			continue;
		}

		outGroundsToCultivate = _view->RequestGroundsToCultivate();
		if (outGroundsToCultivate < 0)
		{
			_view->ShowInvalidInput(View::GroundsToCultivateBelowZero);
			continue;
		}
		if (outGroundsToCultivate > outGroundsLeft)
		{
			_view->ShowInvalidInput(View::GroundsToCultivateNotEnoughGrounds);
			continue;
		}
		if (outGroundsToCultivate > _model->Persons * _config->PersonCultivatesGroundsMax)
		{
			_view->ShowInvalidInput(View::GroundsToCultivateNotEnoughPersons);
			continue;
		}
		outWheatLeft -= static_cast<int>(std::ceilf(outGroundsToCultivate * _config->WheatForGround));
		if (outWheatLeft < 0)
		{
			_view->ShowInvalidInput(View::GroundsToCultivateNotEnoughWheat);
			continue;
		}
		return;
	}
}

void GameplayLogic::CalculateResult() const
{
	float sumDeathRate = 0.0f;
	for (const float deathRate : *_model->RoundsDeathRates)
		sumDeathRate += deathRate;
	_model->ResultAvgDeathRate = sumDeathRate / _model->RoundsDeathRates->size();
	_model->ResultGroundsPerPerson = static_cast<float>(_model->Grounds) / _model->Persons;
	
	for (const auto& configResult : _config->Results)
		if (_model->ResultAvgDeathRate >= configResult.AvgDeathRateMoreThan &&
			_model->ResultGroundsPerPerson <= configResult.GroundsPerPersonLessThen)
		{
			_model->Result = configResult.Result;
			return;
		}
	_model->Result = Perfect;
}


int GameplayLogic::GetRandom(const int min, const int max) const
{
	return min + rand() % (max-min);
}

float GameplayLogic::GetRandom(const float min, const float max) const
{
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}
