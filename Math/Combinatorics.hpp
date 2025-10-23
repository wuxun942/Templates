#include <bits/stdc++.h>
using namespace std;

// 预处理阶乘 / 组合数：注意区分带模和不带模

// 阶乘逆元 + 带模组合数
constexpr int MOD = 1'000'000'007;
long long qpow(long long x, long long n, int mod) {
    int ans = 1;
    x %= mod;
    for (; n > 0; n >>= 1, x = x * x % mod) {
        if (n & 1) {
            ans = ans * x % mod;
        }
    }
    return ans;
}

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
    return n < m ? 0 : fac[n] * inv_f[m] % mod * inv_f[n - m] % mod;
}

// 不带模组合数

// 不预处理
long long comb(int n, int m) {
    if (n < m) {
        return 0;
    }
    m = min(m, n - m);
    long long res = 1;
    for (int i = 1; i <= m; i++) {
        res = res * (n + 1 - i) / i;
    }
    return res;
}

// 预处理
constexpr int MX = 101;
long long c[MX][MX];
auto init = []() {
    memset(c, 0, sizeof(c));
    c[0][0] = 1;
    for (int i = 1; i < MX; ++i) {
        c[i][0] = c[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return 0;
}();

// Lucas 定理：mod 必须是质数
int Lucas(int n, int m, int mod) {
    if (n < mod && m < mod) {
        return c[n][m];
    }
    return 1LL * c[n % mod][m % mod] * Lucas(n / mod, m / mod, mod);
}