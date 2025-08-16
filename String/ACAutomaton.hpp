#include <bits/stdc++.h>
using namespace std;

// 朴素 AC 自动机

// 静态数组实现
constexpr int MX = 1'000'001;
int tree[MX][27]; // 末位表示 end
int fail[MX];
int cnt_e;
void build(const vector<string>& a) {
    // 初始化
    int sum_len = 0;
    for (auto &s: a) {
        sum_len += s.size();
    }
    for (int i = 0; i <= sum_len; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
    }

    // 插入（同字典树）
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

    // 构建 fail 指针
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

// 读文章，命中时报警
bool search(const string& s) {
    int cur = 0;
    for (char c: s) {
        cur = tree[cur][c - 'a'];
        for (int ptr = cur; ptr > 0; ptr = fail[ptr]) {
            if (tree[ptr][26]) {
                return true;
            }
        }
    }
    return false;
}

// 结构体封装 + vector 实现
struct AhoCorasick {
    vector<array<int, 27>> tree; // 末位表示 end
    vector<int> fail;
    int cnt_e = 0;

    // 输入字符串长度总和
    AhoCorasick(int sum_len) {
        tree.resize(sum_len);
        fail.resize(sum_len);
    }

    void build(const vector<string>& a) {
        // 插入（同字典树）
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

        // 构建 fail 指针
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

    // 读文章，命中时报警
    bool search(const string& s) {
        int cur = 0;
        for (char c: s) {
            cur = tree[cur][c - 'a'];
            for (int ptr = cur; ptr > 0; ptr = fail[ptr]) {
                if (tree[ptr][26]) {
                    return true;
                }
            }
        }
        return false;
    }
};

// 只考虑命中时报警
constexpr int MX = 1'000'001;
int tree[MX][27]; // 末位表示 end
int fail[MX];
int cnt_e;
void build(const vector<string>& a) {
    // 初始化
    int sum_len = 0;
    for (auto &s: a) {
        sum_len += s.size();
    }
    for (int i = 0; i <= sum_len; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
        fail[i] = 0;
    }

    // 插入（同字典树）
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

    // 构建 fail 指针
    queue<int> q;
    for (int j = 0; j < 26; ++j) {
        if (tree[0][j] != 0) {
            q.push(tree[0][j]);
        }
    }
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        tree[x][26] |= tree[fail[x]][26]; // 等价位置也要有 end 标记
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

// 读文章，命中时报警
bool search(const string& s) {
    int cur = 0;
    for (char c: s) {
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
int cnt_e;
int head[MAXL];
int nxt[MAXL];
int to[MAXL];
int cnt[MAXL];
constexpr int MAXN = 1'000'001; // 模式串个数
int end_node[MAXN];
int n;
void build(const vector<string>& a) {
    // 初始化
    int sum_len = 0;
    for (auto &s: a) {
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

    // 构建 fail 指针
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

    // 根据 fail 反向建树（用于离线统计）
    for (int i = 1; i <= cnt_e; ++i) {
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

vector<int> scan(const string& s) {
    int cur = 0;
    for (char c: s) {
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