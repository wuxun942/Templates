#include <bits/stdc++.h>
using namespace std;

/*
带权并查集（边权并查集）
从一维坐标的角度理解，dis[i] 表示 i 和其祖先的坐标差，
即 dis[i] = index(i) - index(find(i))

由此可以得到两个重要结论：
1. 如果 x 和 y 属于同一集合，那么 index(x) - index(y) = dis[x] - dis[y]
2. 给出两个不相交的点 from, to 的坐标差 val，
设 find(from) = x, find(to) = y，那么合并后
dis[x] = val + dis[to] - dis[from]
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

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    // 查询两点间的坐标差，需要保证两点连通
    T get_relative_distance(int from, int to) {
        find(from);
        find(to);
        return dis[from] - dis[to];
    }

    // 给出 from 和 to 的坐标差，合并两个集合
    bool merge(int from, int to, T value) {
        int x = find(from), y = find(to);
        if (x == y) {
            return dis[from] - dis[to] == value;
        }
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

bool same(int x, int y) {
    return find(x) == find(y);
}

// 查询两点间的坐标差，需要保证两点连通
T get_relative_distance(int from, int to) {
    find(from);
    find(to);
    return dis[from] - dis[to];
}

// 给出 from 和 to 的坐标差，合并两个集合
bool merge(int from, int to, T value) {
    int x = find(from), y = find(to);
    if (x == y) {
        return dis[from] - dis[to] == value;
    }
    dis[x] = value + dis[to] - dis[from];
    fa[x] = y;
    return true;
}