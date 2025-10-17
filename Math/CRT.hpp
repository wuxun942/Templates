#include <bits/stdc++.h>
using namespace std;

template<typename T>
T exgcd(T a, T b, T &x, T &y) {
    if (b == 0) {
        return a;
    }
    T g = exgcd(b, a % b, x, y);
    T tmp = x;
    x = y;
    y = tmp - a / b * y;
    return g;
}

constexpr int MAXN = 10;
int n;
long long m[MAXN], r[MAXN];
long long crt() {
    long long lcm = 1;
    for (int i = 0; i < n; ++i) {
        lcm *= m[i];
    }
    long long a, c, ans = 0, x, y;
    for (int i = 0; i < n; ++i) {
        a = lcm / m[i];
        exgcd(a, m[i], x, y);
        c = (__int128_t) a * x % lcm * r[i] % lcm;
        ans = (ans + c) % lcm;
    }
    return ans;
}