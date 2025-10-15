#include <bits/stdc++.h>
using namespace std;

/*
给定数位长度，生成回文数
以十进制为例，其他进制只需要修改数位范围和取模即可
*/ 

constexpr int LEN = 9; // 数位长度
constexpr int D = 10; // 进制
vector<long long> pal;

// first_half 为前一半（含中间），last_half 为未反转的后一半
long long merge_number(long long first_half, long long last_half) {
    for (; last_half; last_half /= D) {
        first_half = first_half * D + last_half % D;
    }
    return first_half;
}

auto init = []() {
    for (int mid = 0; mid < D; ++mid) {
        pal.push_back(mid);
    }
    long long base = 1;
    for (int i = 2; i <= LEN; ++i) {
        if (i % 2) {
            for (long long half = base; half < base * D; ++half) {
                for (int mid = 0; mid < D; ++mid) {
                    pal.push_back(merge_number(half * D + mid, half));
                }
            }
        } else {
            base *= D;
            for (long long half = base; half < base * D; ++half) {
                for (int mid = 0; mid < D; ++mid) {
                    pal.push_back(merge_number(half, half));
                }
            }
        }
    }
    return 0;
}();