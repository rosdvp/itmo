#pragma once
#include "EGameResult.h"

class Model;

class View
{
public:
	bool RequestStartNewGameDecision() const;
	bool RequestRestartDecision() const;

	void ShowFirstRoundHeader(const Model* model) const;
	void ShowRoundHeader(const Model* model) const;
	void ShowRequestHeader() const;
	int RequestGroundsDelta() const;
	int RequestWheatForPersons() const;
	int RequestGroundsToCultivate() const;
	enum EInvalidReason;  // NOLINT(clang-diagnostic-microsoft-enum-forward-reference)
	void ShowInvalidInput(EInvalidReason reason) const;

	void ShowGameResult(EGameResult result, float avgDeathRate, float groundsPerPerson) const;

	enum EInvalidReason
	{
		GroundsDeltaNotEnoughWheat,
		GroundsDeltaNotEnoughGrounds,
		WheatForPersonsNotEnough,
		WheatForPersonsBelowZero,
		GroundsToCultivateNotEnoughGrounds,
		GroundsToCultivateNotEnoughPersons,
		GroundsToCultivateNotEnoughWheat,
		GroundsToCultivateBelowZero,
	};

private:
	bool InputYesOrNo() const;
	int InputInt() const;
};
