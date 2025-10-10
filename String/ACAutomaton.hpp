#include <bits/stdc++.h>
using namespace std;

// 朴素 AC 自动机
constexpr int MAXL = 1'000'001;
int tree[MAXL][27]; // 末位表示 end
int fail[MAXL];
int last[MAXL]; // 上一个匹配位置
int cnt_v;
int q[MAXL];
void build(const vector<string>& a) {
    // 初始化
    int sum_len = 0;
    for (auto &s : a) {
        sum_len += s.size();
    }
    for (int i = 0; i <= sum_len; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
    }

    // 插入（同字典树）
    cnt_v = 0;
    for (auto &s : a) {
        int cur = 0;
        for (char c : s) {
            int &son = tree[cur][c - 'a'];
            if (son == 0) {
                son = ++cnt_v;
            }
            cur = son;
        }
        tree[cur][26] = 1;
    }

    // 构建 fail 指针
    int l = -1, r = 0;
    for (int j = 0; j < 26; ++j) {
        if (tree[0][j] != 0) {
            q[r++] = tree[0][j];
        }
    }
    while (l + 1 < r) {
        int x = q[++l];
        for (int j = 0; j < 26; ++j) {
            if (tree[x][j] == 0) {
                tree[x][j] = tree[fail[x]][j];
            } else {
                fail[tree[x][j]] = tree[fail[x]][j];
                last[tree[x][j]] = tree[fail[tree[x][j]]][26] ? fail[tree[x][j]] : last[fail[tree[x][j]]];
                q[r++] = tree[x][j];
            }
        }
    }
}

// 读文章，命中时报警
bool search(const string& s) {
    int cur = 0;
    for (char c : s) {
        cur = tree[cur][c - 'a'];
        for (int ptr = cur; ptr > 0; ptr = last[ptr]) {
            if (tree[ptr][26]) {
                return true;
            }
        }
    }
    return false;
}

// 只考虑命中时报警
constexpr int MAXL = 1'000'001;
int tree[MAXL][27]; // 末位表示 end
int fail[MAXL];
int q[MAXL];
int cnt_v;
void build(const vector<string> &a) {
    // 初始化
    int sum_len = 0;
    for (auto &s : a) {
        sum_len += s.size();
    }
    for (int i = 0; i <= sum_len; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
    }

    // 插入（同字典树）
    cnt_v = 0;
    for (auto &s : a) {
        int cur = 0;
        for (char c : s) {
            int &son = tree[cur][c - 'a'];
            if (son == 0) {
                son = ++cnt_v;
            }
            cur = son;
        }
        tree[cur][26] = 1;
    }

    // 构建 fail 指针
    int l = -1, r = 0;
    for (int j = 0; j < 26; ++j) {
        if (tree[0][j] != 0) {
            q[r++] = tree[0][j];
        }
    }
    while (l + 1 < r) {
        int x = q[++l];
        tree[x][26] |= tree[fail[x]][26]; // 等价位置也要有 end 标记
        for (int j = 0; j < 26; ++j) {
            if (tree[x][j] == 0) {
                tree[x][j] = tree[fail[x]][j];
            } else {
                fail[tree[x][j]] = tree[fail[x]][j];
                q[r++] = tree[x][j];
            }
        }
    }
}

// 读文章，命中时报警
bool search(const string &s) {
    int cur = 0;
    for (char c : s) {
        cur = tree[cur][c - 'a'];
        if (tree[cur][26]) {
            return true;
        }
    }
    return false;
}

// 扫描文章 + 离线统计词频
constexpr int MAXL = 1'000'001; // 模式串长度总和
int tree[MAXL][26];
int fail[MAXL];
int cnt_v;
int q[MAXL];
int head[MAXL];
int nxt[MAXL];
int to[MAXL];
int cnt[MAXL];
constexpr int MAXN = 1'000'001; // 模式串个数
int end_node[MAXN];
int n;
void build(const vector<string> &a) {
    // 初始化
    int sum_len = 0;
    for (auto &s : a) {
        sum_len += s.size();
    }
    for (int i = 0; i <= sum_len; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
        head[i] = -1;
        cnt[i] = 0;
    }

    // 插入（同字典树）
    n = a.size();
    cnt_v = 0;
    for (int i = 0; i < n; ++i) {
        auto &s = a[i];
        int cur = 0;
        for (char c : s) {
            int &son = tree[cur][c - 'a'];
            if (son == 0) {
                son = ++cnt_v;
            }
            cur = son;
        }
        end_node[i] = cur;
    }

    // 构建 fail 指针
    int l = -1, r = 0;
    for (int j = 0; j < 26; ++j) {
        if (tree[0][j] != 0) {
            q[r++] = tree[0][j];
        }
    }
    while (l + 1 < r) {
        int x = q[++l];
        for (int j = 0; j < 26; ++j) {
            if (tree[x][j] == 0) {
                tree[x][j] = tree[fail[x]][j];
            } else {
                fail[tree[x][j]] = tree[fail[x]][j];
                q[r++] = tree[x][j];
            }
        }
    }

    // 根据 fail 反向建树（用于离线统计）
    for (int i = 1; i <= cnt_v; ++i) {
        int x = fail[i], y = i;
        nxt[i] = head[x];
        to[i] = y;
        head[x] = i;
    }
}

// 离线统计
void dfs(int x) {
    for (int e = head[x], y; e >= 0; e = nxt[e]) {
        y = to[e];
        dfs(y);
        cnt[x] += cnt[y];
    }
}

vector<int> scan(const string &s) {
    int cur = 0;
    for (char c : s) {
        cur = tree[cur][c - 'a'];
        ++cnt[cur];
    }
    dfs(0);
    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = cnt[end_node[i]];
    }
    return ans;
}