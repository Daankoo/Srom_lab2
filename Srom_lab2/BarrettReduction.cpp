#include "header.h"

// 129 комірок 
// зробити клас даб інт 
// і зробити всі опереації під це обмеження 

// Рахує кільсть цифр (блоків
int GetBlockLength(BigInt A) {
    for (int i = 63; i >= 0; i--) {
        if (A.box64[i] != 0) {
            return i+1;
        }
    }
    return 0;
}

// Ділення на b^k (зсув блоків)
BigInt129 KillLastDigits129(BigInt129 X, int k) {
    BigInt129 res;
    if (k < 0 || k >= 129) {
        return res;
    }
    for (int i = 0; i < 129 - k; i++) {
        res.box129[i] = X.box129[i + k];
    }
    return res;
}

// mu = b^(2k) / N
BigInt129 mu_const(BigInt N) {
    int k = GetBlockLength(N); 
    BigInt129 basePower;

    if (2 * k < 129) {
        basePower.box129[2 * k] = 1; 
    }
    else {
        cout << "Error mu_const: Module too large!" << endl;
        return BigInt129();
    }

    BigInt129 N129;
    for (int i = 0; i < 64; i++) {
        N129.box129[i] = N.box64[i];
    }

    BigInt129 Q, R;
    LongDivMod129(basePower, N129, Q, R);

    return Q; 
}

// Редукція за Барреттом
BigInt BarrettReduction(BigInt129 X, BigInt N, BigInt129 mu) {
    int k = GetBlockLength(N);
    if (k == 0) return BigInt();

    BigInt129 q1;
    if (k - 1 > 0) {
        q1 = KillLastDigits129(X, k - 1);
    }
    else {
        q1 = X;
    }

    BigInt129 q2 = LongMul129(q1, mu);
    BigInt129 q3 = KillLastDigits129(q2, k + 1);

    BigInt129 N129;
    for (int i = 0; i < 64; i++) {
        N129.box129[i] = N.box64[i];
    }

    BigInt129 q3_n = LongMul129(q3, N129);
    BigInt129 r129 = LongSub129(X, q3_n);

    while (LongCmp129(r129, N129) >= 0) {
        r129 = LongSub129(r129, N129);
    }

    BigInt r;
    for (int i = 0; i < 64; i++) {
        r.box64[i] = r129.box129[i];
    }

    return r;
}
