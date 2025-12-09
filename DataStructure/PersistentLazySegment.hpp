#include <bits/stdc++.h>
using namespace std;

/*
可持久化线段树（区间修改）
常用的实现方法有：
1. 仿照单点修改，每次修改只对最上层节点做添加；但懒标记下传时也要添加
2. 标记永久化

通常来说，前者更为通用，但空间占用较大，为 O(n + q * log n)，
需要开 n * 4 + q * 4 * log n，其中 q 为版本数
当然，具体题目中懒标记不一定会传到底，所以 AC 代码的使用空间只可能更小（和特殊情况的动态开点线段树同理）

后者只用于 区间增加 + 维护区间和 的情况，可以有效减少空间占用
*/

// 懒标记下传实现，以 区间增加 + 维护区间和 为例

// 维护数组的最大空间
constexpr int MAX_N = 100'000 + 5;

// 最大版本数量
constexpr int MAX_Q = 100'000 + 5;

// 可持久化 Lazy 线段树的使用空间 = n * 4 + q * 4 * log n
constexpr int MAX_S = MAX_N * 4 + MAX_Q * 4 * 17;

using T = int;
constexpr T INIT = 0;

using F = int;
constexpr F TO_ADD_INIT = 0;

int n;
T arr[MAX_N];

// 空间使用计数
int cnt = 0;

// 每个版本的根节点
int roots[MAX_Q];

T tree[MAX_S];

F to_add[MAX_S];

int ls[MAX_S];
int rs[MAX_S];

// 清空
void clear(int sz = cnt) {
    cnt = 0;
    fill(tree + 1, tree + sz + 1, 0);
    fill(ls + 1, ls + sz + 1, 0);
    fill(rs + 1, rs + sz + 1, 0);
}

// 拷贝节点
int copy_node(int i) {
    tree[++cnt] = tree[i];
    to_add[cnt] = to_add[i];
    ls[cnt] = ls[i];
    rs[cnt] = rs[i];
    return cnt;
}

void apply(int i, int l, int r, F val) {
    tree[i] += (r - l + 1) * val;
    to_add[i] += val;
}

void down(int i, int l, int r) {
    if (to_add[i] == TO_ADD_INIT) {
        return;
    }
    ls[i] = copy_node(ls[i]);
    rs[i] = copy_node(rs[i]);
    int m = (l + r) / 2;
    apply(ls[i], l, m, to_add[i]);
    apply(rs[i], m + 1, r, to_add[i]);
    to_add[i] = 0;
}

T merge_val(T a, T b) {
    return a + b;
}

void up(int i) {
    tree[i] = merge_val(tree[ls[i]], tree[rs[i]]);
}

int build(const T *arr, int l, int r) {
    if (l == r) {
        tree[++cnt] = arr[l];
        to_add[cnt] = TO_ADD_INIT;
        return cnt;
    }
    int i = ++cnt;
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

int update(int i, int l, int r, int ql, int qr, T val) {
    i = copy_node(i);
    if (ql <= l && r <= qr) {
        apply(i, l, r, val);
        return i;
    }
    down(i, l, r);
    int m = (l + r) / 2;
    if (ql <= m) {
        ls[i] = update(ls[i], l, m, ql, qr, val);
    } 
    if (qr > m) {
        rs[i] = update(rs[i], m + 1, r, ql, qr, val);
    }
    up(i);
    return i;
}

// 在以 i 为根的线段树上，把 [ql, qr] 位置加 val，存储为 v 版本
void update(int v, int i, int ql, int qr, T val) {
    roots[v] = update(i, 1, n, ql, qr, val);
}

T query(int i, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[i];
    }
    down(i, l, r);
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

// 在以 i 为根的线段树上，查询 [ql, qr] 的信息，存储为 v 版本
T query(int v, int i, int ql, int qr) {
    roots[v] = i;
    return query(i, 1, n, ql, qr);
}