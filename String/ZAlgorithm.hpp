#include <bits/stdc++.h>
using namespace std;

// Z 函数: 每个后缀与原串的 LCP
vector<int> calc_z(const string& s) {
    int n = s.size();
    vector z(n, 0);
    // [box_l, box_r) = [0, box_r - box_l)
    for (int i = 1, box_l = 0, box_r = 0; i < n; ++i) {
        int lcp = 0;
        if (i < box_r) {
            lcp = min(z[i - box_l], box_r - i);
        }
        while (i + lcp < n && s[z[i]] == s[i + lcp]) {
            ++lcp;
        }
        z[i] = lcp;
        if (i + lcp > box_r) {
            box_l = i;
            box_r = i + lcp;
        }
    }
    z[0] = n;
    return z;
}