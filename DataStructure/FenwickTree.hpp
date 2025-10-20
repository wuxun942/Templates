#include <bits/stdc++.h>
using namespace std;

// 树状数组，需要注意查询和修改都是 1-index

// 单点更新区间查询（原始） / 区间更新单点查询（直接当差分用）

// 泛型模板类封装
template<typename T>
class FenwickTree {
    vector<T> tree;
public:
    FenwickTree(int n) : tree(n + 1) {}

    FenwickTree(const vector<T> &arr) : tree(arr.size() + 1) {
        int n = arr.size();
        for (int i = 1; i <= n; i++) {
            tree[i] += arr[i - 1];
            if (int nxt = i + (i & -i); nxt <= n) {
                tree[nxt] += tree[i];
            }
        }
    }

    void update(int i, T val) {
        for (; i < tree.size(); i += i & -i) {
            tree[i] += val;
        }
    }

    T query(int i) {
        T res = 0;
        for (; i > 0; i &= i - 1) {
            res += tree[i];
        }
        return res;
    }

    T query(int l, int r) {
        if (l > r) {
            return 0;
        }
        return query(r) - query(l - 1);
    }
};

// 静态数组实现
constexpr int MAXN = 100'001;
using T = int;
T tree[MAXN];
int tree_size;

void build(const T *arr, int n) {
    tree_size = n + 1;
    for (int i = 1; i < tree_size; i++) {
        tree[i] += arr[i - 1];
        if (int nxt = i + (i & -i); nxt < tree_size) {
            tree[nxt] += tree[i];
        }
    }
}

void build(int n) {
    tree_size = n + 1;
    fill(tree, tree + tree_size, 0);
}

void update(int i, T val) {
    for (; i < tree_size; i += i & -i) {
        tree[i] += val;
    }
}

T query(int i) {
    if (i >= tree_size) {
        throw overflow_error("FenwickTree Overflow");
    }
    T res = 0;
    for (; i > 0; i &= i - 1) {
        res += tree[i];
    }
    return res;
}

T query(int l, int r) {
    if (l > r) {
        return 0;
    }
    return query(r) - query(l - 1);
}

// 区间更新区间查询
template<typename T>
class FenwickTree {
    vector<T> info1, info2;

    // 传统初始化方法
    void build(vector<T> &tree, const vector<T> &arr) {
        int n = arr.size();
        for (int i = 1; i <= n; i++) {
            tree[i] += arr[i - 1];
            if (int nxt = i + (i & -i); nxt <= n) {
                tree[nxt] += tree[i];
            }
        }
    }

    // 传统单点更新
    void update(vector<T> &tree, int i, T val) {
        for (; i < tree.size(); i += i & -i) {
            tree[i] += val;
        }
    }

    // 传统单点查询
    T query(const vector<T> &tree, int i) {
        T res = 0;
        for (; i > 0; i &= i - 1) {
            res += tree[i];
        }
        return res;
    }

public:
    FenwickTree(int n): info1(n + 1), info2(n + 1) {}

    FenwickTree(const vector<T> &arr): FenwickTree(arr.size())  {
        int n = arr.size();
        vector<T> diff1(n), diff2(n);
        diff1[0] = arr[0];
        diff2[0] = 0;
        for (int i = 1; i < n; ++i) {
            diff1[i] = arr[i] - arr[i - 1];
            diff2[i] = i * diff1[i];
        }
        build(info1, diff1);
        build(info2, diff2);
    }

    // 区间更新
    void update(int l, int r, T val) {
        update(info1, l, val);
        update(info1, r + 1, -val);
        update(info2, l, (l - 1) * val);
        update(info2, r + 1, -r * val);
    }

    // 区间查询
    T query(int l, int r) {
        return query(info1, r) * r - query(info1, l - 1) * (l - 1) - query(info2, r) + query(info2, l - 1);
    }
};

// 静态数组实现
using T = int;
constexpr int MAXN = 100'001;
T info1[MAXN], info2[MAXN];
T diff1[MAXN], diff2[MAXN];
int tree_size;

// 传统初始化方法
void build(T *tree, const T *arr) {
    for (int i = 1; i < tree_size; i++) {
        tree[i] += arr[i - 1];
        if (int nxt = i + (i & -i); nxt < tree_size) {
            tree[nxt] += tree[i];
        }
    }
}

// 传统单点更新
void update(T *tree, int i, T val) {
    for (; i < tree_size; i += i & -i) {
        tree[i] += val;
    }
}

// 传统单点查询
T query(const T *tree, int i) {
    if (i >= tree_size) {
        throw overflow_error("FenwickTree Overflow");
    }
    T res = 0;
    for (; i > 0; i &= i - 1) {
        res += tree[i];
    }
    return res;
}

void build(int n) {
    tree_size = n + 1;
    fill(info1, info1 + tree_size, 0);
    fill(info2, info2 + tree_size, 0);
}

void build(const T *arr, int n) {
    diff1[0] = arr[0];
    diff2[0] = 0;
    for (int i = 1; i < n; ++i) {
        diff1[i] = arr[i] - arr[i - 1];
        diff2[i] = i * diff1[i];
    }
    tree_size = n + 1;
    build(info1, diff1);
    build(info2, diff2);
}

// 区间更新
void update(int l, int r, T val) {
    update(info1, l, val);
    update(info1, r + 1, -val);
    update(info2, l, (l - 1) * val);
    update(info2, r + 1, -r * val);
}

// 区间查询
T query(int l, int r) {
    return query(info1, r) * r - query(info1, l - 1) * (l - 1) - query(info2, r) + query(info2, l - 1);
}