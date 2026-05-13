#include "header.h"

//задаємо стартовий вигляд
BigInt129::BigInt129() {
	for (int i = 0; i < 129; i++) {
		box129[i] = 0;
	}
}

//задаємо якесь значення
BigInt129::BigInt129(uint32_t a) {
	box129[0] = a;
	for (int i = 1; i < 129; i++) {
		box129[i] = 0;
	}
}

//переводимо вхідних hex -> 2^32
BigInt129::BigInt129(string hex) {
	for (int i = 0; i < 129; i++) {
		box129[i] = 0;
	}

	int boxIndex = 0;
	int hexIndex1 = hex.length();

	while (hexIndex1 > 0 && boxIndex < 129) {
		string chunk = "";
		int hexIndex0 = hexIndex1 - 8;

		if (hexIndex0 < 0) {
			hexIndex0 = 0;
		}

		for (int i = hexIndex0; i < hexIndex1; i++) {
			chunk += hex[i];
		}

		box129[boxIndex] = (uint32_t)stoul(chunk, nullptr, 16);
		boxIndex++;
		hexIndex1 = hexIndex0;
	}
}

// 2^32 -> hex
string BigInt129::Hex_convert() {
	string res = "";
	string Sumbol = "0123456789ABCDEF";
	bool leadingZeros = true; // Щоб не виводити купу нулів на початку

	for (int i = 128; i >= 0; i--) {
		uint32_t CurrentBox = box129[i];
		for (int j = 28; j >= 0; j -= 4) {
			uint32_t bit16 = (CurrentBox >> j) & 0xF;
			if (bit16 != 0) leadingZeros = false;
			if (!leadingZeros || (i == 0 && j == 0)) {
				res += Sumbol[bit16];
			}
		}
	}
	if (res == "") res = "0";
	return res;
}

// множення на 2
void BigInt129::ShiftLeftOne() {
	uint32_t PreviousCarry = 0;
	for (int i = 0; i < 129; i++) {
		uint32_t CurrentCarry = box129[i] >> 31;
		box129[i] = box129[i] << 1;
		box129[i] = box129[i] | PreviousCarry;
		PreviousCarry = CurrentCarry;
	}
}

// Позиція біта
int BigInt129::BitLength() {
	for (int i = 128; i >= 0; i--) {
		if (box129[i] == 0) continue; // Оптимізація: пропускаємо порожні блоки
		for (int j = 31; j >= 0; j--) {
			if ((box129[i] >> j) & 1) {
				return i * 32 + j + 1;
			}
		}
	}
	return 0;
}

void BigInt129::SetBit(int bit_number) {
	if (bit_number < 0 || bit_number >= 4128) return; // 129 * 32 = 4128
	int boxIndex = bit_number / 32;
	int bitPos = bit_number % 32;
	box129[boxIndex] |= (1U << bitPos);
}

int BigInt129::GetBit(int bit_number) {
	if (bit_number < 0 || bit_number >= 4128) return 0;
	int boxIndex = bit_number / 32;
	int bitPos = bit_number % 32;
	return (box129[boxIndex] >> bitPos) & 1;
}

// Додавання 129
BigInt129 LongAdd129(BigInt129 A, BigInt129 B) {
	BigInt129 C;
	uint32_t CarryAdd = 0;
	for (int i = 0; i < 129; i++) {
		uint64_t temp = (uint64_t)A.box129[i] + B.box129[i] + CarryAdd;
		C.box129[i] = (uint32_t)temp;
		CarryAdd = temp >> 32;
	}
	return C;
}

// Віднімання 129
BigInt129 LongSub129(BigInt129 A, BigInt129 B) {
	BigInt129 C;
	uint32_t BorrowSub = 0;
	for (int i = 0; i < 129; i++) {
		int64_t temp = (uint64_t)A.box129[i] - B.box129[i] - BorrowSub;
		C.box129[i] = (uint32_t)temp;
		BorrowSub = (temp >> 32) & 1;
	}
	return C;
}

// Порівняння 129
int LongCmp129(BigInt129 A, BigInt129 B) {
	int i = 128;
	while (i >= 0 && A.box129[i] == B.box129[i]) {
		i = i - 1;
	}
	if (i == -1) return 0;
	return (A.box129[i] > B.box129[i]) ? 1 : -1;
}

// Зсув блоків 129
BigInt129 LongShiftDigitsToHigh129(BigInt129 A, int shift) {
	BigInt129 C;
	if (shift >= 129) return C;
	for (int i = 0; i < 129 - shift; i++) {
		C.box129[i + shift] = A.box129[i];
	}
	return C;
}

// Зсув бітів 129
BigInt129 LongShiftBitsToHigh129(BigInt129 B, int shift) {
	BigInt129 C = B;
	for (int i = 0; i < shift; i++) {
		C.ShiftLeftOne();
	}
	return C;
}

// Ділення 129 (дуже важливо для знаходження mu = b^2k / N)
void LongDivMod129(BigInt129 A, BigInt129 B, BigInt129& Q, BigInt129& R) {
	int k = B.BitLength();
	R = A;
	BigInt129 zero;
	Q = zero;

	if (k == 0) return; // Ділення на нуль

	while (LongCmp129(R, B) >= 0) {
		int t = R.BitLength();
		BigInt129 C = LongShiftBitsToHigh129(B, t - k);

		if (LongCmp129(R, C) < 0) {
			C = LongShiftBitsToHigh129(B, t - k - 1);
			t = t - 1;
		}
		R = LongSub129(R, C);
		Q.SetBit(t - k);
	}
}

// Множення двох BigInt (64) з результатом у BigInt129
BigInt129 LongMulTo129(BigInt A, BigInt B) {
	BigInt129 C; // Вже заповнений нулями

	for (int i = 0; i < 64; i++) {
		if (A.box64[i] == 0) continue; // Пропускаємо множення на нуль
		uint32_t carry = 0;
		for (int j = 0; j < 64; j++) {
			uint64_t temp = (uint64_t)C.box129[i + j] + ((uint64_t)A.box64[i] * B.box64[j]) + carry;
			C.box129[i + j] = (uint32_t)temp;
			carry = temp >> 32;
		}
		C.box129[i + 64] = carry;
	}
	return C;
}

// Квадрат BigInt (64) з результатом у BigInt129
BigInt129 LongSquareTo129(BigInt A) {
	BigInt129 C;

	for (int i = 0; i < 64; i++) {
		uint32_t carry = 0;
		for (int j = i + 1; j < 64; j++) {
			uint64_t prod = (uint64_t)A.box64[i] * A.box64[j];
			uint64_t temp = (uint64_t)C.box129[i + j] + (prod * 2) + carry;
			C.box129[i + j] = (uint32_t)temp;
			carry = temp >> 32;
		}
		if (i + 64 < 129) {
			C.box129[i + 64] += carry;
		}
	}

	uint32_t carryDiag = 0;
	for (int i = 0; i < 64; i++) {
		int targetIndex = i * 2;
		uint64_t prod = (uint64_t)A.box64[i] * A.box64[i];
		uint64_t temp = (uint64_t)C.box129[targetIndex] + prod + carryDiag;

		C.box129[targetIndex] = (uint32_t)temp;
		carryDiag = temp >> 32;

		if (targetIndex + 1 < 129) {
			uint64_t temp2 = (uint64_t)C.box129[targetIndex + 1] + carryDiag;
			C.box129[targetIndex + 1] = (uint32_t)temp2;
			carryDiag = temp2 >> 32;
		}
	}
	return C;
}

// Множення двох BigInt129. 
// Увага: оскільки масив обмежений 129 блоками, старші розряди відсікаються.
// Для Барретта цього достатньо, бо ми все одно там відкидаємо розряди.
BigInt129 LongMul129(BigInt129 A, BigInt129 B) {
	BigInt129 C;
	for (int i = 0; i < 129; i++) {
		if (A.box129[i] == 0) continue;
		uint32_t carry = 0;
		for (int j = 0; i + j < 129; j++) {
			uint64_t temp = (uint64_t)C.box129[i + j] + ((uint64_t)A.box129[i] * B.box129[j]) + carry;
			C.box129[i + j] = (uint32_t)temp;
			carry = temp >> 32;
		}
	}
	return C;
}