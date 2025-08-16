#include <bits/stdc++.h>
using namespace std;

// Z 函数: 每个后缀与原串的 LCP
vector<int> calc_z(const string& s) {
    int n = s.size();
    vector z(n, 0);
    for (int i = 1, box_l = 0, box_r = 0; i < n; i++) {
        if (i <= box_r) {
            z[i] = min(z[i - box_l], box_r - i + 1);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            box_l = i;
            box_r = i + z[i]++;
        }
    }
    z[0] = n;
    return z;
}