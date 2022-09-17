#include "Game.h"
#include "Windows.h"
#include "GameplayLogic.h"
#include "Model.h"
#include "View.h"

#include <fstream>
#include <iostream>

void Game::Run()
{
	Config config;
	View view;
	Model* model = TryLoadModel();

	while (true) 
	{
		const bool isModelNew = model == nullptr || view.RequestStartNewGameDecision();
		if (isModelNew)
		{
			delete model;
			model = new Model();
		}
		
		GameplayLogic gameplay(&config, model, &view);

		if (isModelNew)
		{
			gameplay.FillModelByDefault();
			SaveModel(model);
		}

		bool isGameGoing = true;
		while (isGameGoing)
		{
			system("cls");
			isGameGoing = gameplay.PlayRoundUntilGameEnd();
			SaveModel(model);
		}

		delete model;
		model = nullptr;
		if (view.RequestRestartDecision())
			DeleteSaveFile();
		else
			break;
	}
}

Model* Game::TryLoadModel()
{
	const auto modelBuffer      = new char[sizeof(Model)];
	const auto vectorSizeBuffer = new char[sizeof(size_t)];
	char* vectorDataBuffer      = nullptr;
	
	std::ifstream file(GetSaveFilePath());
	try
	{
		if (!file.is_open())
			throw std::exception("save file wasn't found, perhaps");

		file.read(modelBuffer, sizeof(Model));
		const auto model = reinterpret_cast<Model*>(modelBuffer);
		model->RoundsDeathRates = new std::vector<float>();

		file.read(vectorSizeBuffer, sizeof(size_t));
		const auto vectorSize = *reinterpret_cast<size_t*>(vectorSizeBuffer);
		
		if (vectorSize > 0)
		{
			const size_t vectorDataSize = sizeof(float) * vectorSize;
			vectorDataBuffer = new char[vectorDataSize];
			file.read(vectorDataBuffer, vectorDataSize);
			const auto vectorData = reinterpret_cast<float*>(vectorDataBuffer);
			for (size_t i = 0; i < vectorSize; i++)
				model->RoundsDeathRates->push_back(vectorData[i]);
		}

		file.close();
		return model;
	}
	catch (std::exception& e)
	{
		std::cout << "Failed to load game: " << e.what() << std::endl;
		system("pause");
	}
	delete[] modelBuffer;
	delete[] vectorSizeBuffer;
	delete[] vectorDataBuffer;
	file.close();
	return nullptr;
}

void Game::SaveModel(Model* model)
{
	std::ofstream file(GetSaveFilePath());
	try
	{
		file.write(reinterpret_cast<char*>(model), sizeof(Model));
		size_t vectorSize = model->RoundsDeathRates->size();
		file.write(reinterpret_cast<char*>(&vectorSize), sizeof(size_t));
		if (vectorSize > 0)
			file.write(reinterpret_cast<char*>(model->RoundsDeathRates->data()), vectorSize * sizeof(float));
	}
	catch (std::exception& e)
	{
		std::cout << "Failed to save game: " << e.what() << std::endl;
		system("pause");
	}
	file.close();
}

void Game::DeleteSaveFile()
{
	std::remove(GetSaveFilePath().data());
}

std::string Game::GetSaveFilePath()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	auto path = std::string(buffer);
	const auto pos = path.find_last_of("\\/");
	path = path.substr(0, pos);
	path += "\\/save.bin";
	return path;
}


