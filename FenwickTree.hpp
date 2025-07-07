#include <bits/stdc++.h>
using namespace std;
// 树状数组，需要注意查询和修改都是 1-index

// 单点更新区间查询（原始） / 区间更新单点查询（直接当差分用）
template<typename T>
class FenwickTree {
    vector<T> tree;
public:
    FenwickTree(int n): tree(n + 1) {}

    FenwickTree(const vector<T>& a): tree(a.size() + 1) {
        int n = a.size();
        for (int i = 1; i <= n; i++) {
            tree[i] += a[i - 1];
            if (int nxt = i + (i & -i); nxt <= n) {
                tree[nxt] += tree[i - 1];
            }
        }
    }

    void update(int i, T val) {
        for (; i < tree.size(); i += i & -i) {
            tree[i] += val;
        }
    }

    T query(int i) const {
        T res = 0;
        for (; i > 0; i &= i - 1) {
            res += tree[i];
        }
        return res;
    }

    T query(int l, int r) const {
        if (l > r) {
            return 0;
        }
        return query(r) - query(l - 1);
    }
};

// 区间更新区间查询
template<typename T>
class FenwickTree {
    vector<T> info1, info2;

    T query(const vector<T>& tree, int i) {
        T res = 0;
        for (; i > 0; i &= i - 1) {
            res += tree[i];
        }
        return res;
    }

    void build(vector<T>& tree, const vector<T>& a) {
        int n = a.size();
        for (int i = 1; i <= n; i++) {
            tree[i] += a[i - 1];
            if (int nxt = i + (i & -i); nxt <= n) {
                tree[nxt] += tree[i];
            }
        }
    }

    void update(vector<T>& tree, int i, T val) {
        for (; i < tree.size(); i += i & -i) {
            tree[i] += val;
        }
    }

public:
    FenwickTree(int n): info1(n + 1), info2(n + 1) {}

    FenwickTree(const vector<T>& a): FenwickTree(a.size())  {
        int n = a.size();
        vector<T> diff1(n), diff2(n);
        diff1[0] = a[0];
        diff2[0] = 0;
        for (int i = 1; i < n; ++i) {
            diff1[i] = a[i] - a[i - 1];
            diff2[i] = i * diff1[i];
        }
        build(info1, diff1);
        build(info2, diff2);
    }

    void update(int l, int r, T val) {
        update(info1, l, val);
        update(info1, r + 1, -val);
        update(info2, l, (l - 1) * val);
        update(info2, r + 1, -r * val);
    }

    T query(int l, int r) {
        return query(info1, r) * r - query(info1, l - 1) * (l - 1) - query(info2, r) + query(info2, l - 1);
    }
};