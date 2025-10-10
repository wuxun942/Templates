#include <bits/stdc++.h>
using namespace std;

// 生成随机数 [l, r]
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int l, r;
int randint = uniform_int_distribution<>(l, r)(rng);