#include <bits/stdc++.h>
using namespace std;

// Manacher: 查找以每个位置为中心的最长回文子串
vector<int> manacher(const string& s) {
    int m = s.size();
    // s[i] = t[2 * i + 2]
    string t(2 * m + 3, '#');
    t[0] = '^';
    t[2 * m + 2] = '$';
    for (int i = 0; i < m; ++i) {
        t[2 * i + 2] = s[i];
    }
    int n = t.size();
    vector half_len(n - 2, 0);
    half_len[1] = 1;
    vector<pair<int, int>> ans;
    // (2 * box_m - box_r, box_r) 是镜像的
    for (int i = 2, box_m = 0, box_r = 0; i < n - 2; ++i) {
        int hl = 1; // t[i] 的最大回文半径
        if (i < box_r) {
            hl = min(half_len[box_m * 2 - i], box_r - i);
        }
        while (t[i + hl] == t[i - hl]) {
            ++hl;
        }
        half_len[i] = hl;
        if (i + hl > box_r) {
            box_m = i;
            box_r = i + hl;
        }
        ans.emplace_back((i - hl) / 2, (i + hl) / 2 - 2);
    }
    return half_len;
}