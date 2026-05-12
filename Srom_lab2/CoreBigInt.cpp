#include "header.h"

//задаємо стартовий вигляд
BigInt::BigInt() {
	for (int i = 0; i < 64; i++) {
		box64[i] = 0;
	}
}

//задаємо якесь значення
BigInt::BigInt(uint32_t a) {
	box64[0] = a;
	for (int i = 1; i < 64; i++) {
		box64[i] = 0;
	}
}

//переводимо вхідних hex -> 2^32
BigInt::BigInt(string hex) {
	for (int i = 0; i < 64; i++) {
		box64[i] = 0;
	}

	int boxIndex = 0;
	int hexIndex1 = hex.length();

	while (hexIndex1 > 0 && boxIndex < 64) {
		string chunk = "";
		int hexIndex0 = hexIndex1 - 8;

		if (hexIndex0 < 0) {
			hexIndex0 = 0;
		}

		for (int i = hexIndex0; i < hexIndex1; i++) {
			chunk += hex[i];
		}

		box64[boxIndex] = (uint32_t)stoul(chunk, nullptr, 16);
		boxIndex++;
		hexIndex1 = hexIndex0;
	}
}

// 2^32 -> hex
string BigInt::Hex_convert() {
	string res = "";
	string Sumbol = "0123456789ABCDEF";

	for (int i = 63; i >= 0; i--) {
		uint32_t CurrentBox = box64[i];

		for (int j = 28; j >= 0; j -= 4) {
			uint32_t bit16 = (CurrentBox >> j) & 0xF;
			res += Sumbol[bit16];
		}
	}
	return res;
}

// множення на 2
void BigInt::ShiftLeftOne() {
	uint32_t PreviousCarry = 0;

	for (int i = 0; i < 64; i++) {
		uint32_t CurrentCarry = box64[i] >> 31;
		box64[i] = box64[i] << 1;
		box64[i] = box64[i] | PreviousCarry;
		PreviousCarry = CurrentCarry;
	}
}

// Позиція біта
int BigInt::BitLength() {
	for (int i = 63; i >= 0; i--) {
		for (int j = 31; j >= 0; j--) {
			if ((box64[i] >> j) & 1) {
				return i * 32 + j + 1;
			}
		}
	}
	return 0;
}


void BigInt::SetBit(int bit_number) {
	if (bit_number < 0 || bit_number >= 2048) {
		return;
	}

	int boxIndex = bit_number / 32;
	int bitPos = bit_number % 32;

	box64[boxIndex] |= (1U << bitPos);
}

int BigInt::GetBit(int bit_number) {
	if (bit_number < 0 || bit_number >= 2048) return 0;
	int boxIndex = bit_number / 32;
	int bitPos = bit_number % 32;
	return (box64[boxIndex] >> bitPos) & 1;
}