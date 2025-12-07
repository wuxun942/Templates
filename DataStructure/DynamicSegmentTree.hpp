#include <bits/stdc++.h>
using namespace std;

/*
动态开点线段树，只考虑静态数组实现
使用空间约等于：操作数量 * 树高 * 2，在此基础上适当调大
以 区间增加 + 维护区间和 为例，下标从 1 开始
*/

constexpr int MAX_N = 400'000 + 5;

using T = long long;
constexpr T INIT = 0;

using F = int;

int n, init_val;
int cnt = 0;

T tree[MAX_N];

int ls[MAX_N];
int rs[MAX_N];

F to_add[MAX_N];

void clear(int sz = cnt) {
    fill(tree, tree + sz + 1, 0);
    fill(ls, ls + sz + 1, 0);
    fill(rs, rs + sz + 1, 0);
    fill(to_add, to_add + sz + 1, 0);
}

int add_node(int val = init_val) {
    tree[++cnt] = val;
    return cnt;
}

T merge_val(T a, T b) {
    return a + b;
}

void apply(int i, int l, int r, F f) {
    tree[i] += 1LL * (r - l + 1) * f;
    to_add[i] += f;
}

void down(int i, int l, int r) {
    if (to_add[i] == 0) {
        return;
    }
    int m = (l + r) / 2;
    if (ls[i] == 0) {
        ls[i] = add_node();
    }
    if (rs[i] == 0) {
        rs[i] == add_node();
    }
    apply(ls[i], l, m, to_add[i]);
    apply(rs[i], m + 1, r, to_add[i]);
    to_add[i] = 0;
}

void up(int i) {
    // 0 节点（空节点）作冗余处理
    tree[i] = merge_val(tree[ls[i]], tree[rs[i]]);

    // 一般写法，需要考虑是否为空节点
    // if (ls[i] == 0) {
    //     tree[i] = tree[rs[i]];
    // } else if (cur.right == 0) {
    //     tree[i] = tree[ls[i]];
    // } else {
    //     tree[i] = merge_val(tree[ls[i]], tree[rs[i]]);
    // }
}

void update(int i, int l, int r, int ql, int qr, F f) {
    if (ql <= l && r <= qr) {
        apply(i, l, r, f);
        return;
    }
    down(i, l, r);
    int m = (l + r) / 2;
    if (ql <= m) {
        if (ls[i] == 0) {
            ls[i] = add_node();
        }
        update(ls[i], l, m, ql, qr, f);
    }
    if (qr > m) {
        if (rs[i] == 0) {
            rs[i] = add_node();
        }
        update(rs[i], m + 1, r, ql, qr, f);
    }
    up(i);
}

T query(int i, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[i];
    }
    down(i, l, r);
    int m = (l + r) / 2;
    T res = INIT;
    if (ql <= m) {
        if (ls[i] == 0) {
            ls[i] = add_node();
        }
        res = merge_val(res, query(ls[i], l, m, ql, qr));
    }
    if (qr > m) {
        if (rs[i] == 0) {
            rs[i] = add_node();
        }
        res = merge_val(res, query(rs[i], m + 1, r, ql, qr));
    }
    return res;
}

void build(int sz, T val) {
    n = sz;
    cnt = 1;
    tree[1] = val; // 根节点初始化
    init_val = val;
    // tree[0] = 0; // 作冗余处理
}

void update(int ql, int qr, F f) {
    update(1, 1, n, ql, qr, f);
}

T query(int ql, int qr) {
    return query(1, 1, n, ql, qr);
}