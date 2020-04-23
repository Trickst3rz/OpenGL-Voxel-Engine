#pragma once
#include <chrono>
#include <iostream>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	std::string FuncName;
public:
	Timer(std::string functionName);

	~Timer();

};
