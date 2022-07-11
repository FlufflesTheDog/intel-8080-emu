#pragma once
#include "emulator.hpp"
inline void State::dad(byte op)
{
	auto dad_ = [this](byte rh, byte rl) {
		unsigned result = combineLH(rl, rh);
		result += registers.GetHL();
		registers.l() = result & 0xFF;
		registers.h() = result >> 8;
		registers.flags.Carry = result >> 16;
	};

	switch (op >> 4 & 0b11)
	{
		case 0:
			dad_(registers.b(), registers.c());
			break;
		case 1:
			dad_(registers.d(), registers.e());
			break;
		case 2:
			dad_(registers.h(), registers.l());
			break;
		case 3:
			dad_(registers.SP >> 8, registers.SP & 0xFF);
			break;
	}
}

inline void State::dcr(byte& n)
{
	--n;
	registers.flags.SetMainFlags(n);
}

inline void State::inx(byte op)
{
	auto inx_ = [](byte& rh, byte& rl) {
		++rl;
		if (rl == 0) ++rh;
	};
	switch (op >> 4 & 0b11)
	{
		case 0:
			inx_(registers.b(), registers.c());
			break;
		case 1:
			inx_(registers.d(), registers.e());
			break;
		case 2:
			inx_(registers.h(), registers.l());
			break;
		case 3:
			++registers.SP;
			break;
	}
}
inline bool State::jcnd(byte* op)
{
	byte instr = op[0];
	bool pass = false;
	bool tgt = instr & 1;
	switch ((instr >> 4) & 0b11)
	{
		case 0:
			pass = registers.flags.Zero == tgt;
			break;
		case 1:
			pass = registers.flags.Carry == tgt;
			break;
		case 2:
			pass = registers.flags.Parity == tgt;
			break;
		case 3:
			pass = registers.flags.Sign == tgt;
			break;
	}
	if (pass) { jmp(op); }
	return pass;
}

inline void State::jmp(byte* op) { registers.PC = op[1] | op[2] << 8; }

inline void State::mov(byte* op)
{
	auto getLoc = [this](byte loc) -> byte* {
		using R = Registers::REG_ID;
		auto id = static_cast<R>(loc);
		if (id == R::ADDR) return Memory.get() + registers.GetHL();
		return &registers[id];
	};
	byte opcode = *op;

	byte* src;
	if ((opcode & 0xC0) == 0) //immediate
		src = op + 1;
	else
		src = getLoc(opcode >> 0 & 0b111);
	byte* dst = getLoc(opcode >> 3 & 0b111);
	*dst = *src;
}

inline void State::pop(byte op)
{
	auto pop_ = [this](byte& rh, byte& rl) {
		rl = Memory[registers.SP++];
		rh = Memory[registers.SP++];
	};
	switch (op >> 4 & 0b11)
	{
		case 0:
			pop_(registers.b(), registers.c());
			break;
		case 1:
			pop_(registers.d(), registers.e());
			break;
		case 2:
			pop_(registers.h(), registers.l());
			break;
		case 3:
		{ //PSW
			byte flagByte;
			pop_(registers.a(), flagByte);
			registers.byteToFlags(flagByte);
			break;
		}
	}
}

inline void State::push(byte op)
{
	auto push_ = [this](byte rh, byte rl) {
		Memory[--registers.SP] = rh;
		Memory[--registers.SP] = rl;
	};
	switch (op >> 4 & 0b11)
	{
		case 0:
			push_(registers.b(), registers.c());
			break;
		case 1:
			push_(registers.d(), registers.e());
			break;
		case 2:
			push_(registers.h(), registers.l());
			break;
		case 3: //PSW
			push_(registers.a(), registers.flagsToByte());
			break;
	}
}

inline void State::rotateLeft(bool carry) {
	uint8_t newCarry = registers.a() >> 7;
	uint8_t lowBit = carry ? registers.flags.Carry : newCarry;
	registers.a() = registers.a() << 1 & lowBit;
	registers.flags.Carry = newCarry;
}
inline void State::rotateRight(bool carry) {
	uint8_t newCarry = registers.a() << 7 & 1;
	uint8_t highBit = carry ? registers.flags.Carry : newCarry;
	highBit <<= 7;
	registers.a() = registers.a() >> 1 & highBit;
	registers.flags.Carry = newCarry;
}
