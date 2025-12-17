#include <bits/stdc++.h>
using namespace std;

// 并查集，英文名 Union-Find / Disjoint Set Union

// 封装为类
// class UnionFind {
//     vector<int> fa, sz;
    
// public:
//     int cc;
//     UnionFind(int n) : fa(n), sz(n, 1), cc(n) {
//         ranges::iota(fa, 0);
//     }

//     int find(int x) {
//         if (fa[x] != x) {
//             fa[x] = find(fa[x]);
//         }
//         return fa[x];
//     }

//     bool merge(int x, int y) {
//         x = find(x);
//         y = find(y);
//         if (x == y) {
//             return false;
//         }
//         if (sz[x] < sz[y]) {
//             swap(x, y);
//         }
//         fa[y] = x;
//         sz[x] += sz[y];
//         cc--;
//         return true;
//     }

//     bool is_same(int x, int y) {
//         return find(x) == find(y);
//     }

//     int get_size(int x) {
//         return sz[find(x)];
//     }
// };

// 静态数组实现
// constexpr int MAX_N = 100'000 + 5;
// int fa[MAX_N], sz[MAX_N], cc;

// void build(int n) {
//     iota(fa + 1, fa + n + 1, 1);
//     fill(sz + 1, sz + n + 1, 1);
//     cc = n;
// }

// int find(int x) {
//     if (fa[x] != x) {
//         fa[x] = find(fa[x]);
//     }
//     return fa[x];
// }

// bool merge(int x, int y) {
//     x = find(x);
//     y = find(y);
//     if (x == y) {
//         return false;
//     }
//     if (sz[x] < sz[y]) {
//         swap(x, y);
//     }
//     fa[y] = x;
//     sz[x] += sz[y];
//     cc--;
//     return true;
// }

// bool is_same(int x, int y) {
//     return find(x) == find(y);
// }

// int get_size(int x) {
//     return sz[find(x)];
// }

// 带权并查集（边权并查集）
template<typename T>
struct UnionFind {
    vector<int> fa;
    vector<T> dis;

    UnionFind(int n) : fa(n), dis(n) {
        ranges::iota(fa, 0);
    }

    int find(int x) {
        if (fa[x] != x) {
            int root = find(fa[x]);
            dis[x] += dis[fa[x]];
            fa[x] = root;
        }
        return fa[x];
    }

    bool is_same(int x, int y) {
        return find(x) == find(y);
    }

    T get_relative_distance(int from, int to) {
        find(from);
        find(to);
        // to-from = (x-from) - (x-to) = dis[from] - dis[to]
        return dis[from] - dis[to];
    }

    bool merge(int from, int to, T value) {
        int x = find(from), y = find(to);
        if (x == y) {
            return dis[from] - dis[to] == value;
        }
        // 由于 y-from = (y-x) + (x-from) = (y-to) + (to-from)
        // 所以 y-x = (to-from) + (y-to) - (x-from) = value + dis[to] - dis[from]
        dis[x] = value + dis[to] - dis[from];
        fa[x] = y;
        return true;
    }
};

using T = int;
constexpr int MAX_N = 100'000 + 5;

int fa[MAX_N];
T dis[MAX_N];

void build(int n) {
    iota(fa + 1, fa + n + 1, 1);
}

int find(int x) {
    if (fa[x] != x) {
        int root = find(fa[x]);
        dis[x] += dis[fa[x]];
        fa[x] = root;
    }
    return fa[x];
}

bool is_same(int x, int y) {
    return find(x) == find(y);
}

T get_relative_distance(int from, int to) {
    find(from);
    find(to);
    // to-from = (x-from) - (x-to) = dis[from] - dis[to]
    return dis[from] - dis[to];
}

bool merge(int from, int to, T value) {
    int x = find(from), y = find(to);
    if (x == y) {
        return dis[from] - dis[to] == value;
    }
    // 由于 y-from = (y-x) + (x-from) = (y-to) + (to-from)
    // 所以 y-x = (to-from) + (y-to) - (x-from) = value + dis[to] - dis[from]
    dis[x] = value + dis[to] - dis[from];
    fa[x] = y;
    return true;
}