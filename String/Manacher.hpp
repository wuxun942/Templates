#include <bits/stdc++.h>
using namespace std;

// Manacher: 查找以每个位置为中心的最长回文子串
vector<int> manacher(const string& s) {
    int n = s.size();
    // s[i] = t[2 * i + 2]
    string t(2 * n + 3, '#');
    t[0] = '^';
    t[2 * n + 2] = '$';
    for (int i = 0; i < n; ++i) {
        t[2 * i + 2] = s[i];
    }
    int m = t.size();
    vector half_len(m - 2, 0);
    vector<pair<int, int>> ans;
    // (2 * box_m - box_r, box_r) 关于 box_m 镜像
    for (int i = 1, box_m = 0, box_r = 0; i < m - 1; ++i) {
        // hl: t[i] 的最大回文半径，实际回文串长度等于 hl - 1
        int hl = 1;
        if (i < box_r) {
            hl = min(half_len[box_m * 2 - i], box_r - i);
        }
        while (t[i + hl] == t[i - hl]) {
            ++hl;
            // box 向右扩展
            if (i + hl > box_r) {
                box_m = i;
                box_r = i + hl;
            }
        }
        half_len[i] = hl;
        /*
        在 t 中最后得到的回文子串左右端点一定是 #，映射回 s 时需要删掉
        左端点 t[i - hl + 1]，对应 s[(i - hl) / 2]；
        右端点 t[i + hl - 1]，对应 s[(i + hl) / 2 - 2]
        */
        ans.emplace_back((i - hl) / 2, (i + hl) / 2 - 2);
    }
    return half_len;
}