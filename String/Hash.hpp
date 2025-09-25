#include<bits/stdc++.h>
using namespace std;

/*
字符串哈希：将字符串映射为一个整数。
以下使用无符号整数计算哈希值，利用自然溢出，相当于对 2^64 取模。
常用的 base 分两种：
1. 大质数，如 433, 499, 599, 1'000'000'007
2. 经验值，如 31, 131, 1313, 13131, 131313

如需降低哈希冲突的概率，需要用到双模哈希 + 随机 base，但有被卡常的风险。
*/
constexpr int MAXN = 100'001;
size_t h[MAXN], p[MAXN];

void build(const string& s, int base = 1'000'000'007) {
    int n = s.size();
    h[0] = 0;
    p[0] = 1;
    for (int i = 0; i < n; ++i) {
        h[i + 1] = h[i] * base + s[i];
        p[i + 1] = p[i] * base;
    }
}

// 获取 [l, r) 上的哈希值
size_t get_hash(int l, int r) {
    return h[r] - h[l] * p[r - l];
}