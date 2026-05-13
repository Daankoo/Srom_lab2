#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

class BigInt {
public:
	uint32_t box64[]; // блок з 64 цифр, де кожна цифра це число розміром 32 біти

	BigInt();
	BigInt(uint32_t a);
	BigInt(string hex);

	void ShiftLeftOne();
	string Hex_convert();
	int BitLength();
	void SetBit(int bit_number);
	int GetBit(int bit_number);
};

class BigInt129 {
public:
	uint32_t box129[];

	BigInt129();
	BigInt129(uint32_t a);
	BigInt129(string hex);

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
BigInt129 KillLastDigits129(BigInt129 X, int k);
BigInt129 mu_const(BigInt N);
BigInt BarrettReduction(BigInt129 X, BigInt N, BigInt129 mu);

BigInt LongModMul(BigInt A, BigInt B, BigInt N, BigInt129 mu);
BigInt LongModSquare(BigInt A, BigInt N, BigInt129 mu);
BigInt LongModPowerBarrett(BigInt A, BigInt B, BigInt N);

BigInt LongModAdd(BigInt A, BigInt B, BigInt N);

// 129

BigInt129 LongAdd129(BigInt129 A, BigInt129 B);
BigInt129 LongSub129(BigInt129 A, BigInt129 B);
int LongCmp129(BigInt129 A, BigInt129 B);
BigInt129 LongShiftDigitsToHigh129(BigInt129 A, int shift);
BigInt129 LongShiftBitsToHigh129(BigInt129 B, int shift);
void LongDivMod129(BigInt129 A, BigInt129 B, BigInt129& Q, BigInt129& R);
BigInt129 LongMul129(BigInt129 A, BigInt129 B);

BigInt129 LongMulTo129(BigInt A, BigInt B);
BigInt129 LongSquareTo129(BigInt A);