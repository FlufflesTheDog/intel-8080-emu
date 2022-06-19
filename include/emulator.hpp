#pragma once
#include "oputils.hpp"
#include <cstdint>
#include <memory>

#ifdef _MSC_VER
#  define __builtin_parity OpUtils::findParity
#endif

struct Flags {
	using byte = OpUtils::byte;
	uint8_t zero: 1;
	uint8_t sign: 1;
	uint8_t parity: 1;
	uint8_t carry: 1;
	uint8_t auxCarry: 1; //Unimplemented for Space Invaders

	void setMain(byte n) {
		zero = n == 0;
		sign = n & 0x80;
		parity = __builtin_parity(n) == 0;
	}

	byte doSub(byte l, byte r, bool withBorrow = false) {
		unsigned result = l - r;
		result -= withBorrow && carry;
		carry = result >> 8;
		setMain(result);
		return result;
	}

	byte doAdd(byte l, byte r, bool withCarry = false) {
		unsigned result = l + r;
		result += withCarry && carry;
		carry = result >> 8;
		setMain(result);
		return result;
	}

	void bitOpCheck(byte n) {
		setMain(n);
		carry = 0;
	}
};

struct Registers {
	enum class REGID {
		B = 0,
		C = 1,
		D = 2,
		E = 3,
		H = 4,
		L = 5,
		ADDR = 6,
		A = 7,
	};
	Flags flags;
	uint8_t r[8];
	uint16_t sp;
	uint16_t pc;
	constexpr uint8_t& a();
	constexpr uint8_t& b();
	constexpr uint8_t& c();
	constexpr uint8_t& d();
	constexpr uint8_t& e();
	constexpr uint8_t& h();
	constexpr uint8_t& l();
	constexpr uint8_t& int_enable();
	constexpr void setSPLow(uint8_t l);
	constexpr void setSPHigh(uint8_t h);
	uint16_t getHL();
	constexpr uint8_t& operator[](REGID i);
	constexpr uint8_t operator[](REGID i) const;
};

class State {
	using byte = OpUtils::byte;
	static constexpr int RAMSize = 0xFFFF + 1;
	std::unique_ptr<byte[]> memory;
	Registers registers{};
	uint16_t readAddr(const byte* mem) const;
	void dad(byte op);
	void dcr(byte& n);
	void inx(byte op);
	bool jcnd(byte* op);
	void jmp(byte* op);
	void mov(byte* op);
	void pop(byte op);
	void push(byte op);

public:
	State();
	void readProgram(const char* file);
	void readBytes(const byte* stream, int size);
	bool step();
};

inline constexpr uint16_t combineLH(OpUtils::byte low, OpUtils::byte high) {
	return low | high << 8;
}

constexpr uint8_t& Registers::a() { return operator[](REGID::A); }
constexpr uint8_t& Registers::b() { return operator[](REGID::B); }
constexpr uint8_t& Registers::c() { return operator[](REGID::C); }
constexpr uint8_t& Registers::d() { return operator[](REGID::D); }
constexpr uint8_t& Registers::e() { return operator[](REGID::E); }
constexpr uint8_t& Registers::h() { return operator[](REGID::H); }
constexpr uint8_t& Registers::l() { return operator[](REGID::L); }
//this being in the array is to pad for the register operands not being all adjacent, refer to REGID enum
constexpr uint8_t& Registers::int_enable() { return operator[](REGID::ADDR); }
constexpr void Registers::setSPLow(uint8_t l) { sp = combineLH(l, sp >> 8); }
constexpr void Registers::setSPHigh(uint8_t h) { sp = combineLH(sp & 0xFF, h); }

constexpr uint8_t& Registers::operator[](REGID i) {
	return r[static_cast<int>(i)];
}
constexpr uint8_t Registers::operator[](REGID i) const {
	return r[static_cast<int>(i)];
}
inline uint16_t Registers::getHL() { return h() << 8 | l(); }
#include "cpuimpl.hpp"
