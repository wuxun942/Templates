#include <bits/stdc++.h>
using namespace std;

/*
扩展欧几里得算法：
1. 计算 gcd(a,b)
2. 计算 ax + by = gcd(a,b) 的一组解
*/
template<typename T>
tuple<T, T, T> exgcd(T a, T b) {
    if (b == 0) {
        return {a, 1, 0};
    }
    auto [g, x, y] = exgcd(b, a % b);
    return {g, y, x - a / b * y};
}

// 扩展欧几里得算法求逆元：要求 gcd(a,b) = 1
template<typename T>
T get_inv(T x, T mod) {
    auto [g, a, b] = exgcd(x, mod);
    return (a % mod + mod) % mod;
}