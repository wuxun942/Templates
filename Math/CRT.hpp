#include <bits/stdc++.h>
using namespace std;

// 前置：扩展欧几里得算法

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

long long get_inv(long long a, long long mod) {
    long long x, y;
    long long g = exgcd(a, mod, x, y);
    if (g != 1) {
        throw runtime_error("Invalid Input");
    }
    return (x % mod + mod) % mod;
}

long long multiply(long long a, long long b, long long mod) {
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    return (__int128_t) a * b % mod;
}

/*
中国剩余定理
对于方程组
x % m1 = r1
x % m2 = r2
...
x % mn = rn
求出 x 的最小正整数解
要求：每个模数两两互质

记 ai = m1 * m2 * ... * mn / mi
可以得到 x = c1 + c2 + ... + cn
其中 ci = r1 * ai * pow(ai, -1, mi)
最后一项为 lcm / mi 在模 mi 下的逆元，需要用到扩欧求解
*/ 

constexpr int MAXN = 100'001;
int n;
long long m[MAXN], r[MAXN];
long long crt() {
    long long lcm = 1;
    for (int i = 0; i < n; ++i) {
        lcm *= m[i];
    }
    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        long long a = lcm / m[i];
        long long x = get_inv(a, m[i]); // a 的逆元
        long long c = multiply(r[i], multiply(a, x, lcm), lcm);
        ans = (ans + c) % lcm;
    }
    return ans;
}

/*
扩展中国剩余定理
引理：以上同余方程的通解可以表示为
ans = lcm * x + tail
其中 lcm = m1 * m2 * ... * mn
tail 为常数
证明：
1. 先考虑第一个方程，那么有
ans = m1 * x + r
符合形式
2. （数学归纳）
假设前 k 个方程联立后满足 ans = lcm * x + tail
对第 k + 1 个方程，则有
ans = m_{k+1} * y + r_{k+1}
整理得到 lcm * x - m_{k+1} * y = r_{k+1} - tail
这是一个二元一次不定方程，仅当 gcd(lcm, m_{k+1}) | r_{k+1} - tail
由裴蜀定理，这个方程存在整数解
即对前 k + 1 个方程，仍然能将解的形式表示为 ans = lcm * x + tail
*/

constexpr int MAXN = 100'001;
int n;
long long m[MAXN], r[MAXN];
long long excrt() {
    long long lcm = 1, tail = 0, x, y;
    for (int i = 0; i < n; ++i) {
        /*
        ans = lcm * x + tail = m[i] * y + r[i]
        lcm * x + m[i] * y = r[i] - tail
        a = lcm, b = m[i], c = r[i] - tail
        */
        long long b = m[i];
        long long c = ((r[i] - tail) % b + b) % b; // 通过调整 y，把 c 变成最小非负数
        long long d = exgcd(lcm, b, x, y);
        if (c % d) {
            return -1; // c 不是 d 的整数倍，则无整数解
        }
        /*
        x 是 lcm * x + b * y = d 的一个特解，
        对于 lcm * x + b * y = c，它的对应特解为 x * (c / d)
        将其对 b / d 取模，得到最小非负特解
        */
        long long x0 = multiply(x, c / d, b / d);
        /*
        ans = lcm * x + tail = lcm * (x0 + (b / d) * n) + tail = lcm * (b / d) * n + lcm * x0 + tail
        new_lcm = lcm * (b / d)
        new_tail = (lcm * x0 + tail) % new_lcm
        */
        long long new_lcm = lcm * (b / d);
        tail = (tail + multiply(x0, lcm, new_lcm)) % new_lcm;
        lcm = new_lcm;
    }
    return tail;
}