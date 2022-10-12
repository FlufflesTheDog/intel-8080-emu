#pragma once
#include "oputils.hpp"
#include <cstdint>
#include <memory>

#ifdef _MSC_VER
#  define __builtin_parity OpUtils::findParity
#endif

/// <summary>
/// Struct representing the Flags in the Intel 8080 Processor
/// </summary>
struct Flags
{
	enum FlagPos {
		CARRY = 0,
		PARITY = 2,
		AUXCARRY = 4,
		ZERO = 6,
		SIGN = 7,
	};
	using byte = OpUtils::byte;
	uint8_t Zero : 1;
	uint8_t Sign : 1;
	uint8_t Parity : 1;
	uint8_t Carry : 1;
	uint8_t AuxiliaryCarry : 1; //Unimplemented for Space Invaders

	void SetMainFlags(byte n)
	{
		Zero = n == 0;
		Sign = n >> 7;
		Parity = __builtin_parity(n) == 0;
	}

	byte DoSubtraction(byte l, byte r, bool withBorrow = false)
	{
		unsigned result = l - r;
		result -= withBorrow && Carry;
		Carry = result >> 8 > 0;
		SetMainFlags(result);
		return result;
	}

	byte DoAddition(byte l, byte r, bool withCarry = false)
	{
		unsigned result = l + r;
		result += withCarry && Carry;
		Carry = result >> 8 > 0;
		SetMainFlags(result);
		return result;
	}

	void bitOpCheck(byte n)
	{
		SetMainFlags(n);
		Carry = 0;
	}
};

struct Registers
{
	enum class REG_ID
	{
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
	/// <summary>
	/// Stack Pointer
	/// </summary>
	uint16_t SP;
	/// <summary>
	/// Program Counter
	/// </summary>
	uint16_t PC;
	constexpr uint8_t& a();
	constexpr uint8_t& b();
	constexpr uint8_t& c();
	constexpr uint8_t& d();
	constexpr uint8_t& e();
	constexpr uint8_t& h();
	constexpr uint8_t& l();
	/// <summary>
	/// Interrupts Enable
	/// </summary>
	/// <returns></returns>
	constexpr uint8_t& int_enable();
	constexpr void SetSPLowBits(uint8_t l);
	constexpr void SetSPHighBits(uint8_t h);
	uint16_t GetHL();
	uint8_t flagsToByte() const;
	void byteToFlags(uint8_t flagByte);
	constexpr uint8_t& operator[](REG_ID i);
	constexpr uint8_t operator[](REG_ID i) const;
};

class State
{
	using byte = OpUtils::byte;
public:
	// Initializer for Processor State
	State() = default;
	State(const char* fileName);

	/// <summary>
	/// Read file consisting of data
	/// </summary>
	/// <param name="file">File Name relative to executable</param>
	void ReadProgram(const char* fileName);
	/// <summary>
	/// ReadBytes function. Currently unused.
	/// </summary>
	/// <param name="stream"></param>
	/// <param name="size"></param>
	void ReadBytes(const byte* stream, int size);
	/// <summary>
	/// Steps through a single OP code of the given CPU Emulator state.
	/// </summary>
	/// <returns>True if it succeeded. False if it failed to process the OP Code</returns>
	bool StepOpCode();
	bool IsValid();
	bool Interrupt(int code);
	struct IODeviceHandler {
		virtual byte IN(byte port) = 0;
		virtual void OUT(byte port, byte data) = 0;
		virtual ~IODeviceHandler() = default;
	};
	std::unique_ptr<IODeviceHandler> devices;
	std::unique_ptr<byte[]> Memory;
private:
	static constexpr int RAMSize = 0x10000; //0xFFFF + 1 == 0x10000;
	Registers registers{};
	uint16_t readAddr(const byte* mem) const;
	void dad(byte op);
	void inr(byte& n);
	void dcr(byte& n);
	void inx(byte op);
	void dcx(byte op);
	void ret();
	bool rcnd(byte* op);
	void jmp(byte* op);
	bool jcnd(byte* op);
	void call(byte* op);
	bool ccnd(byte* op);
	void mov(byte* op);
	void pop(byte op);
	void push(byte op);
	void rotateLeft(bool carry = false);
	void rotateRight(bool carry = false);
	void daa();
};

inline constexpr uint16_t combineLH(OpUtils::byte low, OpUtils::byte high)
{
	return low | high << 8;
}

constexpr uint8_t& Registers::a() { return operator[](REG_ID::A); }
constexpr uint8_t& Registers::b() { return operator[](REG_ID::B); }
constexpr uint8_t& Registers::c() { return operator[](REG_ID::C); }
constexpr uint8_t& Registers::d() { return operator[](REG_ID::D); }
constexpr uint8_t& Registers::e() { return operator[](REG_ID::E); }
constexpr uint8_t& Registers::h() { return operator[](REG_ID::H); }
constexpr uint8_t& Registers::l() { return operator[](REG_ID::L); }
//this being in the array is to pad for the register operands not being all adjacent, refer to REGID enum
constexpr uint8_t& Registers::int_enable() { return operator[](REG_ID::ADDR); }
constexpr void Registers::SetSPLowBits(uint8_t l) { SP = combineLH(l, SP >> 8); }
constexpr void Registers::SetSPHighBits(uint8_t h) { SP = combineLH(SP & 0xFF, h); }

constexpr uint8_t& Registers::operator[](REG_ID i)
{
	return r[static_cast<int>(i)];
}
constexpr uint8_t Registers::operator[](REG_ID i) const
{
	return r[static_cast<int>(i)];
}
inline uint8_t Registers::flagsToByte() const {
	return
		flags.Sign << flags.SIGN |
		flags.Zero << flags.ZERO |
		0 << 5 |
		flags.AuxiliaryCarry << flags.AUXCARRY |
		0 << 3 |
		flags.Parity << flags.PARITY |
		1 << 1 |
		flags.Carry;
}
inline void Registers::byteToFlags(uint8_t flagByte) {
	auto getBit = [flagByte](int bitNumber) {
		return flagByte >> bitNumber & 1;
	};
	flags.Sign = getBit(flags.SIGN);
	flags.Zero = getBit(flags.ZERO);
	flags.AuxiliaryCarry = getBit(flags.AUXCARRY);
	flags.Parity = getBit(flags.PARITY);
	flags.Carry = getBit(flags.CARRY);
}
inline uint16_t Registers::GetHL() { return h() << 8 | l(); }
#include "cpuimpl.hpp"
