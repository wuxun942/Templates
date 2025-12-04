#include <bits/stdc++.h>
using namespace std;

// 并查集

// 封装为类
class UnionFind {
    vector<int> fa, sz;
    
public:
    int cc;
    UnionFind(int n): fa(n), sz(n, 1), cc(n) {
        ranges::iota(fa, 0);
        // iota(fa.begin(), fa.end(), 0);
    }

    int find(int x) {
        if (fa[x] != x) {
            fa[x] = find(fa[x]);
        }
        return fa[x];
    }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        if (sz[x] < sz[y]) {
            swap(x, y);
        }
        fa[y] = x;
        sz[x] += sz[y];
        cc--;
        return true;
    }

    bool is_same(int x, int y) {
        return find(x) == find(y);
    }

    int get_size(int x) {
        return sz[find(x)];
    }
};

// 静态数组实现
constexpr int MAX_N = 100'000 + 5;
int fa[MAX_N], sz[MAX_N], cc;
void build(int n) {
    iota(fa, fa + n, 0);
    fill(sz, sz + n, 1);
    cc = n;
}

int find(int x) {
    if (fa[x] != x) {
        fa[x] = find(fa[x]);
    }
    return fa[x];
}

bool merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
        return false;
    }
    if (sz[x] < sz[y]) {
        swap(x, y);
    }
    fa[y] = x;
    sz[x] += sz[y];
    cc--;
    return true;
}

bool is_same(int x, int y) {
    return find(x) == find(y);
}

int get_size(int x) {
    return sz[find(x)];
}