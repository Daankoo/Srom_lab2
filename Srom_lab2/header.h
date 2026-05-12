#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

class BigInt {
public:
	uint32_t box64[64]; // блок з 64 цифр, де кожна цифра це число розміром 32 біти

	BigInt();
	BigInt(uint32_t a);
	BigInt(string hex);

	void ShiftLeftOne();
	string Hex_convert();
	int BitLength();
	void SetBit(int bit_number);
	int GetBit(int bit_number);
};

BigInt LongAdd(BigInt A, BigInt B);
BigInt LongSub(BigInt A, BigInt B);
int LongCmp(BigInt A, BigInt B);
BigInt LongMulOneDigit(BigInt A, uint32_t B);
BigInt LongShiftDigitsToHigh(BigInt A, int shift);
BigInt LongMul(BigInt A, BigInt B);
BigInt LongSquare(BigInt A);
BigInt LongShiftBitsToHigh(BigInt B, int shift);
void LongDivMod(BigInt A, BigInt B, BigInt& Q, BigInt& R);

BigInt LongGCD(BigInt A, BigInt B);
BigInt LongLCM(BigInt A, BigInt B);
int GetBlockLength(BigInt A);
BigInt KillLastDigits(BigInt X, int k);
BigInt mu_const(BigInt N);
BigInt BarrettReduction(BigInt X, BigInt N, BigInt mu);

BigInt LongModMul(BigInt A, BigInt B, BigInt N, BigInt mu);
BigInt LongModSquare(BigInt A, BigInt N, BigInt mu);
BigInt LongModPowerBarrett(BigInt A, BigInt B, BigInt N);