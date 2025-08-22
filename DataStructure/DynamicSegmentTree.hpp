#include <bits/stdc++.h>
using namespace std;

/*
动态开点线段树，只考虑静态数组实现
使用空间约等于：操作数量 * 树高 * 2，在此基础上适当调大
以 区间增加 + 维护区间和 为例，下标从 1 开始
*/
constexpr int MAXN = 400'000 + 5;
using T = long long;
using F = int;
struct Node {
    T val;
    int left = 0;
    int right = 0;
    F to_add = 0;
} tree[MAXN];
Node& root = tree[1];
int cnt = 0;
int n;

void add_node(int& o) {
    if (o == 0) {
        o = ++cnt;
    }
}

void apply(Node& cur, int l, int r, F f) {
    cur.val += 1LL * (r - l + 1) * f;
    cur.to_add += f;
}

void spread(Node& cur, int l, int r) {
    if (cur.to_add == 0) {
        return;
    }
    int m = (l + r) / 2;
    add_node(cur.left);
    apply(tree[cur.left], l, m, cur.to_add);
    add_node(cur.right);
    apply(tree[cur.right], m + 1, r, cur.to_add);
    cur.to_add = 0;
}

void maintain(Node& cur) {
    // 0 节点作冗余处理
    // cur.val = tree[cur.left].val + tree[cur.right].val;

    if (cur.left == 0) {
        cur.val = cur.right;
    } else if (cur.right == 0) {
        cur.val = cur.left;
    } else {
        cur.val = cur.left + cur.right;
    }
}

void update(Node& cur, int l, int r, int ql, int qr, F f) {
    if (ql <= l && r <= qr) {
        apply(cur, l, r, f);
        return;
    }
    spread(cur, l, r);
    int m = (l + r) / 2;
    if (ql <= m) {
        add_node(cur.left);
        update(tree[cur.left], l, m, ql, qr, f);
    }
    if (qr > m) {
        add_node(cur.right);
        update(tree[cur.right], m + 1, r, ql, qr, f);
    }
    maintain(cur);
}

T query(Node& cur, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return cur.val;
    }
    spread(cur, l, r);
    int m = (l + r) / 2;
    if (qr <= m) {
        add_node(cur.left);
        return query(tree[cur.left], l, m, ql, qr);
    }
    if (ql > m) {
        add_node(cur.right);
        return query(tree[cur.right], m + 1, r, ql, qr);
    }
    add_node(cur.left);
    add_node(cur.right);
    T l_res = query(tree[cur.left], l, m, ql, qr);
    T r_res = query(tree[cur.right], m + 1, r, ql, qr);
    return l_res + r_res;
}

void build(int mx, int init_val) {
    n = mx;
    // tree[o].val = 0;
    tree[++cnt].val = init_val;
}

void update(int ql, int qr, F f) {
    update(root, 1, n, ql, qr, f);
}

T query(int ql, int qr) {
    return query(root, 1, n, ql, qr);
}