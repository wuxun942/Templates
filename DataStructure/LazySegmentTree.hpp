#include <bits/stdc++.h>
using namespace std;

// Lazy 线段树，以 区间增加 + 维护区间和为例

// 泛型模板类封装，0-based
template<typename T, typename F>
class LazySegmentTree {
    const F TODO_INIT = 0;

    struct Node {
        T val;
        F todo;
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
    void apply(int o, int l, int r, F todo) {
        Node &cur = tree[o];
        cur.val += todo * (r - l + 1);
        cur.todo = merge_todo(cur.todo, todo);
    }

    // 懒标记下传
    void spread(int o, int l, int r) {
        Node &cur = tree[o];
        if (cur.todo == TODO_INIT) {
            return;
        }
        int m = (l + r) / 2;
        apply(o << 1, l, m, cur.todo);
        apply(o << 1 | 1, m + 1, r, cur.todo);
        cur.todo = TODO_INIT;
    }

    void maintain(int o) {
        tree[o].val = merge_val(tree[o << 1].val, tree[o << 1 | 1].val);
    }

    void build(const vector<T> &arr, int o, int l, int r) {
        Node &cur = tree[o];
        cur.todo = TODO_INIT;
        if (l == r) {
            cur.val = arr[l];
            return;
        }
        int m = (l + r) / 2;
        build(arr, o << 1, l, m);
        build(arr, o << 1 | 1, m + 1, r);
        maintain(o);
    }

    void update(int o, int l, int r, int ql, int qr, F f) {
        if (ql <= l && r <= qr) {
            apply(o, l, r, f);
            return;
        }
        spread(o, l, r);
        int m = (l + r) / 2;
        if (ql <= m) {
            update(o << 1, l, m, ql, qr, f);
        }
        if (qr > m) {
            update(o << 1 | 1, m + 1, r, ql, qr, f);
        }
        maintain(o);
    }

    T query(int o, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            return tree[o].val;
        }
        spread(o, l, r);
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
int n;
using T = int;
using F = int;
T arr[MAX_N];
struct Node {
    T val;
    F todo;
} tree[MAX_N << 2];

constexpr F TODO_INIT = 0;

T merge_val(T a, T b) {
    return a + b;
}

F merge_todo(F a, F b) {
    return a + b;
}

// 懒标记应用于当前节点
void apply(int o, int l, int r, F todo) {
    Node &cur = tree[o];
    cur.val += todo * (r - l + 1);
    cur.todo = merge_todo(cur.todo, todo);
}

// 懒标记下传
void spread(int o, int l, int r) {
    Node &cur = tree[o];
    if (cur.todo == TODO_INIT) {
        return;
    }
    int m = (l + r) / 2;
    apply(o << 1, l, m, cur.todo);
    apply(o << 1 | 1, m + 1, r, cur.todo);
    cur.todo = TODO_INIT;
}

void maintain(int o) {
    tree[o].val = merge_val(tree[o << 1].val, tree[o << 1 | 1].val);
}

void build(const T *arr, int o, int l, int r) {
    Node &cur = tree[o];
    cur.todo = TODO_INIT;
    if (l == r) {
        cur.val = arr[l];
        return;
    }
    int m = (l + r) / 2;
    build(arr, o << 1, l, m);
    build(arr, o << 1 | 1, m + 1, r);
    maintain(o);
}

void update(int o, int l, int r, int ql, int qr, F f) {
    if (ql <= l && r <= qr) {
        apply(o, l, r, f);
        return;
    }
    spread(o, l, r);
    int m = (l + r) / 2;
    if (ql <= m) {
        update(o << 1, l, m, ql, qr, f);
    }
    if (qr > m) {
        update(o << 1 | 1, m + 1, r, ql, qr, f);
    }
    maintain(o);
}

T query(int o, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[o].val;
    }
    spread(o, l, r);
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
    fill(arr, arr + n + 1, init_val);
    build(arr, 1, 1, n);
}

void update(int ql, int qr, F f) {
    if (ql > qr) {
        return;
    }
    update(1, 1, n, ql, qr, f);
}

T query(int ql, int qr) {
    return query(1, 1, n, ql, qr);
}