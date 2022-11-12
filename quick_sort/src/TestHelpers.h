#pragma once
#include<iostream>
#include <string>

template<typename T>
void PrintArray(const std::string& prefix, const T* arr, const int count)
{
	std::cout << prefix << ": ";
	for (int i = 0; i < count; i++)
		std::cout << arr[i] << ", ";
	std::cout << std::endl;
}

inline void FillIntArrayRandomly(int* arr, const int count)
{
	for (int i = 0; i < count; i++)
		arr[i] = rand() % 10;
}

inline void FillIntArrayBackwards(int* arr, const int count)
{
    for (int i = 0; i < count; i++)
        arr[i] = count - i;
}


inline void FillStringArrayRandomly(std::string* arr, const int count, int minLength, int maxLength)
{
	char letterFirst = 'A';
	char letterLast = 'z';
	int lettersCount = letterLast - letterFirst;

	for (int i = 0; i < count; i++)
	{
		int length = minLength == maxLength ? minLength : minLength + rand() % (maxLength - minLength + 1);

		std::string str;
		for (int j = 0; j < length; j++)
			str += static_cast<char>(letterFirst + rand() % lettersCount);
		arr[i] = str;
	}
}


template<typename T, typename FunCreate>
void FillArrayRandomly(T* arr, const int count, FunCreate funCreate)
{
	for (int i = 0; i < count; i++)
		arr[i] = funCreate();
}