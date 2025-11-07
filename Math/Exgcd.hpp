#include <bits/stdc++.h>
using namespace std;

/*
扩展欧几里得算法：
1. 计算 gcd(a,b)
2. 计算 ax + by = gcd(a,b) 的一组特解

一般地，对于 ax + by = c，设 d = gcd(a, b)
方程组拥有整数解的条件为 c % d == 0
整数解的通解形式为
x = x0 + b / d * n
y = y0 - a / d * n
其中 n 为任意正整数
x0, y0 可通过扩展欧几里得算法得到，设求出的解为 x1, y1，则有
x0 = x1 * c / d
y0 = y1 * c / d
*/

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

// 迭代版本的扩展欧几里得算法
long long gcd(long long a, long long b, long long &x, long long &y) {
    x = 1, y = 0;
    long long x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1) {
        long long q = a1 / b1;
        tie(x, x1) = make_tuple(x1, x - q * x1);
        tie(y, y1) = make_tuple(y1, y - q * y1);
        tie(a1, b1) = make_tuple(b1, a1 - q * b1);
    }
    return a1;
}