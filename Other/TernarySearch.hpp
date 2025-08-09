#include <bits/stdc++.h>
using namespace std;

// 三分查找，以下是单峰函数 + 闭区间的写法，单谷函数需要把不等号反向
int ternary_search(const vector<int>& arr) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int m1 = l + (r - l) / 3;
        int m2 = r - (r - l) / 3;
        if (arr[m1] < arr[m2]) {
            l = m1 + 1;
        } else {
            r = m2 - 1;
        }
    }
    return l;
}