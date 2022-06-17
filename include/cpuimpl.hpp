#pragma once
#include "emulator.hpp"
#include <iostream>
inline void State::dad(byte op) {
	auto dad_ = [this](byte rh, byte rl) {
		unsigned result = combineLH(rl, rh);
		result += registers.getHL();
		registers.l() = result & 0xFF;
		registers.h() = result >> 8;
		registers.flags.carry = result >> 16;
	};
	switch (op >> 4 & 0b11) {
		case 0:
			dad_(registers.b(), registers.c());
			break;
		case 1:
			dad_(registers.d(), registers.e());
			break;
		case 2:
			dad_(registers.h(), registers.l());
			break;
		case 3: {
			dad_(registers.sp >>8, registers.sp & 0xFF);
			break;
		}
	}
}
inline void State::dcr(byte& n) {
	std::cout << "decremented " << (int)n << '\n';
	--n;
	registers.flags.setMain(n);
}
inline void State::inx(byte op) {
	auto inx_ = [](byte& rh, byte& rl) {
		++rl;
		if (rl == 0) ++rh;
	};
	switch (op >> 4 & 0b11) {
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
			++registers.sp;
			break;
	}
}
inline bool State::jcnd(byte* op) {
	byte instr = op[0];
	bool pass;
	bool tgt = instr & 1;
	switch (instr >> 4 & 0b11) {
		case 0:
			pass = registers.flags.zero == tgt;
			break;
		case 1:
			pass = registers.flags.carry == tgt;
			break;
		case 2:
			pass = registers.flags.parity == tgt;
			break;
		case 3:
			pass = registers.flags.sign == tgt;
			break;
	}
	if (pass) jmp(op);
	return pass;
}
inline void State::jmp(byte* op) { registers.pc = op[1] | op[2] << 8; }
inline void State::mov(byte* op) {
	auto getLoc = [this](byte loc) -> byte* {
		using R = Registers::REGID;
		auto id = static_cast<R>(loc);
		if (id == R::ADDR) return memory.get() + registers.getHL();
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
inline void State::pop(byte op) {
	auto pop_ = [this](byte& rh, byte& rl) {
		rl = memory[registers.sp++];
		rh = memory[registers.sp++];
	};
	switch (op >> 4 & 0b11) {
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
			break;
	}
}
inline void State::push(byte op) {
	auto push_ = [this](byte& rh, byte& rl) {
		memory[--registers.sp] = rh;
		memory[--registers.sp] = rl;
	};
	switch (op >> 4 & 0b11) {
		case 0:
			push_(registers.b(), registers.c());
			break;
		case 1:
			push_(registers.d(), registers.e());
			break;
		case 2:
			push_(registers.h(), registers.l());
			break;
		case 3:
			break;
	}
}
