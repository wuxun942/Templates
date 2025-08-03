#include <bits/stdc++.h>
using namespace std;

// 埃氏筛筛质数
constexpr int MX = 100'001;
bool is_prime[MX];
vector<int> primes;
auto init = []() {
    ranges::fill(is_prime, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < MX; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = i * i; j < MX; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return 0;
}();

// 欧拉筛预处理 LPF (最小质因子 / 最大质因子)
constexpr int MX = 100'001;
vector<int> primes;
array<int, MX> lpf;
auto init = []() {
    for (int i = 2; i < MX; i++) {
        if (lpf[i] == 0) {
            lpf[i] = i;
            primes.push_back(i);
        }
        for (int p: primes) {
            if (i * p >= MX) {
                break;
            }
            if (lpf[i * p] == 0) {
                lpf[i * p] = p;
            }
            // lpf[i * p] = p;
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
auto init = []() {
    for (int i = 1; i < MX; ++i) {
        for (int j = i; j < MX; j += i) {
            divisors[j].push_back(i);
        }
    }
    return 0;
}();