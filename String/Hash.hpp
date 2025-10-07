#include<bits/stdc++.h>
using namespace std;

/*
字符串哈希：将字符串映射为一个整数。
常用的 base 分两种：
1. 大质数，如 433, 499, 599, 1'000'000'007
2. 经验值，如 31, 131, 1313, 13131, 131313

如需降低哈希冲突的概率，需要用到双模哈希 + 随机 base，但有被卡常的风险。
*/

// 单模哈希：以下使用无符号整数计算哈希值，利用自然溢出，相当于对 2^64 取模。
// 可使用 “异或随机数” 的方式降低哈希冲突的概率
constexpr int MAXN = 100'001;
size_t h[MAXN], p[MAXN], q;

void build(const string& s, int base = 1'000'000'007) {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    q = uniform_int_distribution<>(8e8, 9e8)(rng); // 生成随机数，用于异或
    h[0] = 0;
    p[0] = 1;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        h[i + 1] = h[i] * base + s[i] ^ q;
        p[i + 1] = p[i] * base;
    }
}

// 获取 [l, r) 上的哈希值
size_t get_hash(int l, int r) {
    return h[r] - h[l] * p[r - l];
}

// 双模哈希：使用随机 base
// 为方便压缩表示，以下使用 int 范围下的 MOD
constexpr int MAXN = 100'001;
long long h1[MAXN], p1[MAXN], h2[MAXN], p2[MAXN];
int base1, base2;
constexpr int MOD1 = 1'070'777'777, MOD2 = 1'000'000'007;

void build(const string& s) {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    base1 = uniform_int_distribution<>(8e8, 9e8)(rng);
    base2 = uniform_int_distribution<>(8e8, 9e8)(rng);
    h1[0] = h2[0] = 0;
    p1[0] = p2[0] = 1;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        h1[i + 1] = (h1[i] * base1 + s[i]) % MOD1;
        p1[i + 1] = p1[i] * base1 % MOD1;
        h2[i + 1] = (h2[i] * base2 + s[i]) % MOD2;
        p2[i + 1] = p2[i] * base2 % MOD2;
    }
}

// 获取 [l, r) 上的哈希值，压缩成一个 long long 类型整数
long long get_hash2(int l, int r) {
    long long v1 = (h1[r] - h1[l] * p1[r - l] % MOD1 + MOD1) % MOD1;
    long long v2 = (h2[r] - h2[l] * p2[r - l] % MOD2 + MOD2) % MOD2;
    return v1 << 32 | v2;
}