#include "header.h"

// Рахує кільсть цифр (блоків
int GetBlockLength(BigInt A) {
    for (int i = 63; i >= 0; i--) {
        if (A.box64[i] != 0) {
            return i + 1;
        }
    }
    return 0;
}

// Ділення на b^k (зсув блоків)
BigInt KillLastDigits(BigInt X, int k) {
    BigInt res;

    if (k < 0 || k >= 64) {
        return res; 
    }

    for (int i = 0; i < 64 - k; i++) {
        res.box64[i] = X.box64[i + k];
    }

    return res;
}

// mu = b^(2k) / N
BigInt mu_const(BigInt N) {
    int k = GetBlockLength(N);
    BigInt basePower;

    if (2 * k < 64) {
        basePower.box64[2 * k] = 1;
    }
    else {
        cout << "Error mu_const: Module too large!" << endl;
        return BigInt();
    }

    BigInt Q, R;
    LongDivMod(basePower, N, Q, R);

    return Q;
}

// Редукція за Барреттом
BigInt BarrettReduction(BigInt X, BigInt N, BigInt mu) {
  
    int k = GetBlockLength(N);

    if (k == 0) return BigInt();

    BigInt q1;
    if (k - 1 > 0) {
        q1 = KillLastDigits(X, k - 1);
    }
    else {
        q1 = X;
    }

    BigInt q2 = LongMul(q1, mu);

    BigInt q3 = KillLastDigits(q2, k + 1);
    BigInt q3_n = LongMul(q3, N);
    BigInt r = LongSub(X, q3_n);

    while (LongCmp(r, N) >= 0) { 
        r = LongSub(r, N); 
    }

    return r;
}