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

// 欧拉筛预处理 LPF (最小质因子 / 最大质因子)
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
            if (lpf[i * p] == 0) {
                lpf[i * p] = p;
            }
            // lpf[i * p] = p;
            // if (i % p == 0) {
            //     break;
            // }
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