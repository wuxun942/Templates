#include <bits/stdc++.h>
using namespace std;

// Lazy 线段树，以 区间增加 + 维护区间和为例

// 泛型模板类封装，0-based
template<typename T, typename F = T>
class LazySegmentTree {
    const T INIT = 0;
    const F TO_ADD_INIT = 0;

    struct Node {
        T val;
        F to_add;
    };

    int n;
    vector<Node> tree;

    T merge_val(T a, T b) {
        return a + b;
    }

    F merge_todo(F a, F b) {
        return a + b;
    }

    // 懒标记应用于当前节点
    void apply(int i, int l, int r, F to_add) {
        Node &cur = tree[i];
        cur.val += to_add * (r - l + 1);
        cur.to_add = merge_todo(cur.to_add, to_add);
    }

    // 懒标记下传
    void down(int i, int l, int r) {
        Node &cur = tree[i];
        if (cur.to_add == TO_ADD_INIT) {
            return;
        }
        int m = (l + r) / 2;
        apply(i * 2, l, m, cur.to_add);
        apply(i * 2 + 1, m + 1, r, cur.to_add);
        cur.to_add = TO_ADD_INIT;
    }

    void up(int i) {
        tree[i].val = merge_val(tree[i * 2].val, tree[i * 2 + 1].val);
    }

    void build(const vector<T> &arr, int i, int l, int r) {
        Node &cur = tree[i];
        cur.to_add = TO_ADD_INIT;
        if (l == r) {
            cur.val = arr[l];
            return;
        }
        int m = (l + r) / 2;
        build(arr, i * 2, l, m);
        build(arr, i * 2 + 1, m + 1, r);
        up(i);
    }

    void update(int i, int l, int r, int ql, int qr, F f) {
        if (ql <= l && r <= qr) {
            apply(i, l, r, f);
            return;
        }
        down(i, l, r);
        int m = (l + r) / 2;
        if (ql <= m) {
            update(i * 2, l, m, ql, qr, f);
        }
        if (qr > m) {
            update(i * 2 + 1, m + 1, r, ql, qr, f);
        }
        up(i);
    }

    T query(int i, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            return tree[i].val;
        }
        down(i, l, r);
        int m = (l + r) / 2;
        T res = INIT;
        if (ql <= m) {
            res = merge_val(res, query(i * 2, l, m, ql, qr));
        }
        if (qr > m) {
            return merge_val(res, query(i * 2 + 1, m + 1, r, ql, qr));
        }
        return res;
    }
    
public:
    LazySegmentTree(int n, T init_val) : LazySegmentTree(vector<T>(n, init_val)) {}

    LazySegmentTree(const vector<T> &arr) : n(arr.size()), tree(2 << bit_width(arr.size() - 1)) {
        build(arr, 1, 0, n - 1);
    }

    void update(int ql, int qr, F f) {
        if (ql > qr) {
            return;
        }
        update(1, 0, n - 1, ql, qr, f);
    }

    T query(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr);
    }
};

// 静态数组实现，1-based
constexpr int MAX_N = 100'000 + 5;

using T = long long;
constexpr T INIT = 0;

using F = long long;
constexpr F TO_ADD_INIT = 0;

int n;
T arr[MAX_N];
T tree[MAX_N << 2];
F to_add[MAX_N << 2];

T merge_val(T a, T b) {
    return a + b;
}

F merge_todo(F a, F b) {
    return a + b;
}

// 懒标记应用于当前节点
void apply(int i, int l, int r, F v) {
    tree[i] += v * (r - l + 1);
    to_add[i] = merge_todo(to_add[i], v);
}

// 懒标记下传
void down(int i, int l, int r) {
    if (to_add[i] == TO_ADD_INIT) {
        return;
    }
    int m = (l + r) / 2;
    apply(i * 2, l, m, to_add[i]);
    apply(i * 2 + 1, m + 1, r, to_add[i]);
    to_add[i] = TO_ADD_INIT;
}

void up(int i) {
    tree[i] = merge_val(tree[i * 2], tree[i * 2 + 1]);
}

void build(const T *arr, int i, int l, int r) {
    to_add[i] = TO_ADD_INIT;
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
    n = sz;
    build(arr, 1, 1, n);
}

void build(int sz, T init_val) {
    fill(arr + 1, arr + sz + 1, init_val);
    build(arr, sz);
}

void update(int i, int l, int r, int ql, int qr, F v) {
    if (ql <= l && r <= qr) {
        apply(i, l, r, v);
        return;
    }
    down(i, l, r);
    int m = (l + r) / 2;
    if (ql <= m) {
        update(i * 2, l, m, ql, qr, v);
    }
    if (qr > m) {
        update(i * 2 + 1, m + 1, r, ql, qr, v);
    }
    up(i);
}

void update(int ql, int qr, F f) {
    if (ql > qr) {
        return;
    }
    update(1, 1, n, ql, qr, f);
}

T query(int i, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[i];
    }
    down(i, l, r);
    int m = (l + r) / 2;
    T res = INIT;
    if (ql <= m) {
        res = merge_val(res, query(i * 2, l, m, ql, qr));
    }
    if (qr > m) {
        return merge_val(res, query(i * 2 + 1, m + 1, r, ql, qr));
    }
    return res;
}

T query(int ql, int qr) {
    return query(1, 1, n, ql, qr);
}

/*
标记永久化
对于 区间增加 + 查询区间和 这种情况，前者的影响在后者中是可加的，
可以通过 标记永久化 完成，替代懒更新机制
*/

// 泛型模板类封装

template<typename T, typename F = T>
class LazySegmentTree {
    const T INIT = 0;
    const T TO_ADD_INIT = 0;

    struct Node {
        T val;
        F tag;
    };

    int n;
    vector<Node> tree;

    T merge_val(T a, T b) {
        return a + b;
    }

    F merge_tag(F a, F b) {
        return a + b;
    }

    void build(const vector<T> &arr, int i, int l, int r) {
        Node &cur = tree[i];
        if (l == r) {
            cur.val = arr[l];
            cur.tag = TO_ADD_INIT;
            return;
        }
        int m = (l + r) / 2;
        build(arr, i * 2, l, m);
        build(arr, i * 2 + 1, m + 1, r);
        tree[i].val = merge_val(tree[i * 2].val, tree[i * 2 + 1].val);
    }

    /*
    区间更新时，分三种情况：
    1. 更新区间的父亲节点：只更新区间和信息
    2. 更新区间本身：更新区间和信息和标记信息
    3. 其他区间：不更新

    同时，这个过程不需要 push_up
    */
    void update(int i, int l, int r, int ql, int qr, F f) {
        Node &cur = tree[i];
        cur.val = merge_val(cur.val, (min(r, qr) - max(l, ql) + 1) * f);
        if (ql <= l && r <= qr) {
            cur.tag = merge_tag(cur.tag, f);
            return;
        }
        int m = (l + r) / 2;
        if (ql <= m) {
            update(i * 2, l, m, ql, qr, f);
        }
        if (qr > m) {
            update(i * 2 + 1, m + 1, r, ql, qr, f);
        }
    }

    // 查询时，需要将所有标记信息自顶向下传
    T query(int i, int l, int r, int ql, int qr, T to_add) {
        Node &cur = tree[i];
        if (ql <= l && r <= qr) {
            return cur.val + to_add * (r - l + 1);
        }
        int m = (l + r) / 2;
        T res = INIT;
        if (ql <= m) {
            res = merge_val(res, query(i * 2, l, m, ql, qr, to_add + cur.tag));
        }
        if (qr > m) {
            res = merge_val(res, query(i * 2 + 1, m + 1, r, ql, qr, to_add + cur.tag));
        }
        return res;
    }
    
public:
    LazySegmentTree(int n, T init_val) : LazySegmentTree(vector<T>(n, init_val)) {}

    LazySegmentTree(const vector<T> &arr) : n(arr.size()), tree(2 << bit_width(arr.size() - 1)) {
        build(arr, 1, 0, n - 1);
    }

    void update(int ql, int qr, F f) {
        if (ql > qr) {
            return;
        }
        update(1, 0, n - 1, ql, qr, f);
    }

    T query(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr, TO_ADD_INIT);
    }
};

// 静态数组实现
constexpr int MAX_N = 100'000 + 5;

using T = long long;
constexpr T INIT = 0;
constexpr T TO_ADD_INIT = 0;

int n;
T arr[MAX_N];

T tree[MAX_N << 2];
T add_tag[MAX_N << 2];

T merge_val(T a, T b) {
    return a + b;
}

void build(const T *arr, int i, int l, int r) {
    if (l == r) {
        tree[i] = arr[l];
        add_tag[i] = TO_ADD_INIT;
        return;
    }
    int m = (l + r) / 2;
    build(arr, i * 2, l, m);
    build(arr, i * 2 + 1, m + 1, r);
    tree[i] = merge_val(tree[i * 2], tree[i * 2 + 1]);
}

void build(const T *arr, int sz) {
    n = sz;
    build(arr, 1, 1, n);
}

void build(int sz, T init_val) {
    fill(arr + 1, arr + sz + 1, init_val);
    build(arr, sz);
}

/*
区间更新时，分三种情况：
1. 更新区间的父亲节点：只更新区间和信息
2. 更新区间本身：更新区间和信息和标记信息
3. 其他区间：不更新

同时，这个过程不需要 push_up
*/
void update(int i, int l, int r, int ql, int qr, T val) {
    tree[i] += (min(r, qr) - max(l, ql) + 1) * val;
    if (ql <= l && r <= qr) {
        add_tag[i] += val;
        return;
    }
    int m = (l + r) / 2;
    if (ql <= m) {
        update(i * 2, l, m, ql, qr, val);
    }
    if (qr > m) {
        update(i * 2 + 1, m + 1, r, ql, qr, val);
    }
}

void update(int ql, int qr, T val) {
    update(1, 1, n, ql, qr, val);
}

// 查询时，需要将所有标记信息自顶向下传
T query(int i, int l, int r, int ql, int qr, T to_add) {
    if (ql <= l && r <= qr) {
        return tree[i] + to_add * (r - l + 1);
    }
    int m = (l + r) / 2;
    T res = INIT;
    if (ql <= m) {
        res = merge_val(res, query(i * 2, l, m, ql, qr, to_add + add_tag[i]));
    }
    if (qr > m) {
        res = merge_val(res, query(i * 2 + 1, m + 1, r, ql, qr, to_add + add_tag[i]));
    }
    return res;
}

T query(int ql, int qr) {
    return query(1, 1, n, ql, qr, TO_ADD_INIT);
}