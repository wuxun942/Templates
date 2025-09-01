#include <bits/stdc++.h>
using namespace std;

// 预处理回文数，以 [1, 1e9] 范围为例

long long merge_num(long long x, long long y) {
    for (; y; y /= 10) {
        x = x * 10 + y % 10;
    }
    return x;
}

vector<long long> pal;
auto init = []() {
    for (int mid = 0; mid < 10; ++mid) {
        pal.push_back(mid);
    }
    for (int i = 0, base = 1; i < 4; ++i, base *= 10) {
        for (int x = base; x < base * 10; ++x) {
            pal.push_back(merge_num(x, x));
        }
        for (int x = base; x < base * 10; ++x) {
            for (int mid = 0; mid < 10; ++mid) {
                pal.push_back(merge_num(10 * x + mid, x));
            }
        }
    }
    return 0;
}();