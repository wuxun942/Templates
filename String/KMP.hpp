#include <bits/stdc++.h>
using namespace std;

// KMP: 查找所有匹配位置，以匹配首字母为基准
vector<int> kmp(const string& text, const string& pattern) {
    int m = pattern.size();
    vector pi(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        while (j > 0 && pattern[j] != pattern[i]) {
            j = pi[j - 1];
        }
        if (pattern[j] == pattern[i]) {
            ++j;
        }
        pi[i] = j;
    }
    vector<int> res;
    for (int i = 0, j = 0, n = text.size(); i < n; ++i) {
        while (j > 0 && pattern[j] != text[i]) {
            j = pi[j - 1];
        }
        if (pattern[j] == text[i]) {
            ++j;
        }
        if (j == m) {
            res.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return res;
}