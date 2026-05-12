#include "header.h"

// Найбільший спільний дільник 
BigInt LongGCD(BigInt A, BigInt B) {
    BigInt zero; 
    BigInt Q, R;

    while (LongCmp(B, zero) != 0) {
        LongDivMod(A, B, Q, R); 
        A = B;
        B = R;
    }
    return A;
}

// Найменше спільне кратне
BigInt LongLCM(BigInt A, BigInt B) {
    BigInt zero;

    if (LongCmp(A, zero) == 0 || LongCmp(B, zero) == 0) {
        return zero;
    }

    BigInt gcd = LongGCD(A, B);
    BigInt mul = LongMul(A, B);
    BigInt lcmQ, lcmR;

    LongDivMod(mul, gcd, lcmQ, lcmR);

    return lcmQ;
}