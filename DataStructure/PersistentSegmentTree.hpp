#include <bits/stdc++.h>
using namespace std;

/*
可持久化：通过节点复用，记录每个版本的信息

可持久化线段树（单点修改）：以查找最大值为例
一般来说，初始数组是 0 号版本
*/

// 维护数组的最大空间
constexpr int MAX_N = 100'000 + 5;

// 最大版本数量
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

// 清空
void clear(int sz = cnt) {
    cnt = 0;
    fill(tree + 1, tree + sz + 1, 0);
    fill(ls + 1, ls + sz + 1, 0);
    fill(rs + 1, rs + sz + 1, 0);
}

int add_node(T x = INIT) {
    tree[++cnt] = x;
    return cnt;
}

// 拷贝节点
int copy_node(int i) {
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
        return add_node(arr[l]);
    }
    int i = add_node();
    int m = (l + r) / 2;
    ls[i] = build(arr, l, m);
    rs[i] = build(arr, m + 1, r);
    up(i);
    return i;
}

// 建立 0 号版本
void build(const T *arr, int arr_size) {
    n = arr_size;
    roots[0] = build(arr, 1, n);
}

void build(int sz, T init_val) {
    fill(arr + 1, arr + sz + 1, init_val);
    build(arr, sz);
}

int update(int i, int l, int r, int qi, T val) {
    i = copy_node(i);
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

// 在 qv 版本的线段树上，把 qi 位置改为 val，存储为 v 版本
void update(int v, int qv, int qi, T val) {
    roots[v] = update(roots[qv], 1, n, qi, val);
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

// 在 qv 版本的线段树上，查询 [ql, qr] 的信息，存储为 v 版本
T query(int v, int qv, int ql, int qr) {
    roots[v] = roots[qv];
    return query(roots[qv], 1, n, ql, qr);
}