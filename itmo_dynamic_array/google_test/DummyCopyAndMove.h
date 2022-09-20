#pragma once
#include <algorithm>
#include <cstdlib>

class DummyCopyAndMove
{
public:
	enum ECreationType;

	int Val;
	ECreationType CreationType;


	DummyCopyAndMove(const int val) : Val(val), CreationType(Construct) { }

	DummyCopyAndMove(const DummyCopyAndMove& other) : Val(other.Val), CreationType(CopyConstruct) { }

	DummyCopyAndMove(DummyCopyAndMove&& other) noexcept : Val(other.Val), CreationType(MoveConstruct) { }

	~DummyCopyAndMove() = default;

	DummyCopyAndMove& operator=(const DummyCopyAndMove& other)
	{
		Val = other.Val;
		CreationType = CopyAssign;
		return *this;
	}

	DummyCopyAndMove& operator=(DummyCopyAndMove&& other) noexcept
	{
		Val = other.Val;
		CreationType = MoveAssign;
		return *this;
	}


	enum ECreationType
	{
		Construct,
		CopyConstruct,
		CopyAssign,
		MoveConstruct,
		MoveAssign
	};
};