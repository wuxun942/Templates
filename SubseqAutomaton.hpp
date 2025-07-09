#include <bits/stdc++.h>
using namespace std;

// 字符集较小的情况，直接将字符集作为维度
vector<array<int, 26>> right, left;
void build(string& s) {
    int n = s.size();
    right.resize(n);
    left.resize(n);
    right[n - 1].fill(n);
    for (int i = n - 2; i >= 0; --i) {
        right[i] = right[i + 1];
        right[i][s[i + 1] - 'a'] = i + 1;
    }
    left[0].fill(-1);
    for (int i = 1; i < n; ++i) {
        left[i] = left[i - 1];
        left[i][s[i - 1] - 'a'] = i - 1;
    }
}