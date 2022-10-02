#pragma once
#include <iostream>

class Dummy
{
public:
	int Power;
	int Value;

	Dummy(const int power, const int value) : Power(power), Value(value) { }

	Dummy(Dummy&& other) noexcept : Power(other.Power), Value(other.Value) { }

	Dummy& operator=(Dummy&& other) noexcept
	{
		Power = other.Power;
		Value = other.Value;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const Dummy& dummy)
	{
		os << "(" << dummy.Power << ", " << dummy.Value << ")";
		return os;
	}
};