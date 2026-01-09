#include <bits/stdc++.h>
using namespace std;

/*
带权并查集（边权并查集）
每次合并都要给出两点的距离，这样就能得到任意一点与超级源点的距离
*/
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

    // 查询两点间的距离，需要保证两点连通
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
    fill(dis + 1, dis + n + 1, 0);
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

// 查询两点间的距离，需要保证两点连通
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