#include <bits/stdc++.h>
using namespace std;

// KMP: 查找所有匹配位置，以匹配首字母为基准
vector<int> kmp(string text, string pattern) {
    int m = pattern.size();
    int pi[m]{};
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && pattern[j] != pattern[i]) {
            j = pi[j - 1];
        }
        if (pattern[j] == pattern[i]) j++;
        pi[i] = j;
    }
    vector<int> res;
    for (int i = 0, j = 0, n = text.size(); i < n; i++) {
        while (j > 0 && pattern[j] != text[i]) {
            j = pi[j - 1];
        }
        if (pattern[j] == text[i]) j++;
        if (j == m) {
            res.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return res;
}

// Z 函数: 每个位置的 LCP
vector<int> calc_z(string s) {
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