#include "header.h"

// Додавання
BigInt LongAdd(BigInt A, BigInt B) {
	BigInt C;
	uint32_t CarryAdd = 0;

	for (int i = 0; i < 64; i++) {
		uint64_t temp = (uint64_t)A.box64[i] + B.box64[i] + CarryAdd;
		C.box64[i] = (uint32_t)temp;
		CarryAdd = temp >> 32;
	}

	return C;
}

// Віднімання
BigInt LongSub(BigInt A, BigInt B) {
	BigInt C;
	uint32_t BorrowSub = 0;

	for (int i = 0; i < 64; i++) {
		int64_t temp = (uint64_t)A.box64[i] - B.box64[i] - BorrowSub;
		C.box64[i] = (uint32_t)temp;
		BorrowSub = (temp >> 32) & 1;
	}

	return C;
}

// Порівняння
int LongCmp(BigInt A, BigInt B) {
	int i = 63;

	while (i >= 0 && A.box64[i] == B.box64[i]) {
		i = i - 1;
	}

	if (i == -1) {
		return 0;
	}
	else {
		if (A.box64[i] > B.box64[i]) {
			return 1;
		}
		else {
			return -1;
		}
	}
}

// Множення на одне число
BigInt LongMulOneDigit(BigInt A, uint32_t B) {
	BigInt C;
	uint32_t CarryMulOne = 0;

	for (int i = 0; i < 64; i++) {
		uint64_t temp = (uint64_t)A.box64[i] * B + CarryMulOne;
		C.box64[i] = (uint32_t)temp;
		CarryMulOne = temp >> 32;
	}

	return C;
}

// Зсовуємо
BigInt LongShiftDigitsToHigh(BigInt A, int shift) {
	BigInt C;
	for (int i = 0; i < 64 - shift; i++) {
		C.box64[i + shift] = A.box64[i];
	}
	return C;
}

// Повноцінне множення
BigInt LongMul(BigInt A, BigInt B) {
	BigInt C;
	for (int i = 0; i < 64; i++) {
		BigInt temp = LongMulOneDigit(A, B.box64[i]);
		temp = LongShiftDigitsToHigh(temp, i);
		C = LongAdd(C, temp);
	}
	return C;
}

//Піднесення в квадрат
BigInt LongSquare(BigInt A) {
	BigInt C;

	for (int i = 0; i < 64; i++) {
		uint32_t carry = 0;
		for (int j = i + 1; j < 64; j++) {
			if (i + j < 64) {

				uint64_t prod = (uint64_t)A.box64[i] * A.box64[j];
				uint64_t temp = (uint64_t)C.box64[i + j] + (prod * 2) + carry;

				C.box64[i + j] = (uint32_t)temp;
				carry = temp >> 32;
			}
		}
	}

	uint32_t carryDiag = 0;
	for (int i = 0; i < 64; i++) {
		int targetIndex = i * 2;

		if (targetIndex < 64) {
			uint64_t prod = (uint64_t)A.box64[i] * A.box64[i];
			uint64_t temp = (uint64_t)C.box64[targetIndex] + prod + carryDiag;

			C.box64[targetIndex] = (uint32_t)temp;
			carryDiag = temp >> 32;

			if (targetIndex + 1 < 64) {
				uint64_t temp2 = (uint64_t)C.box64[targetIndex + 1] + carryDiag;
				C.box64[targetIndex + 1] = (uint32_t)temp2;
				carryDiag = temp2 >> 32;
			}
		}
	}

	return C;
}

// зсув числа на певну кількість біт
BigInt LongShiftBitsToHigh(BigInt B, int shift) {
	BigInt C = B;
	for (int i = 0; i < shift; i++) {
		C.ShiftLeftOne();
	}
	return C;
}

// ділення
void LongDivMod(BigInt A, BigInt B, BigInt& Q, BigInt& R) {
	int k = B.BitLength();
	R = A;

	while (LongCmp(R, B) >= 0) {
		int t = R.BitLength();
		BigInt C = LongShiftBitsToHigh(B, t - k);

		if (LongCmp(R, C) < 0) {
			C = LongShiftBitsToHigh(B, t - k - 1);
			t = t - 1;
		}

		R = LongSub(R, C);
		Q.SetBit(t - k);
	}
}