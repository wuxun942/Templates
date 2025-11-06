#include <bits/stdc++.h>
using namespace std;

// 预处理阶乘 / 组合数

// 1. 不预处理，直接计算组合数
long long comb(int n, int m) {
    if (n < m) {
        return 0;
    }
    m = min(m, n - m);
    long long res = 1;
    for (int i = 1; i <= m; i++) {
        res = res * (n + 1 - i) / i; // 注意计算顺序
    }
    return res;
}

// 2. 递推预处理组合数
constexpr int MAXN = 101;
long long c[MAXN][MAXN];
auto init = []() {
    memset(c, 0, sizeof(c));
    c[0][0] = 1;
    for (int i = 1; i < MAXN; ++i) {
        c[i][0] = c[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return 0;
}();

// 3. 质数模下的预处理：阶乘逆元 + 带模组合数
constexpr int MOD = 1'000'000'007;
long long qpow(long long x, long long n, int mod = MOD) {
    int ans = 1;
    x %= mod;
    for (; n > 0; n >>= 1, x = x * x % mod) {
        if (n & 1) {
            ans = ans * x % mod;
        }
    }
    return ans;
}

constexpr int MAXN = 200'001;
long long fac[MAXN], inv_f[MAXN];
auto init = []() {
    fac[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        fac[i] = fac[i - 1] * i % MOD;
    }
    inv_f[MAXN - 1] = qpow(fac[MAXN - 1], MOD - 2, MOD);
    for (int i = MAXN - 2; i >= 0; i--) {
        inv_f[i] = inv_f[i + 1] * (i + 1) % MOD;
    }
    return 0;
}();

long long comb(int n, int m, int mod = MOD) {
    if (n < m) {
        return 0;
    }
    return fac[n] * inv_f[m] % mod * inv_f[n - m] % mod;
}

// 4. 小模数下的更大范围计算
// 4.1 Lucas 定理：mod 必须是质数
constexpr int MAXN = 101;
long long c[MAXN][MAXN];
auto init = []() {
    memset(c, 0, sizeof(c));
    c[0][0] = 1;
    for (int i = 1; i < MAXN; ++i) {
        c[i][0] = c[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return 0;
}();

int Lucas(int n, int m, int mod) {
    if (n < mod && m < mod) {
        return c[n][m];
    }
    return 1LL * c[n % mod][m % mod] * Lucas(n / mod, m / mod, mod) % mod;
}

/*
扩展 Lucas：推广到 mod 为一般数的情况

把 mod 质因数分解为 p1 ** q1 * p2 ** q2 * ...
令 mi = pi ** qi
先求出 ri = x % mi
由 CRT，x 的通解为 mod * y + x0
那么 x0 才是最后取模的结果
*/