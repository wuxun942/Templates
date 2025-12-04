#include <bits/stdc++.h>
using namespace std;

/*
质数筛 和 调和级数枚举

质数筛的用法：
1. 筛质数 / 预处理质数判断
2. 求最小质因子

调和级数枚举常用于 “倍数” 相关问题
*/ 

// 埃氏筛筛质数 O(MAX_X * log(log(MAX_X)))
constexpr int MAX_X = 100'000 + 5;
bool is_prime[MAX_X];
vector<int> primes;
auto init = []() {
    ranges::fill(is_prime, true);
    is_prime[0] = is_prime[1] = false;
    // 如果只要填 is_prime，循环条件可以放宽到 i * i < MAX_X
    for (int i = 2; i < MAX_X; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = i * i; j < MAX_X; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return 0;
}();

// 欧拉筛预处理 LPF (最小质因子) O(MAX_X)
constexpr int MAX_X = 100'000 + 5;
vector<int> primes;
int lpf[MAX_X];
auto init = []() {
    for (int i = 2; i < MAX_X; i++) {
        if (lpf[i] == 0) {
            lpf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (i * p >= MAX_X) {
                break;
            }
            if (lpf[i * p] == 0) {
                lpf[i * p] = p;
            }
            if (i % p == 0) {
                break;
            }
        }
    }
    return 0;
}();

// 预处理因数（和质数筛无关，属于调和级数枚举，不过套路相似）
constexpr int MAX_X = 100'000 + 5;
vector<int> divisors[MAX_X];
auto init = []() {
    for (int i = 1; i < MAX_X; ++i) {
        for (int j = i; j < MAX_X; j += i) {
            divisors[j].push_back(i);
        }
    }
    return 0;
}();

// 平方剩余核
constexpr int MAX_X = 100'000 + 5;
int core[MAX_X];
auto init = []() {
    for (int i = 1; i < MAX_X; ++i) {
        for (int j = 1; j * i * i < MAX_X; ++j) {
            core[j] = i;
        }
    }
    return 0;
}();