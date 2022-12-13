#pragma once

struct Dummy8
{
	int Data[2];
};

struct Dummy16
{
	int Data[4];
};

struct Dummy24
{
	int Data[6];
};

struct Dummy32
{
	int Data[8];
};

static constexpr size_t SIZE_D8 = sizeof(Dummy8);
static constexpr size_t SIZE_D16 = sizeof(Dummy16);
static constexpr size_t SIZE_D24 = sizeof(Dummy24);
static constexpr size_t SIZE_D32 = sizeof(Dummy32);