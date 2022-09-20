#pragma once
#include <algorithm>
#include <cstdlib>

class DummyCopyOnly
{
public:
	int Val;
	bool IsAssignUsed;
	bool IsDestructed;

	DummyCopyOnly(const int val)
		: Val(val), IsAssignUsed(false), IsDestructed(false) { }

	DummyCopyOnly(const DummyCopyOnly& other)
		: Val(other.Val), IsAssignUsed(false), IsDestructed(other.IsDestructed) { }

	DummyCopyOnly& operator=(const DummyCopyOnly& other)
	{
		Val = other.Val;
		IsAssignUsed = true;
		IsDestructed = false;
		return *this;
	}
	
	~DummyCopyOnly() { IsDestructed = true; }

	DummyCopyOnly(DummyCopyOnly&& other) = delete;
	DummyCopyOnly& operator=(DummyCopyOnly&& other) = delete;
};