#include <bits/stdc++.h>
using namespace std;

// Manacher: 查找以每个位置为中心的最长回文子串
vector<int> manacher(const string& s) {
    string t = "^";
    for (char c : s) {
        t += '#';
        t += c;
    }
    t += '$';
    int n = t.size();
    vector half_len(n - 2, 0);
    half_len[1] = 1;
    vector<pair<int, int>> ans;
    for (int i = 2, box_m = 0, box_r = 0; i < n - 2; ++i) {
        int hl = i < box_r ? min(half_len[box_m * 2 - i], box_r - i) : 1;
        while (t[i + hl] == t[i - hl]) {
            box_m = i;
            box_r = i + ++hl;
        }
        half_len[i] = hl;
        ans.emplace_back((i - hl) / 2, (i + hl) / 2 - 2);
    }
    return half_len;
}