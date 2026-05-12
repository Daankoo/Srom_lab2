#include "header.h"

// Ğàõóº ê³ëüñòü öèôğ (áëîê³â
int GetBlockLength(BigInt A) {
    for (int i = 63; i >= 0; i--) {
        if (A.box64[i] != 0) {
            return i + 1;
        }
    }
    return 0;
}

// Ä³ëåííÿ íà ?^k (çñóâ áëîê³â)
BigInt ShiftBlocksRight(BigInt X, int k) {
    BigInt res;

    if (k >= 64) {
        return res;
    }

    for (int i = 0; i < 64 - k; i++) {
        res.box64[i] = X.box64[i + k];
    }

    return res;
}

// ? = ??^(2k) / N?
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