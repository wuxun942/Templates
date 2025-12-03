#include <bits/stdc++.h>
using namespace std;

/*
树状数组，需要注意查询和修改都是 1-based
对于 arr，所有模板类封装均为 0-based，所有静态数组实现均为 1-based
*/

// 单点修改 + 区间查询（原始） / 区间修改 + 单点查询（直接当差分用）

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
        T ans = 0;
        for (; i > 0; i &= i - 1) {
            ans += tree[i];
        }
        return ans;
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
int n;
T arr[MAXN];
int tree_size;
T tree[MAXN];

void build(int n, const T *arr) {
    tree_size = n;
    for (int i = 1; i <= tree_size; i++) {
        tree[i] += arr[i];
        if (int nxt = i + (i & -i); nxt <= tree_size) {
            tree[nxt] += tree[i];
        }
    }
}

void build(int n) {
    tree_size = n;
    fill(tree, tree + tree_size + 1, 0);
}

void update(int i, T val) {
    for (; i <= tree_size; i += i & -i) {
        tree[i] += val;
    }
}

T query(int i) {
    if (i > tree_size) {
        throw overflow_error("FenwickTree Overflow");
    }
    T ans = 0;
    for (; i > 0; i &= i - 1) {
        ans += tree[i];
    }
    return ans;
}

T query(int l, int r) {
    if (l > r) {
        return 0;
    }
    return query(r) - query(l - 1);
}

/*
权值树状数组第 k 小
按值域建树，用倍增代替二分，可以实现 O(log U) 的查询
在值域较大时，需要结合离散化，复杂度为 O(log n)
*/
template<typename T>
class FenwickTree {
    vector<T> tree;
    int n;

public:
    FenwickTree(int n) : tree(n + 1), n(n) {}

    void update(int i, T val) {
        for (; i <= n; i += i & -i) {
            tree[i] += val;
        }
    }

    int query(T k) {
        T s = 0;
        int ans = 0;
        for (int i = bit_width<uint32_t>(n) - 1; i >= 0; --i) {
            if ((ans | 1 << i) <= n && s + tree[ans | 1 << i] < k) {
                ans |= 1 << i;
                s += tree[ans];
            }
        }
        return ans + 1;
    }
};

// 区间修改 + 区间查询
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

    // 传统单点修改
    void update(vector<T> &tree, int i, T val) {
        for (; i < tree.size(); i += i & -i) {
            tree[i] += val;
        }
    }

    // 传统单点查询
    T query(const vector<T> &tree, int i) {
        T ans = 0;
        for (; i > 0; i &= i - 1) {
            ans += tree[i];
        }
        return ans;
    }

public:
    FenwickTree(int n) : info1(n + 1), info2(n + 1) {}

    FenwickTree(const vector<T> &arr) : FenwickTree(arr.size())  {
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

    // 区间
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
    for (int i = 1; i <= tree_size; i++) {
        tree[i] += arr[i];
        if (int nxt = i + (i & -i); nxt <= tree_size) {
            tree[nxt] += tree[i];
        }
    }
}

// 传统单点修改
void update(T *tree, int i, T val) {
    for (; i <= tree_size; i += i & -i) {
        tree[i] += val;
    }
}

// 传统单点查询
T query(const T *tree, int i) {
    if (i > tree_size) {
        throw overflow_error("FenwickTree Overflow");
    }
    T ans = 0;
    for (; i > 0; i &= i - 1) {
        ans += tree[i];
    }
    return ans;
}

void build(int n) {
    tree_size = n;
    fill(info1, info1 + tree_size + 1, 0);
    fill(info2, info2 + tree_size + 1, 0);
}

void build(const T *arr, int n) {
    diff1[1] = arr[1];
    diff2[1] = 0;
    for (int i = 2; i <= n; ++i) {
        diff1[i] = arr[i] - arr[i - 1];
        diff2[i] = i * diff1[i];
    }
    tree_size = n;
    build(info1, diff1);
    build(info2, diff2);
}

// 区间修改
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

// 二维树状数组，代码和一维没什么差别，实现上比树套树简单很多

// 单点修改 + 范围查询
template<typename T>
class FenwickTree {
    int m, n;
    vector<vector<T>> tree;

public:
    FenwickTree(int m, int n) {
        this->m = m + 1;
        this->n = n + 1;
        tree.resize(m + 1, vector<T>(n + 1));
    }

    FenwickTree(const vector<vector<T>> &mat) {
        int m = mat.size(), n = mat[0].size();
        this->m = m + 1;
        this->n = n + 1;
        tree.resize(m + 1, vector<T>(n + 1));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                update(i + 1, j + 1, mat[i][j]);
            }
        }
    }

    void update(int x, int y, T val) {
        for (int i = x; i < m; i += i & -i) {
            for (int j = y; j < n; j += j & -j) {
                tree[i][j] += val;
            }
        }
    }

    T query(int x, int y) {
        T ans = 0;
        for (int i = x; i > 0; i &= i - 1) {
            for (int j = y; j > 0; j &= j - 1) {
                ans += tree[i][j];
            }
        }
        return ans;
    }

    T query(int x1, int y1, int x2, int y2) {
        if (x1 < x2 || y1 < y2) {
            return 0;
        }
        return query(x2, y2) - query(x2, y1 - 1) - query(x1 - 1, y2) + query(x1 - 1, y1 - 1);
    }
};

// 范围修改 + 范围查询
template<typename T>
class FenwickTree {
    vector<vector<T>> info1, info2, info3, info4;
    int m, n;

    void update(int x, int y, T v) {
        T v1 = v;
        T v2 = x * v;
        T v3 = y * v;
        T v4 = x * y * v;
        for (int i = x; i <= m; i += i & -i) {
            for (int j = y; j <= n; j += j & -j) {
                info1[i][j] += v1;
                info2[i][j] += v2;
                info3[i][j] += v3;
                info4[i][j] += v4;
            }
        }
    }

    T query(int x, int y) {
        T ans = 0;
        for (int i = x; i > 0; i &= i - 1) {
            for (int j = y; j > 0; j &= j - 1) {
                ans += (x + 1) * (y + 1) * info1[i][j] - (y + 1) * info2[i][j] - (x + 1) * info3[i][j] + info4[i][j];
            }
        }
        return ans;
    }

public:
    FenwickTree(int m, int n) {
        this->m = m + 1;
        this->n = n + 1;
        info1.resize(m + 1, vector(n + 1, 0));
        info2.resize(m + 1, vector(n + 1, 0));
        info3.resize(m + 1, vector(n + 1, 0));
        info4.resize(m + 1, vector(n + 1, 0));
    }

    FenwickTree(const vector<vector<T>> &mat) {
        m = mat.size();
        n = mat[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                update(i, j, mat[i][j]);
            }
        }
    }

    void update(int x1, int y1, int x2, int y2, T v) {
        update(x1, y1, v);
        update(x2 + 1, y2 + 1, v);
        update(x1, y2 + 1, -v);
        update(x2 + 1, y1, -v);
    }

    T query(int x1, int y1, int x2, int y2) {
        return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
    }
};