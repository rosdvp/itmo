#pragma once
#include <string>

#include "Model.h"

class Game
{
public:
	void Run();

private:
	Model* TryLoadModel();
	void SaveModel(Model* model);
	void DeleteSaveFile();
	std::string GetSaveFilePath();
};
