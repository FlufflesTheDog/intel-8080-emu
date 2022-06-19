#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include "oputils.hpp"
#include "emulator.hpp"

int main()
{
	State emulator("invaders/invaders");
	bool running = true;
	do
	{
		running = emulator.StepOpCode();
	} while (running);
}

// Program Tester
std::vector<OpUtils::byte> testProgram()
{
	std::vector<OpUtils::byte> ret;
	ret.push_back(0x00);
	//set pointer to write to
	ret.insert(ret.end(), { 0x2E, 0x0 }); //MVI L
	ret.insert(ret.end(), { 0x26, 0x40 });//MVI H

	for (int i = 0; i < 256; ++i)
	{
		ret.push_back(0x36); //MVI M
		ret.push_back(i);
		ret.push_back(0x2c); //INR L, write to next position
	}
	ret.push_back(0x76);
	return ret;
}
