#include <bits/stdc++.h>
using namespace std;

/*
可持久化线段树（单点修改）：以查找最大值为例
一般来说，初始数组是 0 号版本
*/

// 维护数组的最大空间
constexpr int MAX_N = 100'000 + 5;

// 最大查询次数
constexpr int MAX_Q = 100'000 + 5;

// 可持久化线段树的使用空间 = n * 4 + q * log n
constexpr int MAX_M = MAX_N * 4 + MAX_Q * 17;

using T = int;
constexpr T INIT = INT_MIN;

int n;
T arr[MAX_N];

// 空间使用计数
int cnt = 0;

// 每个版本的根节点
int roots[MAX_Q];

T tree[MAX_M];

int ls[MAX_M];
int rs[MAX_M];

int copy(int i) {
    tree[++cnt] = tree[i];
    ls[cnt] = ls[i];
    rs[cnt] = rs[i];
    return cnt;
}

T merge_val(T a, T b) {
    return max(a, b);
}

void up(int i) {
    tree[i] = merge_val(tree[ls[i]], tree[rs[i]]);
}

int build(const T *arr, int l, int r) {
    if (l == r) {
        tree[++cnt] = arr[l];
        return cnt;
    }
    int i = ++cnt;
    int m = (l + r) / 2;
    ls[i] = build(arr, l, m);
    rs[i] = build(arr, m + 1, r);
    up(i);
    return i;
}

void build(const T *arr, int arr_size) {
    n = arr_size;
    roots[0] = build(arr, 1, n);
}

void build(int sz, T init_val) {
    fill(arr + 1, arr + sz + 1, init_val);
    build(arr, sz);
}

int update(int i, int l, int r, int qi, T val) {
    i = copy(i);
    if (l == r) {
        tree[i] = val;
        return i;
    }
    int m = (l + r) / 2;
    if (qi <= m) {
        ls[i] = update(ls[i], l, m, qi, val);
    } else {
        rs[i] = update(rs[i], m + 1, r, qi, val);
    }
    up(i);
    return i;
}

void update(int v, int i, int qi, T val) {
    roots[v] = update(i, 1, n, qi, val);
}

T query(int i, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[i];
    }
    int m = (l + r) / 2;
    T res = INIT;
    if (ql <= m) {
        res = merge_val(res, query(ls[i], l, m, ql, qr));
    }
    if (qr > m) {
        res = merge_val(res, query(rs[i], m + 1, r, ql, qr));
    }
    return res;
}

T query(int v, int i, int ql, int qr) {
    roots[v] = i;
    return query(i, 1, n, ql, qr);
}