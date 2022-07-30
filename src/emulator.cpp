#include "emulator.hpp"
#include <cstring>
#include <fstream>

State::State(const char* fileName)
{
	ReadProgram(fileName);
}

void State::ReadBytes(const byte* stream, int size)
{
	if (!Memory)
	{
		Memory = std::make_unique_for_overwrite<byte[]>(RAMSize);
	}
	std::memcpy(Memory.get(), stream, size);
}

void State::ReadProgram(const char* fileName)
{
	std::ifstream inFile{ fileName, inFile.binary };
	if (!inFile.is_open()) return;
	if (!Memory)
	{
		Memory = std::make_unique_for_overwrite<byte[]>(RAMSize);
	}
	inFile.read((char*)Memory.get(), RAMSize);
}

bool State::IsValid()
{
	return Memory != nullptr;
}

bool State::StepOpCode()
{
	byte* opline = Memory.get() + registers.PC;
	OpUtils::disassembleOp(Memory.get(), registers.PC);
	byte opCode = opline[0];
	OpUtils::BriefOpData opCodeInfo = OpUtils::opData(opCode);
	registers.PC += opCodeInfo.size;
	[[maybe_unused]] bool altSpeed = false;
	switch (opCode)
	{
		case 0x00: //NOP
			break;
		case 0x01: //LXI B
			registers.c() = opline[1];
			registers.b() = opline[2];
			break;
		case 0x05: //DCR B
			dcr(registers.b());
			break;
		case 0x06: //MVI B, D8
			mov(opline);
			break;
		case 0x09: //DAD B
			dad(opCode);
			break;
		case 0x0D: //DCR C
			dcr(registers.c());
			break;
		case 0x0e: //MVI C, D8
			mov(opline);
			break;
		case 0x0f: //RRC
			rotateRight();
			break;
		case 0x11: //LXI D, D16
			registers.e() = opline[1];
			registers.d() = opline[2];
			break;
		case 0x13: //INX D
			inx(opCode);
			break;
		case 0x19: //DAD D
			dad(opCode);
			break;
		case 0x1A: //LDAX D
			registers.a() = Memory[combineLH(registers.e(), registers.d())];
			break;
		case 0x1F: //RAR
			rotateRight(true);
			break;
		case 0x21: //LXI H D16
			registers.l() = opline[1];
			registers.h() = opline[2];
			break;
		case 0x23: //INX H
			inx(opCode);
			break;
		case 0x26: // MVI H, D8
			mov(opline);
			break;
		case 0x29: //DAD H
			dad(opCode);
			break;
		case 0x31: //LXI SP
			registers.SP = combineLH(opline[1], opline[2]);
			break;
		case 0x32: //STA adr
			/*
			* STA Store Accumulator Direct
				Description: The contents of the accumulator replace
				the byte at the memory address formed by concatenating
				HI ADD with LOW ADD.
			*/
			Memory[combineLH(opline[1], opline[2])] = registers.a();
			break;
		case 0x36: //MVI M,D8
			mov(opline);
			break;
		case 0x3a: //LDA adr
			registers.a() = Memory[combineLH(opline[1], opline[2])];
			break;
		case 0x3e: //MVI A,D8
			mov(opline);
			break;
		case 0x56: //MOV D,M
			mov(opline);
			break;
		case 0x5e: //MOV E,M
			mov(opline);
			break;
		case 0x66: //MOV H,M
			mov(opline);
			break;
		case 0x6F: //MOV L,A
			mov(opline);
			break;
		case 0x76: //HLT
			return false;
		case 0x77: //MOV M, A
			mov(opline);
			break;
		case 0x7A: //MOV A, D
			mov(opline);
			break;
		case 0x7B: //MOV A, E
			mov(opline);
			break;
		case 0x7C: //MOV A, H
			mov(opline);
			break;
		case 0x7E: //MOV A, M
			mov(opline);
			break;
		case 0xA7: //ANA A
			registers.flags.bitOpCheck(registers.a() &= registers.a());
			break;
		case 0xAF: //XRA A
			registers.flags.bitOpCheck(registers.a() ^= registers.a());
			break;
		case 0xB6: //ORA M
			registers.flags.bitOpCheck(registers.a() |= Memory[registers.GetHL()]);
			break;
		case 0xC1: //POP B
			pop(opCode);
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
		case 0xC6: //ADI D8
			registers.a() = registers.flags.DoAddition(registers.a(), opline[1]);
			break;
		case 0xC9: //RET
			registers.PC = combineLH(Memory[registers.SP], Memory[registers.SP + 1]);
			registers.SP += 2;
			break;
		case 0xCD: //CALL
			Memory[--registers.SP] = registers.PC >> 8;
			Memory[--registers.SP] = registers.PC & 0xFF;
			registers.PC = combineLH(opline[1], opline[2]);
			break;
		case 0xD1: //POP D
			pop(opCode);
			break;
		case 0xD3: // OUT
			devices->OUT(opline[1], registers.a());
			break;
		case 0xD5: //PUSH D
			push(opCode);
			break;
		case 0xDB: // IN
			registers.a() = devices->IN(opline[1]);
			break;
		case 0xE1: //POP H
			pop(opCode);
			break;
		case 0xE5: //PUSH H
			push(opCode);
			break;
		case 0xE6: //ANI
			registers.flags.bitOpCheck(registers.a() &= opline[1]);
			break;
		case 0xEB: //XCHG
			std::swap(registers.h(), registers.d());
			std::swap(registers.l(), registers.e());
			break;
		case 0xF1: //POP PSW
			pop(opCode);
			break;
		case 0xF5: //PUSH PSW
			push(opCode);
			break;
		case 0xF6: //ORI
			registers.flags.bitOpCheck(registers.a() |= opline[1]);
			break;
		case 0xFB: //EI
			//Would be more accurate to delay enabling
			//until end of *next* instruction, unsure if important
			registers.int_enable() = 1;
			break;
		case 0xFE: //CPI
			registers.flags.DoSubtraction(registers.a(), opline[1]);
			break;
		default:
			throw std::runtime_error{ "Unimplemented instruction" };
	}
	//cycleCount += altSpeed ? opCodeInfo.time.max : opCodeInfo.time.min;
	return true;
}
