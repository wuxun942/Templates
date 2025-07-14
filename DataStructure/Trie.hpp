#include <bits/stdc++.h>
using namespace std;

// 静态数组实现字典树

// 一般字典树
constexpr int MX = 5'000'001;
// 末位为 end
int tree[MX][27];
int cnt;
void init(int n) {
    cnt = 0;
    for (int i = 0; i <= n; ++i) {
        memset(tree[i], 0, sizeof(tree[i]));
    }
}

void insert(string& s) {
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

bool search_word(string& s) {
    int cur = 0;
    for (char c: s) {
        int son = tree[cur][c - 'a'];
        if (son == 0) {
            return false;
        }
        cur = son;
    }
    return tree[cur][26];
}

bool search_prefix(string& s) {
    int cur = 0;
    for (char c: s) {
        int son = tree[cur][c - 'a'];
        if (son == 0) {
            return false;
        }
        cur = son;
    }
    return true;
}

// 0-1 字典树
constexpr int MX = 5'000'001;
int tree[MX][3];
int cnt;

void init(int n) {
    cnt = 0;
    for (int i = 0; i <= n; ++i) {
        tree[i][0] = 0;
        tree[i][1] = 0;
        tree[i][2] = 0; // 经过次数，用于删除节点
    }
}

// k: 比特位长度，不要减一
void insert(int x, int k) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        int bit = x >> i & 1, &son = tree[cur][bit];
        if (son == 0) {
            son = ++cnt;
        }
        cur = son;
        ++tree[cur][2];
    }
}

// 以找最大值为例
int search(int x, int k) {
    int res = 0, cur = 0;
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

void remove(int x, int k) {
    int cur = 0;
    for (int i = k - 1; i >= 0; --i) {
        cur = tree[cur][x >> i & 1];
        --tree[cur][2];
    }
}