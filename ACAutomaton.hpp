#include <bits/stdc++.h>
using namespace std;

// 只记录匹配状态
constexpr int MX = 1'000'001;
int tree[MX][27]; // 末位表示 end
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

// 统计词频
constexpr int MX = 1'000'001;
int tree[MX][27]; // 末位记录索引
int fail[MX];
int cnt, n;
int freq[MX];
void build(const vector<string>& a) {
    // reset
    int sum_len = 0;
    for (auto &s: a) {
        sum_len += s.size();
    }
    for (int m = sum_len, i = 0; i <= m; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        tree[i][26] = -1;
        fail[i] = 0;
        freq[i] = 0;
    }

    // 插入字符串
    cnt = 0;
    n = a.size();
    for (int i = 0; i < n; ++i) {
        auto &s = a[i];
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

vector<int> calc() {
    vector head(cnt + 1, -1), nxt(cnt, -1), to(cnt, -1);
    for (int i = 1; i <= cnt; ++i) {
        int x = fail[i], y = i, idx_e = i - 1;
        nxt[idx_e] = head[x];
        to[idx_e] = y;
        head[x] = idx_e;
    }
    vector ans(n, 0);
    auto dfs = [&](this auto&& dfs, int x) -> int {
        int res = freq[x];
        for (int e = head[x]; e >= 0; e = nxt[e]) {
            res += dfs(nxt[e]);
        }
        if (int idx_v = tree[x][26]; idx_v >= 0) {
            ans[idx_v] = res;
        }
        return res;
    };
    dfs(0);
    return ans;
}