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
	bool running = emulator.IsValid();
	while (running)
	{
		running = emulator.StepOpCode();
	}
}
