#include <bits/stdc++.h>
using namespace std;

// 线段树，单点更新 + 区间查询，以找区间最大值为例

// 泛型模板类封装，0-based
template<typename T>
class SegmentTree {
    int n;
    vector<T> tree;
    const T INIT = INT_MIN;

    T merge_val(T a, T b) {
        return max(a, b);
    }

    void up(int i) {
        tree[i] = merge_val(tree[i * 2], tree[i * 2 + 1]);
    }

    void build(const vector<T> &arr, int i, int l, int r) {
        if (l == r) {
            tree[i] = arr[l];
            return;
        }
        int m = (l + r) / 2;
        build(arr, i * 2, l, m);
        build(arr, i * 2 + 1, m + 1, r);
        up(i);
    }

    void update(int i, int l, int r, int qi, T val) {
        if (l == r) {
            tree[i] = val;
            return;
        }
        int m = (l + r) / 2;
        if (qi <= m) {
            update(i * 2, l, m, qi, val);
        } else {
            update(i * 2 + 1, m + 1, r, qi, val);
        }
        up(i);
    }

    T query(int i, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            return tree[i];
        }
        int m = (l + r) / 2;
        T res = INIT;
        if (ql <= m) {
            res = merge_val(res, query(i * 2, l, m, ql, qr));
        }
        if (qr > m) {
            res = merge_val(res, query(i * 2 + 1, m + 1, r, ql, qr));
        }
        return res;
    }

public:
    SegmentTree(int n, T init_val) : SegmentTree(vector<T>(n, init_val)) {}

    SegmentTree(const vector<T> &arr) : n(arr.size()), tree(2 << bit_width(arr.size() - 1)) {
        build(arr, 1, 0, n - 1);
    }

    void update(int qi, T val) {
        update(1, 0, n - 1, qi, val);
    }

    T query(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr);
    }

    T get(int qi) {
        return query(1, 0, n - 1, qi, qi);
    }
};

// 静态数组实现，1-based
constexpr int MAX_N = 100'000 + 5;

using T = int;
constexpr T INIT = INT_MIN;

int arr_size;
T arr[MAX_N];
T tree[MAX_N << 2];

T merge_val(T a, T b) {
    return max(a, b);
}

void up(int i) {
    tree[i] = merge_val(tree[i * 2], tree[i * 2 + 1]);
}

void build(const T *arr, int i, int l, int r) {
    if (l == r) {
        tree[i] = arr[l];
        return;
    }
    int m = (l + r) / 2;
    build(arr, i * 2, l, m);
    build(arr, i * 2 + 1, m + 1, r);
    up(i);
}

void build(const T *arr, int sz) {
    arr_size = sz;
    build(arr, 1, 1, arr_size);
}

void build(int sz, T init_val) {
    fill(arr + 1, arr + sz + 1, init_val);
    build(arr, sz);
}

void update(int i, int l, int r, int qi, T val) {
    if (l == r) {
        tree[i] = val;
        return;
    }
    int m = (l + r) / 2;
    if (qi <= m) {
        update(i * 2, l, m, qi, val);
    } else {
        update(i * 2 + 1, m + 1, r, qi, val);
    }
    up(i);
}

void update(int qi, T val) {
    update(1, 1, arr_size, qi, val);
}

T query(int i, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[i];
    }
    int m = (l + r) / 2;
    T res = INIT;
    if (ql <= m) {
        res = merge_val(res, query(i * 2, l, m, ql, qr));
    }
    if (qr > m) {
        res = merge_val(res, query(i * 2 + 1, m + 1, r, ql, qr));
    }
    return res;
}

T query(int ql, int qr) {
    return query(1, 1, arr_size, ql, qr);
}