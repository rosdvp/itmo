#include <iostream>
#include <string>

#include "TestFunctions.h"

int main()
{
	//TestFSA();
	//TestCoalesceAllocator();
	//TestManagerIterative(10, 8, 600, 1, 10);
	TestManagerArrays(10, 5, 10, std::vector<int>{ 10, 50, 200, 500});
	//TestManagerDestroyWithBusyBlocks();
}
