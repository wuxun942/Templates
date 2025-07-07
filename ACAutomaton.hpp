#include <bits/stdc++.h>
using namespace std;

// 标准版 AC 自动机
constexpr int MX = 1'000'001;
int tree[MX][27];
int fail[MX];
int cnt;
void build(const vector<string>& a) {
    // reset
    int sum_len = 0;
    for (auto &s: a) {
        sum_len += s.size();
    }
    for (int m = sum_len, i = 0; i <= m; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
    }

    // 插入字符串
    cnt = 0;
    for (auto &s: a) {
        int cur = 0;
        for (char c: s) {
            int &son = tree[cur][c - 'a'];
            if (son == 0) {
                son = ++cnt;
            }
            cur = son;
        }
        tree[cur][26] = 1;
    }

    // 构建 fail
    queue<int> q;
    for (int j = 0; j < 26; ++j) {
        if (tree[0][j] != 0) {
            q.push(tree[0][j]);
        }
    }
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        tree[x][26] |= tree[fail[x]][26];
        for (int j = 0; j < 26; ++j) {
            if (tree[x][j] == 0) {
                tree[x][j] = tree[fail[x]][j];
            } else {
                fail[tree[x][j]] = tree[fail[x]][j];
                q.push(tree[x][j]);
            }
        }
    }
}