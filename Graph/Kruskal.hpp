#include <bits/stdc++.h>
using namespace std;

// Kruskal 算法，需要用到并查集；返回生成树权值和
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
        if (sz[x] > sz[y]) {
            fa[y] = x;
            sz[x] += sz[y];
        } else {
            fa[x] = y;
            sz[y] += sz[x];
        }
        cc--;
        return true;
    }
};

long long kruskal(int n, vector<vector<int>> &edges) {
    long long res = 0;
    UnionFind uf(n);
    ranges::sort(edges, {}, [](const auto &x) {return x[2];});
    for (int i = 0; i < edges.size() && uf.cc > 1; ++i) {
        auto &e = edges[i];
        if (uf.merge(e[0], e[1])) {
            res += e[2];
        }
    }
    return uf.cc == n ? res : -1;
}