#include <bits/stdc++.h>
using namespace std;

// 字典树 trie，做题时更推荐使用静态数组实现

// 只考虑26个字母的字典树的静态数组实现
constexpr int MAX_N = 100'000 + 5;
constexpr int MAX_H = 20;
constexpr int MAX_V = MAX_N * (MAX_H + 1);
int tree[MAX_V][27]; // 末位表示 end
int cnt_v = 0;
void clear(int n = cnt_v) {
    cnt_v = 0;
    memset(tree, 0, (n + 1) * 27 * sizeof(int));
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
constexpr int MAX_N = 200'000 + 5;
constexpr int MAX_H = 30;
constexpr int MAX_V = MAX_N * (MAX_H + 1); // 总长度 = 数量 * (最长比特位 + 1)
int tree[MAX_V][2];
int passby[MAX_V];
int cnt_v = 0;

void clear(int n = cnt_v) {
    cnt_v = 0;
    memset(tree, 0, (n + 1) * 2 * sizeof(int));
    fill(passby, passby + cnt_v, 0);
}

// k: 比特位长度，不要减一
void insert(int x, int k = MAX_H) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        int bit = x >> i & 1, &son = tree[cur][bit];
        if (son == 0) {
            son = ++cnt_v;
        }
        cur = son;
        ++passby[cur];
    }
}

// 以找异或最大值为例
int query(int x, int k = MAX_H) {
    int res = 0;
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        int bit = x >> i & 1;
        if (int son = tree[cur][bit ^ 1]; passby[son] > 0) {
            res |= 1 << i;
            cur = son;
        } else {
            cur = tree[cur][bit];
        }
    }
    return res;
}

// 删除节点：不需要直接删除，只需要减少经过次数
void remove(int x, int k = MAX_H) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        cur = tree[cur][x >> i & 1];
        --passby[cur];
    }
}