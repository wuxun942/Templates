#include <bits/stdc++.h>
using namespace std;

/*
树的直径有两种求法：
1. 两次 DFS，只适用于非负权树，但可以得到具体路径
2. 树形 DP，适用于所有树，只能得到长度

对于边权为正的树，有以下结论：
1. 如果有多条直径，那么这些直径一定有公共部分，可以只是一个点，也可以是一段路径
2. 取树上任意一点，对于和它相距最远的点的集合，至少包含了直径的一个端点
*/ 

// 两次 DFS 求直径
constexpr int MAX_N = 1'000'000 + 5;
int head[MAX_N];
int nxt[MAX_N];
int to[MAX_N];
int weight[MAX_N];
int last[MAX_N];
int e;

void add_edge(int x, int y, int w) {
    nxt[e] = head[x];
    to[e] = y;
    weight[e] = w;
    head[x] = e++;
}

long long get_diameter(const vector<vector<int>>& edges) {
    e = 0;
    int n = edges.size() + 1;
    fill(head, head + n, -1);
    for (auto& e: edges) {
        int x = e[0], y = e[1], w = e[2];
        add_edge(x, y, w);
        add_edge(y, x, w);
    }
    long long ans = 0;
    int end = 0;
    auto dfs = [&](this auto&& dfs, int x, int fa, long long dis) -> void {
        if (dis > ans) {
            end = x;
            ans = dis;
        }
        for (int e = head[x]; e >= 0; e = nxt[e]) {
            if (int y = to[e]; y != fa) {
                last[y] = x;
                dfs(y, x, dis + weight[e]);
            }
        }
    };
    dfs(0, -1, 0);
    int start = end;
    ans = 0;
    dfs(start, -1, 0);
    // 第二次 DFS 后 start - end 所在的路径就是直径
    return ans;
}

// 树形 DP 求直径
constexpr int MAX_N = 1'000'000 + 5;
int head[MAX_N];
int nxt[MAX_N];
int to[MAX_N];
int weight[MAX_N];
int e;
long long ans;

void add_edge(int x, int y, int w) {
    nxt[e] = head[x];
    to[e] = y;
    weight[e] = w;
    head[x] = e++;
}

long long get_diameter(const vector<vector<int>> &edges) {
    e = 0;
    ans = 0;
    int n = edges.size() + 1;
    fill(head, head + n, -1);
    for (auto &e: edges) {
        int x = e[0], y = e[1], w = e[2];
        add_edge(x, y, w);
        add_edge(y, x, w);
    }
    long long ans = 0;
    auto dfs = [&](this auto &&dfs, int x, int fa) -> long long {
        long long mx = 0;
        for (int e = head[x]; e >= 0; e = nxt[e]) {
            if (int y = to[e]; y != fa) {
                long long res = dfs(y, x) + weight[e];
                ans = max(ans, mx + res);
                mx = max(mx, res);
            }
        }
        return mx;
    };
    dfs(0, -1);
    return ans;
}