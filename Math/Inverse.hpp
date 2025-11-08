#include <bits/stdc++.h>
using namespace std;

/*
乘法逆元：a * inv[a] % mod = 1
请注意不同方法中对模的要求
*/

/*
快速幂求逆元：要求 mod 是质数，原理为费马小定理
实际上可以用欧拉定理求更多 mod 的逆元，条件放宽为 gcd(a, b) = 1
只需要把指数项改为 phi(mod) - 1
phi 为欧拉函数，表示小于等于 mod 的正整数中与 mod 互质的整数个数
*/
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

int get_inv(long long a, int mod) {
    return qpow(a, mod - 2, mod);
}

// 扩展欧几里得算法求逆元：要求 gcd(a,b) = 1
long long exgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long g = exgcd(b, a % b, x, y);
    long long tmp = x;
    x = y;
    y = tmp - a / b * y;
    return g;
}

int get_inv(long long a, int mod) {
    long long x, y;
    long long g = exgcd(a, mod, x, y);
    if (g != 1) {
        throw runtime_error("Invalid Input");
    }
    return (x % mod + mod) % mod;
}

/*
连续整数求逆元，同样要求两数互质
证明：
p / i * i + p % i = 0 (mod p)
inv[i] + p / i * inv[p % i] = 0 (mod p)
为了使其非负，需要加上 p
inv[i] = p - p / i * inv[p % i] (mod p)
*/
constexpr int MOD = 1'000'000'007;
constexpr int MAXX = 100'001;
int inv[MAXX];
auto init = []() {
    inv[1] = 1;
    for (int i = 2; i < MAXX; ++i) {
        inv[i] = MOD - 1LL * inv[MOD % i] * (MOD / i) % MOD;
    }
    return 0;
}();