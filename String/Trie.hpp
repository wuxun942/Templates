#include <bits/stdc++.h>
using namespace std;

// 字典树，做题时更推荐使用静态数组实现

// 只考虑26个字母的字典树的静态数组实现
constexpr int MAXL = 5'000'001;
int tree[MAXL][27]; // 末位表示 end
int cnt_v;
void init(int n) {
    cnt_v = 0;
    for (int i = 0; i <= n; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
    }
}

void init(const vector<string> &a) {
    int sum_len = 0;
    for (auto& s : a) {
        sum_len += s.size();
    }
    init(sum_len);
}

void insert(const string &s) {
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

bool search_word(const string &s) {
    int cur = 0;
    for (char c : s) {
        int son = tree[cur][c - 'a'];
        if (son == 0) {
            return false;
        }
        cur = son;
    }
    return tree[cur][26];
}

bool search_prefix(const string &s) {
    int cur = 0;
    for (char c : s) {
        int son = tree[cur][c - 'a'];
        if (son == 0) {
            return false;
        }
        cur = son;
    }
    return true;
}

// 0-1 字典树
using T = int;
constexpr int MAXL = 5'000'001; // 最长比特位 * 数量
int tree[MAXL][3];
int cnt_v;

void init(int n) {
    cnt_v = 0;
    for (int i = 0; i <= n; ++i) {
        tree[i][0] = 0;
        tree[i][1] = 0;
        tree[i][2] = 0; // 经过次数，用于删除节点
    }
}

void init(const vector<T> &a) {
    int mx = 0;
    for (T x : a) {
        mx = max(mx, bit_width((unsigned) x));
    }
    init(mx * a.size());
}

// k: 比特位长度，不要减一
void insert(T x, int k) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        int bit = x >> i & 1, &son = tree[cur][bit];
        if (son == 0) {
            son = ++cnt_v;
        }
        cur = son;
        ++tree[cur][2];
    }
}

// 以找异或最大值为例
T search(T x, int k) {
    T res = 0;
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        int bit = x >> i & 1;
        if (int son = tree[cur][bit ^ 1]; tree[son][2] > 0) {
            res = res * 2 + 1;
            cur = son;
        } else {
            cur = tree[cur][bit];
            res = res * 2;
        }
    }
    return res;
}

// 删除节点
void remove(T x, int k) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        cur = tree[cur][x >> i & 1];
        --tree[cur][2];
    }
}