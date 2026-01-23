#include <bits/stdc++.h>
using namespace std;

/*
可持久化线段树（区间修改）
常用的实现方法有：
1. 节点复用 + 懒更新机制
2. 节点复用 + 标记永久化

通常来说，前者更为通用，但空间占用较大，为 O(n + q * log n)，
需要开 n * 4 + q * 4 * log n，其中 q 为版本数
当然，具体题目中懒标记不一定会传到底，所以 AC 代码的使用空间只可能更小（和特殊情况的动态开点线段树同理）

后者只用于 “修改操作对查询信息具有可加性” 的情况，可以有效减少空间占用（说人话就是应对卡空间的题目）
*/

// 懒标记下传实现，以 区间增加 + 维护区间和 为例

// 维护数组的最大空间
constexpr int MAX_N = 100'000 + 5;

// 最大版本数量
constexpr int MAX_Q = 100'000 + 5;

// 可持久化 Lazy 线段树的使用空间 = n * 4 + q * 4 * log n
constexpr int MAX_V = MAX_N * 4 + MAX_Q * 4 * 17;

using T = long long;
constexpr T INIT = 0;

using F = long long;
constexpr F TO_ADD_INIT = 0;

int arr_size;
T arr[MAX_N];

// 空间使用计数
int cnt = 0;

// 每个版本的根节点
int roots[MAX_Q];

T tree[MAX_V];

F to_add[MAX_V];

int ls[MAX_V];
int rs[MAX_V];

// 清空
void clear(int sz = cnt) {
    cnt = 0;
    fill(tree + 1, tree + sz + 1, 0);
    fill(ls + 1, ls + sz + 1, 0);
    fill(rs + 1, rs + sz + 1, 0);
}

int add_node(T x = INIT) {
    tree[++cnt] = x;
    to_add[cnt] = TO_ADD_INIT;
    return cnt;
}

// 拷贝节点
int copy_node(int i) {
    tree[++cnt] = tree[i];
    to_add[cnt] = to_add[i];
    ls[cnt] = ls[i];
    rs[cnt] = rs[i];
    return cnt;
}

T merge_val(T a, T b) {
    return a + b;
}

F merge_todo(F a, F b) {
    return a + b;
}

void apply(int i, int l, int r, F val) {
    tree[i] = merge_val(tree[i], (r - l + 1) * val);
    to_add[i] = merge_todo(to_add[i], val);
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
    arr_size = arr_size;
    roots[0] = build(arr, 1, arr_size);
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

void update(int v, int qv, int ql, int qr, T val) {
    roots[v] = update(roots[qv], 1, arr_size, ql, qr, val);
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

T query(int v, int qv, int ql, int qr) {
    roots[v] = roots[qv];
    return query(roots[qv], 1, arr_size, ql, qr);
}

// 标记永久化实现
constexpr int MAX_N = 100'000 + 5;

using T = long long;
constexpr T INIT = 0;
constexpr T TO_ADD_INIT = 0;

int arr_size;
T arr[MAX_N];

T tree[MAX_N << 2];
T add_tag[MAX_N << 2];

// 清空
void clear(int sz = cnt) {
    cnt = 0;
    fill(tree + 1, tree + sz + 1, 0);
    fill(ls + 1, ls + sz + 1, 0);
    fill(rs + 1, rs + sz + 1, 0);
}

int add_node(T x = INIT) {
    tree[++cnt] = x;
    to_add[cnt] = TO_ADD_INIT;
    return cnt;
}

// 拷贝节点
int copy_node(int i) {
    tree[++cnt] = tree[i];
    to_add[cnt] = to_add[i];
    ls[cnt] = ls[i];
    rs[cnt] = rs[i];
    return cnt;
}

T merge_val(T a, T b) {
    return a + b;
}

T merge_tag(T a, T b) {
    return a + b;
}

int build(const T *arr, int l, int r) {
    if (l == r) {
        return add_node(arr[l]);
    }
    int i = add_node();
    int m = (l + r) / 2;
    ls[i] = build(arr, l, m);
    rs[i] = build(arr, m + 1, r);
    tree[i] = merge_val(tree[i * 2], tree[i * 2 + 1]);
    return i;
}

void build(const T *arr, int sz) {
    arr_size = sz;
    roots[0] = build(arr, 1, arr_size);
}

void build(int sz, T init_val) {
    fill(arr + 1, arr + sz + 1, init_val);
    build(arr, sz);
}

int update(int i, int l, int r, int ql, int qr, T val) {
    i = copy_node(i);
    tree[i] = merge_val(tree[i], (min(r, qr) - max(l, ql) + 1) * val);
    if (ql <= l && r <= qr) {
        add_tag[i] = merge_tag(add_tag[i], val);
        return i;
    }
    int m = (l + r) / 2;
    if (ql <= m) {
        update(ls[i], l, m, ql, qr, val);
    }
    if (qr > m) {
        update(rs[i], m + 1, r, ql, qr, val);
    }
    return i;
}

void update(int v, int qv, int ql, int qr, T val) {
    roots[v] = update(roots[qv], 1, arr_size, ql, qr, val);
}

T query(int i, int l, int r, int ql, int qr, T to_add) {
    if (ql <= l && r <= qr) {
        return merge_val(tree[i], to_add * (r - l + 1));
    }
    int m = (l + r) / 2;
    T res = INIT;
    if (ql <= m) {
        res = merge_val(res, query(ls[i], l, m, ql, qr, to_add + add_tag[i]));
    }
    if (qr > m) {
        res = merge_val(res, query(rs[i], m + 1, r, ql, qr, to_add + add_tag[i]));
    }
    return res;
}

T query(int v, int qv, int ql, int qr) {
    roots[v] = roots[qv];
    return query(roots[qv], 1, arr_size, ql, qr, TO_ADD_INIT);
}