#pragma once
#include <emulator.hpp>


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
		ShiftDevice shiftDev;
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
			case 3:
				return shiftDev.read();
				break;
			default:
				//Unhandled port
				return 0;
				break;
		}
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
