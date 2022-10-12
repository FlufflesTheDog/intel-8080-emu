#pragma once
#include <emulator.hpp>
#include <SDL2/SDL_scancode.h>


namespace Devices
{
	using byte = OpUtils::byte;
	struct ShiftDevice
	{
		std::uint16_t shiftRegister;
		byte offset;
		void setOffset(byte n);
		void write(byte n);
		byte read();
	};

	struct SpaceInvadersIO final: State::IODeviceHandler
	{
		byte port1;
		byte port2;
		ShiftDevice shiftDev;
		enum class Button {
			CREDIT = 0,
			P1_start = 2,
			P1_shoot = 4,
			P1_left = 5,
			P1_right = 6,
			P2_start = 1,
			P2_shoot = 4 << 8,
			P2_left = 5 << 8,
			P2_right = 6 << 8,

		};
		void pressButton(Button b);
		void releaseButton(Button b);
		byte IN(byte port) override;
		void OUT(byte port, byte data) override;
	};



	inline void ShiftDevice::setOffset(byte n)
	{
		offset = n;
	}

	inline void ShiftDevice::write(byte n)
	{
		shiftRegister >>= 8;
		shiftRegister |= n << 8;
	}

	inline byte ShiftDevice::read()
	{
		return shiftRegister >> (8 - offset);
	}

	inline byte SpaceInvadersIO::IN(byte port)
	{
		switch (port)
		{
			case 0:
				return 0;
			case 1:
				return port1;
			case 2:
				return port2;
			case 3:
				return shiftDev.read();
				break;
			default:
				//Unhandled port
				return 0;
				break;
		}
	}

	inline void SpaceInvadersIO::pressButton(Button b)
	{
		auto inputBit = static_cast<int>(b);
		auto setBit = [&](byte& port) {
			port |= 1 << (inputBit & 0xFF);
		};
		setBit(inputBit < 0xFF ? port1 : port2);
	}

	inline void SpaceInvadersIO::releaseButton(Button b)
	{
		auto inputBit = static_cast<int>(b);
		auto clearBit = [&](byte& port) {
			port &= ~(1 << (inputBit & 0xFF));
		};
		clearBit(inputBit < 0xFF ? port1 : port2);
	}

	inline void SpaceInvadersIO::OUT(byte port, byte data)
	{
		switch (port)
		{
			case 2:
				shiftDev.setOffset(data);
				break;
			case 4:
				shiftDev.write(data);
				break;
			default:
				//Unhandled port
				return;
		}
	}
}
