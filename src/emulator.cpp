#include "emulator.hpp"
#include <cstring>
#include <fstream>
#include <functional>

State::State(): memory(std::make_unique_for_overwrite<byte[]>(RAMSize)) {}

void State::readBytes(const byte* stream, int size) {
	std::memcpy(memory.get(), stream, size);
}
void State::readProgram(const char* file) {
	std::ifstream inFile{file, inFile.binary};
	inFile.read((char*)memory.get(), RAMSize);
}
void dbreak() {}
void bcheck(OpUtils::byte b) {
	static OpUtils::byte opcodes[256]{};
	if (opcodes[b] == 0) {
		dbreak();
		opcodes[b] = 1;
	}
}
bool State::step() {
	byte* opline = memory.get() + registers.pc;
	OpUtils::disassembleOp(memory.get(), registers.pc);
	byte opCode = opline[0];
	OpUtils::BriefOpData opCodeInfo = OpUtils::opData(opCode);
	registers.pc += opCodeInfo.size;
	//if (registers.pc == 0x1A63) dbreak();
	[[maybe_unused]] bool altSpeed = false;
	switch (opCode) {
		case 0x00: //NOP
			break;
		case 0x01: //LXI B
			registers.c() = opline[1];
			registers.b() = opline[2];
			break;
		case 0x05: //DCR B
			dcr(registers.b());
			break;
		case 0x09: //DAD B
			registers.l() += registers.c();
			registers.h() += 0xFF - registers.c() > registers.l();
			registers.h() += registers.b();
			registers.flags.carry += 0xFF - registers.b() > registers.h();
			break;
		case 0x0D: //DCR C
			dcr(registers.c());
			break;
		case 0x11: //LXI D
			registers.e() = opline[1];
			registers.d() = opline[2];
			break;
		case 0x13: //INX D
			inx(opCode);
			break;
		case 0x1A: //LDAX D
			registers.a() = memory[combineLH(registers.e(), registers.d())];
			break;
		case 0x1F: { //RAR
			byte tmpBit = registers.a() & 1;
			registers.a() >>= 1;
			registers.a() &= registers.flags.carry << 7;
			registers.flags.carry = tmpBit;
			break;
		}
		case 0x21: //LXI H D16
			registers.l() = opline[1];
			registers.h() = opline[2];
			break;
		case 0x23: //INX H
			inx(opCode);
			break;
		case 0x29: //DAD H
			registers.l() += registers.l();
			registers.h() += 0xFF - registers.l() > registers.l();//this could overflow :(
			registers.h() += registers.h();
			registers.flags.carry += 0xFF - registers.h() > registers.h();
			break;
		case 0x31: //LXI SP
			registers.sp = combineLH(opline[1], opline[2]);
			break;
		case 0x76: //HLT
			return false;
		case 0xB6: //ORA M
			registers.flags.bitOpCheck(registers.a() |= memory[registers.getHL()]);
			break;
		case 0xC1: //POP B
			break;
		case 0xC2: //JNZ
			altSpeed = jcnd(opline);
			break;
		case 0xC3: //JMP
			jmp(opline);
			break;
		case 0xC5: //PUSH B
			push(opCode);
			break;
		case 0xCD: //CALL
			memory[--registers.sp] = registers.pc >> 8;
			memory[--registers.sp] = registers.pc & 0xFF;
			registers.pc = combineLH(opline[1], opline[2]);
			break;
		case 0xC9: { //RET
			auto& sp = registers.sp;
			registers.pc = combineLH(memory[sp], memory[sp + 1]);
			sp += 2;
			break;
		}
		case 0xD5: { //PUSH B
			push(opCode);
			break;
		}
		case 0xE1: //POP H
			pop(opCode);
			break;
		case 0xE5: //PUSH H
			push(opCode);
			break;
		case 0xE6: //ANI
			registers.flags.bitOpCheck(registers.a() &= opline[1]);
			break;
		case 0xF6: //ORI
			registers.flags.bitOpCheck(registers.a() |= opline[1]);
			break;
		case 0xFE: //CPI
			registers.a() = registers.flags.doSub(registers.a(), opline[1]);
			break;
		default: {
			constexpr byte MVIMask = ~0x38u & 0xFF;
			if (static_cast<byte>(opCode - 0x40) < 0x40u || (opCode & MVIMask) == 0b110) {
				mov(opline); //MOV & MVI
			} else {
				throw std::runtime_error{"Unimplemented instruction"};
			}
		}
	}

	//cycleCount += altSpeed ? opCodeInfo.time.max : opCodeInfo.time.min;
	return true;
}
