#include "header.h"

// Модулярне додавання: (A + B) mod N
BigInt LongModAdd(BigInt A, BigInt B, BigInt N) {
    BigInt C = LongAdd(A, B);

    while (LongCmp(C, N) >= 0) {
        C = LongSub(C, N);
    }

    return C;
}

// Модулярне віднімання: (A - B) mod N
BigInt LongModSub(BigInt A, BigInt B, BigInt N) {
    BigInt tempA = A;

    while (LongCmp(tempA, B) < 0) {
        tempA = LongAdd(tempA, N);
    }

    BigInt C = LongSub(tempA, B);

    while (LongCmp(C, N) >= 0) {
        C = LongSub(C, N);
    }

    return C;
}