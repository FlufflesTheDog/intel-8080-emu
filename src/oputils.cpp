#include "oputils.hpp"
#include <vector>
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include <fmt/core.h>

namespace OpUtils
{
	int opSize(int opCode) { return codes.data[opCode].size; }
	BriefOpData opData(int opCode) { return codes.data[opCode]; }
	const char* opName(int opCode) { return codes.names[opCode]; }

	int disassembleOp(byte* codebuf, int pc)
	{
		byte* code = codebuf + pc;
		int opbytes = 1;
		fmt::print("0x{:04X} ", pc);

		const BriefOpData& op = codes.data[*code];
		auto name = opName(*code);
		fmt::print("{:02X} ", *code);
		if (name)
			fmt::print("{:12} ", name);
		else
			fmt::print("? 0x{:02X}", *code);
		std::string operands;
		for (int i = op.size - 1; i > 0; --i)
		{
			operands += fmt::format("{:02X} ", code[i]);
		}
		fmt::print("{:9}f:{}\n", operands, op.flags);
		return opbytes;
	}
}
