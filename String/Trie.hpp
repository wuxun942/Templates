#include <bits/stdc++.h>
using namespace std;

// 字典树，做题时更推荐使用静态数组实现

// 只考虑26个字母的字典树的静态数组实现
constexpr int MAXL = 5'000'001;
int trie[MAXL][27]; // 末位表示 end
int cnt_v;
void clear(int n = cnt_v) {
    cnt_v = 0;
    memset(trie, 0, (n + 1) * 27 * sizeof(int));
}

void clear(const vector<string> &a) {
    int sum_len = 0;
    for (auto& s : a) {
        sum_len += s.size();
    }
    clear(sum_len);
}

void insert(const string &s) {
    int cur = 0;
    for (char c : s) {
        int &son = trie[cur][c - 'a'];
        if (son == 0) {
            son = ++cnt_v;
        }
        cur = son;
    }
    trie[cur][26] = 1;
}

bool search_word(const string &s) {
    int cur = 0;
    for (char c : s) {
        int son = trie[cur][c - 'a'];
        if (son == 0) {
            return false;
        }
        cur = son;
    }
    return trie[cur][26];
}

bool search_prefix(const string &s) {
    int cur = 0;
    for (char c : s) {
        int son = trie[cur][c - 'a'];
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
int trie[MAXL][3];
int cnt_v;

void clear(int n = cnt_v) {
    cnt_v = 0;
    memset(trie, 0, (n + 1) * 3 * sizeof(int));
}

void clear(const vector<T> &a) {
    int mx = 0;
    for (T x : a) {
        mx = max(mx, bit_width((unsigned) x));
    }
    clear(mx * a.size());
}

// k: 比特位长度，不要减一
void insert(T x, int k) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        int bit = x >> i & 1, &son = trie[cur][bit];
        if (son == 0) {
            son = ++cnt_v;
        }
        cur = son;
        ++trie[cur][2];
    }
}

// 以找异或最大值为例
T search(T x, int k) {
    T res = 0;
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        int bit = x >> i & 1;
        if (int son = trie[cur][bit ^ 1]; trie[son][2] > 0) {
            res = res * 2 + 1;
            cur = son;
        } else {
            cur = trie[cur][bit];
            res = res * 2;
        }
    }
    return res;
}

// 删除节点
void remove(T x, int k) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        cur = trie[cur][x >> i & 1];
        --trie[cur][2];
    }
}