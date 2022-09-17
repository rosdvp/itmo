#include "View.h"

#include <iostream>

#include "EGameResult.h"
#include "Model.h"


bool View::RequestStartNewGameDecision() const
{
	std::cout << "Повелитель, желаешь вернуться к старым рабам? (y/n) ";
	return !InputYesOrNo();
}

bool View::RequestRestartDecision() const
{
	std::cout << "Повелитель, желаешь найти новых рабов? (y/n) ";
	return InputYesOrNo();
}


void View::ShowFirstRoundHeader(const Model* model) const
{
	std::cout << "Мой повелитель, начинается твое правление\n"
	<< "\tНаселение города сейчас составляет " << model->Persons << " человек;\n"
	<< "\tНаш город занимает " << model->Grounds << " акров;\n"
	<< "\tВ наших абмарах " << model->Wheat << " бушелей пшеницы;\n"
	<< "\t1 акр земли стоит сейчас " << model->GroundCostWheat << " бушель." << std::endl;
}

void View::ShowRoundHeader(const Model* model) const
{
	std::cout << "Мой повелитель, соизволь поведать тебе\n"
	<< "\tв году " << model->Round+1 << " твоего высочайшего правления\n"
	<< "\t" << model->DiedPersons << " человек умерли с голоду, и " << model->ArrivedPersons
	<< " прибыли в наш великий город;\n";

	if (model->IsPlague)
		std::cout << "\tЧума уничтожила половину населения;\n";

	std::cout << "\tНаселение города сейчас составляет " << model->Persons << " человек;\n"
	<< "\tМы собрали " << model->SummaryWheatFromGrounds << " бушелей пшеницы, по "
	<< model->WheatFromEachGround << " бушеля с акра;\n"
	<< "\tКрысы истребили " << model->WheatSpentAtRats << " бушелей пшеницы, оставив "
	<< model->Wheat << " бушеля в амбарах;\n"
	<< "\tГород сейчас занимает " << model->Grounds << " акров;\n"
	<< "\t1 акр земли стоит сейчас " << model->GroundCostWheat << " бушель." << std::endl;
}

void View::ShowRequestHeader() const
{
	std::cout << "Что пожелаешь, повелитель?" << std::endl;
}

int View::RequestGroundsDelta() const
{
	std::cout << "Сколько акров земли повелеваешь купить(>0) или продать(<0)? ";
	return InputInt();
}

int View::RequestWheatForPersons() const
{
	std::cout << "Сколько бушелей пшеницы повелеваешь съесть? ";
	return InputInt();
}

int View::RequestGroundsToCultivate() const
{
	std::cout << "Сколько акров земли повелеваешь засеять? ";
	return InputInt();
}


void View::ShowInvalidInput(EInvalidReason reason) const
{
	std::cout << "Сжалься, повелитель! ";
	switch (reason)
	{
	case GroundsDeltaNotEnoughWheat:
		std::cout << "У нас не хватит пшеницы, чтобы купить столько акров!";
		break;
	case GroundsDeltaNotEnoughGrounds:
		std::cout << "Мы не сможем продать больше акров, чем у нас есть!";
		break;
	case WheatForPersonsNotEnough:
		std::cout << "У нас нет столько пшеницы!";
		break;
	case WheatForPersonsBelowZero:
		std::cout << "Твои рабы не могут есть наоборот!";
		break;
	case GroundsToCultivateNotEnoughGrounds:
		std::cout << "У нас нет стольки акров!";
		break;
	case GroundsToCultivateNotEnoughPersons:
		std::cout << "Нам не хватит рабов на столько акров!";
		break;
	case GroundsToCultivateNotEnoughWheat:
		std::cout << "Нам не хватит пшеницы для посева!";
		break;
	case GroundsToCultivateBelowZero:
		std::cout << "Твои рабы не знаю как сеять в обратную сторону!";
		break;
	default:
		throw std::out_of_range("EInvalidReason");
	}
	std::cout << std::endl << std::endl;
}

void View::ShowGameResult(const EGameResult result, const float avgDeathRate, const float groundsPerPerson) const
{
	std::cout << "По итогам вашего правления..\n"
		<< "В среднем в год умирало " << static_cast<int>(avgDeathRate * 100) << "% населения\n"
		<< "На одного жителя приходится " << groundsPerPerson << " акров земли" << std::endl;
	switch (result)
	{
	case Bad:
		std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города."
			"Теперь вы вынуждены влачить жалкое существование в изгнании.";
		break;
	case Satisfactory:
		std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. "
			"Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем.";
		break;
	case Good:
		std::cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, "
			"но многие хотели бы увидеть вас во главе города снова.";
		break;
	case Perfect:
		std::cout << "«Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше.";
		break;
	default:
		throw std::out_of_range("EGameResult");
	}
	std::cout << std::endl;
}

bool View::InputYesOrNo() const
{
	while (true)
	{
		char input;
		std::cin >> input;
		if (input == 'y')
			return true;
		if (input == 'n')
			return false;
		std::cin.clear();
		std::cout << "О Великий, я знаю только буквы 'y' и 'n'. Прошу, выбери из них: ";
	}
}

int View::InputInt() const
{
	while (true)
	{
		int input;
		std::cin >> input;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cout << "О Великий, я не понимаю. Скажи целое число: ";
		}
		else
			return input;
	}
}

