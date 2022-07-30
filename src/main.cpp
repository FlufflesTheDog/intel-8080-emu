#include "emulator.hpp"
#include "ioport.hpp"

int main()
{
	State emulator("invaders/invaders");
	emulator.devices = std::make_unique<Devices::SpaceInvadersIO>();
	bool running = emulator.IsValid();
	while (running)
	{
		running = emulator.StepOpCode();
	}
}
