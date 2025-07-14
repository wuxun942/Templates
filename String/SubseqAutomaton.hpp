#include <bits/stdc++.h>
using namespace std;

// 子序列自动机：字符集较小的情况，直接将字符集作为维度
vector<array<int, 26>> r, l;
void build(string& s) {
    int n = s.size();
    r.resize(n);
    l.resize(n);
    r[n - 1].fill(n);
    for (int i = n - 2; i >= 0; --i) {
        r[i] = r[i + 1];
        r[i][s[i + 1] - 'a'] = i + 1;
    }
    l[0].fill(-1);
    for (int i = 1; i < n; ++i) {
        l[i] = l[i - 1];
        l[i][s[i - 1] - 'a'] = i - 1;
    }
}