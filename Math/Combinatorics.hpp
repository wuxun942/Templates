#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 1'000'000'007;
int qpow(long long x, long long n, int mod) {
    int ans = 1;
    x %= mod;
    for (; n > 0; n >>= 1, x = (x * x) % mod) {
        if (n & 1) {
            ans = ans * x % mod;
        }
    }
    return ans;
}

// 阶乘逆元 + 带模组合数
constexpr int MOD = 1'000'000'007;
constexpr int MX = 200'001;
long long fac[MX], inv_f[MX];
auto init = []() {
    fac[0] = 1;
    for (int i = 1; i < MX; i++) {
        fac[i] = fac[i - 1] * i % MOD;
    }
    inv_f[MX - 1] = qpow(fac[MX - 1], MOD - 2, MOD);
    for (int i = MX - 2; i >= 0; i--) {
        inv_f[i] = inv_f[i + 1] * (i + 1) % MOD;
    }
    return 0;
}();

int comb(int n, int m, int mod) {
    return fac[n] * inv_f[m] % mod * inv_f[n - m] % mod;
}

// 不带模组合数
long long comb(int n, int m) {
    m = min(m, n - m);
    long long res = 1;
    for (int i = 1; i <= m; i++) {
        res = res * (n + 1 - i) / i;
    }
    return res;
}