#pragma once
#include <initializer_list>

namespace OpUtils
{
	using byte = unsigned char;
#ifdef _MSC_VER
	inline int findParity(int n)
	{
		int b;
		b = n ^ (n >> 1);
		b = b ^ (b >> 2);
		b = b ^ (b >> 4);
		b = b ^ (b >> 8);
		b = b ^ (b >> 16);
		return (b & 1);
	}
#endif

	struct Time
	{
		byte min;
		byte max;
	};

	enum FlagType
	{
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

	struct BriefOpData
	{
		byte size;
		byte flags;
		Time time;
	};

	struct OpData
	{
		struct FullOpData
		{
			byte idx;
			const char* name;
			byte size;
			OpUtils::Time time;
			byte flags;
		};
		const char* names[256]{};
		BriefOpData data[256]{};
		constexpr OpData(std::initializer_list<FullOpData> list)
		{
			for (const auto& e : list)
			{
				names[e.idx] = e.name;
				data[e.idx] = { e.size, e.flags, e.time };
			}
		}
	};

	static constexpr OpData codes
	{
		{.idx = 0x00, .name = "NOP"         , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x01, .name = "LXI B,D16"   , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0x02, .name = "STAX B"      , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x03, .name = "INX B"       , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x04, .name = "INR B"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x05, .name = "DCR B"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x06, .name = "MVI B, D8"   , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x07, .name = "RLC"         , .size = 1, .time = { 4,  4}, .flags = CY           },
		{.idx = 0x08, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x09, .name = "DAD B"       , .size = 1, .time = {10, 10}, .flags = CY           },
		{.idx = 0x0A, .name = "LDAX B"      , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x0B, .name = "DCX B"       , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x0C, .name = "INR C"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x0D, .name = "DCR C"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x0E, .name = "MVI C,D8"    , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x0F, .name = "RRC"         , .size = 1, .time = { 4,  4}, .flags = CY           },
		{.idx = 0x10, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x11, .name = "LXI D,D16"   , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0x12, .name = "STAX D"      , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x13, .name = "INX D"       , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x14, .name = "INR D"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x15, .name = "DCR D"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x16, .name = "MVI D, D8"   , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x17, .name = "RAL"         , .size = 1, .time = { 4,  4}, .flags = CY           },
		{.idx = 0x18, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x19, .name = "DAD D"       , .size = 1, .time = {10, 10}, .flags = CY           },
		{.idx = 0x1A, .name = "LDAX D"      , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x1B, .name = "DCX D"       , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x1C, .name = "INR E"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x1D, .name = "DCR E"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x1E, .name = "MVI E,D8"    , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x1F, .name = "RAR"         , .size = 1, .time = { 4,  4}, .flags = CY           },
		{.idx = 0x20, .name = "RIM"         , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x21, .name = "LXI H,D16"   , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0x22, .name = "SHLD adr"    , .size = 3, .time = {16, 16}, .flags = NONE         },
		{.idx = 0x23, .name = "INX H"       , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x24, .name = "INR H"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x25, .name = "DCR H"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x26, .name = "MVI H,D8"    , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x27, .name = "DAA"         , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x28, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x29, .name = "DAD H"       , .size = 1, .time = {10, 10}, .flags = CY           },
		{.idx = 0x2A, .name = "LHLD adr"    , .size = 3, .time = {16, 16}, .flags = NONE         },
		{.idx = 0x2B, .name = "DCX H"       , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x2C, .name = "INR L"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x2D, .name = "DCR L"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x2E, .name = "MVI L, D8"   , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x2F, .name = "CMA"         , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x30, .name = "SIM"         , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x31, .name = "LXI SP, D16" , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0x32, .name = "STA adr"     , .size = 3, .time = {13, 13}, .flags = NONE         },
		{.idx = 0x33, .name = "INX SP"      , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x34, .name = "INR M"       , .size = 1, .time = {10, 10}, .flags = NOCY         },
		{.idx = 0x35, .name = "DCR M"       , .size = 1, .time = {10, 10}, .flags = NOCY         },
		{.idx = 0x36, .name = "MVI M,D8"    , .size = 2, .time = {10, 10}, .flags = NONE         },
		{.idx = 0x37, .name = "STC"         , .size = 1, .time = { 4,  4}, .flags = CY           },
		{.idx = 0x38, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x39, .name = "DAD SP"      , .size = 1, .time = {10, 10}, .flags = CY           },
		{.idx = 0x3A, .name = "LDA adr"     , .size = 3, .time = {13, 13}, .flags = NONE         },
		{.idx = 0x3B, .name = "DCX SP"      , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x3C, .name = "INR A"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x3D, .name = "DCR A"       , .size = 1, .time = { 5,  5}, .flags = NOCY         },
		{.idx = 0x3E, .name = "MVI A,D8"    , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x3F, .name = "CMC"         , .size = 1, .time = { 4,  4}, .flags = CY           },
		{.idx = 0x40, .name = "MOV B,B"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x41, .name = "MOV B,C"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x42, .name = "MOV B,D"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x43, .name = "MOV B,E"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x44, .name = "MOV B,H"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x45, .name = "MOV B,L"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x46, .name = "MOV B,M"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x47, .name = "MOV B,A"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x48, .name = "MOV C,B"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x49, .name = "MOV C,C"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x4A, .name = "MOV C,D"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x4B, .name = "MOV C,E"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x4C, .name = "MOV C,H"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x4D, .name = "MOV C,L"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x4E, .name = "MOV C,M"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x4F, .name = "MOV C,A"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x50, .name = "MOV D,B"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x51, .name = "MOV D,C"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x52, .name = "MOV D,D"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x53, .name = "MOV D,E"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x54, .name = "MOV D,H"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x55, .name = "MOV D,L"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x56, .name = "MOV D,M"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x57, .name = "MOV D,A"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x58, .name = "MOV E,B"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x59, .name = "MOV E,C"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x5A, .name = "MOV E,D"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x5B, .name = "MOV E,E"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x5C, .name = "MOV E,H"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x5D, .name = "MOV E,L"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x5E, .name = "MOV E,M"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x5F, .name = "MOV E,A"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x60, .name = "MOV H,B"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x61, .name = "MOV H,C"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x62, .name = "MOV H,D"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x63, .name = "MOV H,E"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x64, .name = "MOV H,H"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x65, .name = "MOV H,L"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x66, .name = "MOV H,M"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x67, .name = "MOV H,A"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x68, .name = "MOV L,B"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x69, .name = "MOV L,C"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x6A, .name = "MOV L,D"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x6B, .name = "MOV L,E"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x6C, .name = "MOV L,H"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x6D, .name = "MOV L,L"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x6E, .name = "MOV L,M"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x6F, .name = "MOV L,A"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x70, .name = "MOV M,B"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x71, .name = "MOV M,C"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x72, .name = "MOV M,D"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x73, .name = "MOV M,E"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x74, .name = "MOV M,H"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x75, .name = "MOV M,L"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x76, .name = "HLT"         , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x77, .name = "MOV M,A"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x78, .name = "MOV A,B"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x79, .name = "MOV A,C"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x7A, .name = "MOV A,D"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x7B, .name = "MOV A,E"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x7C, .name = "MOV A,H"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x7D, .name = "MOV A,L"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x7E, .name = "MOV A,M"     , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x7F, .name = "MOV A,A"     , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0x80, .name = "ADD B"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x81, .name = "ADD C"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x82, .name = "ADD D"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x83, .name = "ADD E"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x84, .name = "ADD H"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x85, .name = "ADD L"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x86, .name = "ADD M"       , .size = 1, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0x87, .name = "ADD A"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x88, .name = "ADC B"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x89, .name = "ADC C"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x8A, .name = "ADC D"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x8B, .name = "ADC E"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x8C, .name = "ADC H"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x8D, .name = "ADC L"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x8E, .name = "ADC M"       , .size = 1, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0x8F, .name = "ADC A"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x90, .name = "SUB B"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x91, .name = "SUB C"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x92, .name = "SUB D"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x93, .name = "SUB E"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x94, .name = "SUB H"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x95, .name = "SUB L"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x96, .name = "SUB M"       , .size = 1, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0x97, .name = "SUB A"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0x98, .name = "SBB B"       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x99, .name = "SBB C"       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x9A, .name = "SBB D"       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x9B, .name = "SBB E"       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x9C, .name = "SBB H"       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x9D, .name = "SBB L"       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0x9E, .name = "SBB M"       , .size = 1, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0x9F, .name = "SBB A"       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xA0, .name = "ANA B"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA1, .name = "ANA C"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA2, .name = "ANA D"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA3, .name = "ANA E"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA4, .name = "ANA H"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA5, .name = "ANA L"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA6, .name = "ANA M"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA7, .name = "ANA A"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA8, .name = "XRA B"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xA9, .name = "XRA C"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xAA, .name = "XRA D"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xAB, .name = "XRA E"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xAC, .name = "XRA H"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xAD, .name = "XRA L"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xAE, .name = "XRA M"       , .size = 1, .time = { 7,  7}, .flags = ALL | CLEARC },
		{.idx = 0xAF, .name = "XRA A"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB0, .name = "ORA B"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB1, .name = "ORA C"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB2, .name = "ORA D"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB3, .name = "ORA E"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB4, .name = "ORA H"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB5, .name = "ORA L"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB6, .name = "ORA M"       , .size = 1, .time = { 7,  7}, .flags = ALL | CLEARC },
		{.idx = 0xB7, .name = "ORA A"       , .size = 1, .time = { 4,  4}, .flags = ALL | CLEARC },
		{.idx = 0xB8, .name = "CMP B"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0xB9, .name = "CMP C"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0xBA, .name = "CMP D"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0xBB, .name = "CMP E"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0xBC, .name = "CMP H"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0xBD, .name = "CMP L"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0xBE, .name = "CMP M"       , .size = 1, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0xBF, .name = "CMP A"       , .size = 1, .time = { 4,  4}, .flags = ALL          },
		{.idx = 0xC0, .name = "RNZ"         , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xC1, .name = "POP B"       , .size = 1, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xC2, .name = "JNZ adr"     , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xC3, .name = "JMP adr"     , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xC4, .name = "CNZ adr"     , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xC5, .name = "PUSH B"      , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xC6, .name = "ADI D8"      , .size = 2, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0xC7, .name = "RST 0"       , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xC8, .name = "RZ"          , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xC9, .name = "RET"         , .size = 1, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xCA, .name = "JZ adr"      , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xCB, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xCC, .name = "CZ adr"      , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xCD, .name = "CALL adr"    , .size = 3, .time = {17, 17}, .flags = NONE         },
		{.idx = 0xCE, .name = "ACI D8"      , .size = 2, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0xCF, .name = "RST 1"       , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xD0, .name = "RNC"         , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xD1, .name = "POP D"       , .size = 1, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xD2, .name = "JNC adr"     , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xD3, .name = "OUT D8"      , .size = 2, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xD4, .name = "CNC adr"     , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xD5, .name = "PUSH D"      , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xD6, .name = "SUI D8"      , .size = 2, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0xD7, .name = "RST 2"       , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xD8, .name = "RC"          , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xD9, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xDA, .name = "JC adr"      , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xDB, .name = "IN D8"       , .size = 2, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xDC, .name = "CC adr"      , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xDD, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xDE, .name = "SBI D8"      , .size = 2, .time = { 7,  7}, .flags = NONE         },
		{.idx = 0xDF, .name = "RST 3"       , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xE0, .name = "RPO"         , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xE1, .name = "POP H"       , .size = 1, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xE2, .name = "JPO adr"     , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xE3, .name = "XTHL"        , .size = 1, .time = {18, 18}, .flags = NONE         },
		{.idx = 0xE4, .name = "CPO adr"     , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xE5, .name = "PUSH H"      , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xE6, .name = "ANI D8"      , .size = 2, .time = { 7,  7}, .flags = ALL | CLEARC },
		{.idx = 0xE7, .name = "RST 4"       , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xE8, .name = "RPE"         , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xE9, .name = "PCHL"        , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0xEA, .name = "JPE adr"     , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xEB, .name = "XCHG"        , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xEC, .name = "CPE adr"     , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xED, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xEE, .name = "XRI D8"      , .size = 2, .time = { 7,  7}, .flags = ALL | CLEARC },
		{.idx = 0xEF, .name = "RST 5"       , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xF0, .name = "RP"          , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xF1, .name = "POP PSW"     , .size = 1, .time = {10, 10}, .flags = ALL          },
		{.idx = 0xF2, .name = "JP adr"      , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xF3, .name = "DI"          , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xF4, .name = "CP adr"      , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xF5, .name = "PUSH PSW"    , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xF6, .name = "ORI D8"      , .size = 2, .time = { 7,  7}, .flags = ALL | CLEARC },
		{.idx = 0xF7, .name = "RST 6"       , .size = 1, .time = {11, 11}, .flags = NONE         },
		{.idx = 0xF8, .name = "RM"          , .size = 1, .time = { 5, 11}, .flags = NONE         },
		{.idx = 0xF9, .name = "SPHL"        , .size = 1, .time = { 5,  5}, .flags = NONE         },
		{.idx = 0xFA, .name = "JM adr"      , .size = 3, .time = {10, 10}, .flags = NONE         },
		{.idx = 0xFB, .name = "EI"          , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xFC, .name = "CM adr"      , .size = 3, .time = {11, 17}, .flags = NONE         },
		{.idx = 0xFD, .name = nullptr       , .size = 1, .time = { 4,  4}, .flags = NONE         },
		{.idx = 0xFE, .name = "CPI D8"      , .size = 2, .time = { 7,  7}, .flags = ALL          },
		{.idx = 0xFF, .name = "RST 7"       , .size = 1, .time = {11, 11}, .flags = NONE         },
	};

	int opSize(int opCode);
	BriefOpData opData(int opCode);
	const char* opName(int opCode);
	int disassembleOp(OpUtils::byte* codebuf, int pc);
}
