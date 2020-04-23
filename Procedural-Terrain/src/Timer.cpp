#include "Timer.h"

Timer::Timer(std::string functionName)
{
	FuncName = functionName;
	start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;

	float ms = duration.count() * 1000.0f;
	std::cout << FuncName << " function took: "<< ms << "ms\n ";
}