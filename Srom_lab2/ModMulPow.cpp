#include "header.h"

// Модулярне множення
BigInt LongModMul(BigInt A, BigInt B, BigInt N, BigInt mu) {

    BigInt mul_res = LongMul(A, B);
    return BarrettReduction(mul_res, N, mu);
}

// Модулярний квадрат
BigInt LongModSquare(BigInt A, BigInt N, BigInt mu) {

    BigInt sqr_res = LongSquare(A);
    return BarrettReduction(sqr_res, N, mu);
}

// Піднесення до степеня за модулем
BigInt LongModPowerBarrett(BigInt A, BigInt B, BigInt N) {
    
    BigInt C(1); 
    BigInt mu = mu_const(N);

    int m = B.BitLength();
    BigInt currentA = A; 

    for (int i = 0; i < m; i++) {
        if (B.GetBit(i) == 1) {
            C = LongModMul(C, currentA, N, mu);
        }

        currentA = LongModSquare(currentA, N, mu);
    }

    return C; 
}