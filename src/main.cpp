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
