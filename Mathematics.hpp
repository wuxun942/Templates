#include <bits/stdc++.h>
using namespace std;

// 埃氏筛筛质数
constexpr int MX = 100'001;
bool not_prime[MX]{};
auto init_prime = []() {
    not_prime[0] = not_prime[1] = true;
    for (int i = 2; i * i < MX; i++) {
        if (!not_prime[i]) {
            for (int j = i * i; j < MX; j += i) {
                not_prime[j] = true;
            }
        }
    }
    return 0;
}();

// 欧拉筛预处理 LPF (最大质因子)
constexpr int MX = 100'001;
vector<int> primes;
array<int, MX> lpf;
auto init_lpf = []() {
    for (int i = 2; i < MX; i++) {
        if (lpf[i] == 0) {
            lpf[i] = i;
            primes.push_back(i);
        }
        for (int p: primes) {
            if (i * p >= MX) {
                break;
            }
            // if (lpf[i * p] == 0) {
            //     lpf[i * p] = p;
            // }
            lpf[i * p] = p;
            if (i % p == 0) {
                break;
            }
        }
    }
    return 0;
}();

// 预处理因数
constexpr int MX = 100'001;
vector<int> divisors[MX];
auto init_div = []() {
    for (int i = 1; i < MX; ++i) {
        for (int j = i; j < MX; j += i) {
            divisors[j].push_back(i);
        }
    }
    return 0;
}();

// 乘法快速幂
long long qpow(unsigned long long x, int n) {
    long long ans = 1;
    for (; n > 0; n >>= 1, x *= x) {
        if (n & 1) {
            ans = ans * x;
        }
    }
    return ans;
}

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
auto init_fac = []() {
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