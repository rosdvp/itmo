#pragma once
#include "pch.h"
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

template<typename T, typename FunCreate>
void FillArrayRandomly(T* arr, const int count, FunCreate funCreate)
{
	for (int i = 0; i < count; i++)
		arr[i] = funCreate();
}