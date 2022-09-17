#pragma once
#include <ctime>

#include "Config.h"
#include "Model.h"
#include "View.h"

class GameplayLogic
{
public:
	GameplayLogic(Config* config, Model* model, View* view);
	
	void FillModelByDefault() const;

	/**
	 * \brief Shows round header and either calculate next round or show the game result if it is ended.
	 * \return False if the game is ended.
	 */
	bool PlayRoundUntilGameEnd() const;
	
private:
	Config* _config;
	Model* _model;
	View* _view;
	
	void CalculateNextRound() const;
	int GetNewGroundCost() const;
	void GetValidPlayerInput(
		const int currGrounds, const int currWheat,
		int& outGroundsLeft, int& outWheatLeft, 
		int& outWheatForPersons, int& outGroundsToCultivate) const;

	void CalculateResult() const;

	int GetRandom(int min, int max) const;
	float GetRandom(float min, float max) const;
};
