#include <bits/stdc++.h>
using namespace std;

// 只考虑命中时报警
constexpr int MX = 1'000'001;
int tree[MX][27];
int fail[MX];
int cnt_e;
void build(const vector<string>& a) {
    int sum_len = 0;
    for (auto &s: a) {
        sum_len += s.size();
    }
    for (int m = sum_len, i = 0; i <= m; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
    }

    cnt_e = 0;
    for (auto &s: a) {
        int cur = 0;
        for (char c: s) {
            int &son = tree[cur][c - 'a'];
            if (son == 0) {
                son = ++cnt_e;
            }
            cur = son;
        }
        tree[cur][26] = 1;
    }

    queue<int> q;
    for (int j = 0; j < 26; ++j) {
        if (tree[0][j] != 0) {
            q.push(tree[0][j]);
        }
    }
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        tree[x][26] |= tree[fail[x]][26]; // 继承命中状态
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

bool search(string& s) {
    int cur = 0;
    for (char c: s) {
        cur = tree[cur][c - 'a'];
        if (tree[cur][26]) { // 命中
            return true;
        }
    }
    return false;
}

// 离线统计词频
constexpr int MX = 1'000'001;
int tree[MX][26];
int fail[MX];
int cnt_e;
int head[MX];
int nxt[MX];
int to[MX];
int cnt[MX];
constexpr int MX2 = 1'000'001;
int end_node[MX2];
int n;
void build(const vector<string>& a) {
    // reset
    int sum_len = 0;
    for (auto &s: a) {
        sum_len += s.size();
    }
    for (int m = sum_len, i = 0; i <= m; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
        head[i] = -1;
        cnt[i] = 0;
    }

    // 插入字符串
    n = a.size();
    cnt_e = 0;
    for (int i = 0; i < n; ++i) {
        auto &s = a[i];
        int cur = 0;
        for (char c: s) {
            int &son = tree[cur][c - 'a'];
            if (son == 0) {
                son = ++cnt_e;
            }
            cur = son;
        }
        end_node[i] = cur;
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

    // 根据 fail 反向建树
    for (int i = 1; i <= cnt_e; ++i) {
        int x = fail[i], y = i;
        nxt[i] = head[x];
        to[i] = y;
        head[x] = i;
    }
}

vector<int> scan(string& s) {
    int cur = 0;
    for (char c: s) {
        cur = tree[cur][c - 'a'];
        ++cnt[cur];
    }
    auto dfs = [](this auto&& dfs, int x) -> void {
        for (int e = head[x], y; e >= 0; e = nxt[e]) {
            y = to[e];
            dfs(y);
            cnt[x] += cnt[y];
        }
    };
    dfs(0);
    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = cnt[end_node[i]];
    }
    return ans;
}