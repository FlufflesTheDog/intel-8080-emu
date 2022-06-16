#pragma once
namespace OpUtils {
using byte = unsigned char;

struct Time {
	byte min;
	byte max;
};

enum FlagType {
	NONE = 0,
	Z = 1 << 0,
	S = 1 << 1,
	P = 1 << 2,
	CY = 1 << 3,
	AC = 1 << 4,
	CLEARC = 1 << 5, //clears CY / AC
	NOCY = Z | S | P | AC,
	ALL = Z | S | P | CY | AC,
};
struct BriefOpData {
	byte size;
	byte flags;
	Time time;
};

int opSize(int opCode);
BriefOpData opData(int opCode);
const char* opName(int opCode);
int disassembleOp(OpUtils::byte* codebuf, int pc);
} //namespace OpUtils
