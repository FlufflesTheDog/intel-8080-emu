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
