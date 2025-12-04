#include <bits/stdc++.h>
using namespace std;

// 线段树，单点更新 + 区间查询，以找区间最大值为例

// 泛型模板类封装，0-based
template<typename T>
class SegmentTree {
    int n;
    vector<T> tree;

    T merge_val(T a, T b) {
        return max(a, b);
    }

    void maintain(int o) {
        tree[o] = merge_val(tree[o << 1], tree[o << 1 | 1]);
    }

    void build(const vector<T> &arr, int o, int l, int r) {
        if (l == r) {
            tree[o] = arr[l];
            return;
        }
        int m = (l + r) / 2;
        build(arr, o << 1, l, m);
        build(arr, o << 1 | 1, m + 1, r);
        maintain(o);
    }

    void update(int o, int l, int r, int i, T val) {
        if (l == r) {
            tree[o] = val;
            return;
        }
        int m = (l + r) / 2;
        if (i <= m) {
            update(o << 1, l, m, i, val);
        } else {
            update(o << 1 | 1, m + 1, r, i, val);
        }
        maintain(o);
    }

    T query(int o, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            return tree[o];
        }
        int m = (l + r) / 2;
        if (qr <= m) {
            return query(o << 1, l, m, ql, qr);
        }
        if (ql > m) {
            return query(o << 1 | 1, m + 1, r, ql, qr);
        }
        T l_res = query(o << 1, l, m, ql, qr);
        T r_res = query(o << 1 | 1, m + 1, r, ql, qr);
        return merge_val(l_res, r_res);
    }

public:
    SegmentTree(int n, T init_val) : SegmentTree(vector<T>(n, init_val)) {}

    SegmentTree(const vector<T> &arr) : n(arr.size()), tree(2 << bit_width(arr.size() - 1)) {
        build(arr, 1, 0, n - 1);
    }

    void update(int i, T val) {
        update(1, 0, n - 1, i, val);
    }

    T query(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr);
    }

    T get(int i) {
        return query(1, 0, n - 1, i, i);
    }
};

// 静态数组实现，1-based
constexpr int MAX_N = 100'000 + 5;
using T = int;
int n;
T arr[MAX_N];
T tree[MAX_N << 2];

T merge_val(T a, T b) {
    return max(a, b);
}

void maintain(int o) {
    tree[o] = merge_val(tree[o << 1], tree[o << 1 | 1]);
}

void build(const T *arr, int o, int l, int r) {
    if (l == r) {
        tree[o] = arr[l];
        return;
    }
    int m = (l + r) / 2;
    build(arr, o << 1, l, m);
    build(arr, o << 1 | 1, m + 1, r);
    maintain(o);
}

void update(int o, int l, int r, int i, T val) {
    if (l == r) {
        tree[o] = val;
        return;
    }
    int m = (l + r) / 2;
    if (i <= m) {
        update(o << 1, l, m, i, val);
    } else {
        update(o << 1 | 1, m + 1, r, i, val);
    }
    maintain(o);
}

T query(int o, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[o];
    }
    int m = (l + r) / 2;
    if (qr <= m) {
        return query(o << 1, l, m, ql, qr);
    }
    if (ql > m) {
        return query(o << 1 | 1, m + 1, r, ql, qr);
    }
    return merge_val(query(o << 1, l, m, ql, qr), query(o << 1 | 1, m + 1, r, ql, qr));
}

void build(const T *arr, int a_size) {
    n = a_size;
    build(arr, 1, 1, n);
}

void build(int sz, T init_val) {
    n = sz;
    fill(tree, tree + (n << 2) + 1, init_val);
}

void update(int i, T val) {
    update(1, 1, n, i, val);
}

T query(int ql, int qr) {
    return query(1, 1, n, ql, qr);
}