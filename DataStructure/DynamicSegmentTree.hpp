#include <bits/stdc++.h>
using namespace std;

/*
动态开点线段树，只考虑静态数组实现
使用空间约等于：操作数量 * 树高 * 2，在此基础上适当调大
以 区间增加 + 维护区间和 为例，下标从 1 开始
*/

constexpr int MAXN = 400'001;
using T = long long;
using F = int;
struct Node {
    T val;
    int left = 0;
    int right = 0;
    F to_add = 0;
} tree[MAXN];
int cnt = 0;
int n, init_val;

void add_node(int& o) {
    if (o == 0) {
        o = ++cnt;
        tree[o].val = init_val;
    }
}

void apply(int o, int l, int r, F f) {
    Node &cur = tree[o];
    cur.val += 1LL * (r - l + 1) * f;
    cur.to_add += f;
}

void spread(int o, int l, int r) {
    Node &cur = tree[o];
    if (cur.to_add == 0) {
        return;
    }
    int m = (l + r) / 2;
    add_node(cur.left);
    add_node(cur.right);
    apply(cur.left, l, m, cur.to_add);
    apply(cur.right, m + 1, r, cur.to_add);
    cur.to_add = 0;
}

void maintain(int o) {
    Node &cur = tree[o];
    // 0 节点（空节点）作冗余处理
    cur.val = tree[cur.left].val + tree[cur.right].val;

    // 一般写法，需要考虑是否为空节点
    // if (cur.left == 0) {
    //     cur.val = tree[cur.right].val;
    // } else if (cur.right == 0) {
    //     cur.val = tree[cur.left].val;
    // } else {
    //     cur.val = tree[cur.left].val + tree[cur.right].val;
    // }
}

void update(int o, int l, int r, int ql, int qr, F f) {
    Node &cur = tree[o];
    if (ql <= l && r <= qr) {
        apply(o, l, r, f);
        return;
    }
    spread(o, l, r);
    int m = (l + r) / 2;
    if (ql <= m) {
        add_node(cur.left);
        update(cur.left, l, m, ql, qr, f);
    }
    if (qr > m) {
        add_node(cur.right);
        update(cur.right, m + 1, r, ql, qr, f);
    }
    maintain(o);
}

T query(int o, int l, int r, int ql, int qr) {
    Node &cur = tree[o];
    if (ql <= l && r <= qr) {
        return cur.val;
    }
    spread(o, l, r);
    int m = (l + r) / 2;
    if (qr <= m) {
        add_node(cur.left);
        return query(cur.left, l, m, ql, qr);
    }
    if (ql > m) {
        add_node(cur.right);
        return query(cur.right, m + 1, r, ql, qr);
    }
    add_node(cur.left);
    add_node(cur.right);
    return query(cur.left, l, m, ql, qr) + query(cur.right, m + 1, r, ql, qr);
}

void build(int mx, T val) {
    n = mx;
    cnt = 1;
    tree[1].val = init_val = val;
    tree[0].val = 0; // 作冗余处理
}

void update(int ql, int qr, F f) {
    update(1, 1, n, ql, qr, f);
}

T query(int ql, int qr) {
    return query(1, 1, n, ql, qr);
}