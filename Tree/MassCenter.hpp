#include <bits/stdc++.h>
using namespace std;

/*
树的重心有三种定义：
1. 最大子树的节点数最少
2. 每棵子树的节点数不超过总节点数的一半
3. 所有节点走向该节点的边数最少
*/

// 以下均以 1 为最小编号

constexpr int MX = 100'001;
int head[MX];
int nxt[MX];
int to[MX];
int e;

void add_edge(int x, int y) {
    nxt[e] = head[x];
    to[e] = y;
    head[x] = e++;
}

// 第一种定义解重心
vector<int> calc_masscenter1(const vector<vector<int>>& edges) {
    e = 0;
    int n = edges.size() + 1;
    for (int i = 1; i <= n; ++i) {
        head[i] = -1;
    }
    for (auto& e: edges) {
        int x = e[0], y = e[1];
        add_edge(x, y);
        add_edge(y, x);
    }
    int cnt = INT_MAX;
    int ans1 = 0, ans2 = 0;
    auto dfs = [&](auto&& dfs, int x, int fa) -> int {
        int res = 1, maxsub = 0;
        for (int e = head[x]; e >= 0; e = nxt[e]) {
            if (int y = to[e]; y != fa) {
                int cntsub = dfs(dfs, y, x);
                res += cntsub;
                maxsub = max(maxsub, cntsub);
            }
        }
        maxsub = max(maxsub, n - res);
        if (maxsub < cnt) {
            ans1 = x;
            ans2 = 0;
            cnt = maxsub;
        } else if (maxsub == cnt) {
            ans2 = x;
        }
        return res;
    };
    dfs(dfs, 1, 0);
    if (ans2 == 0) {
        return {ans2};
    }
    return {ans1, ans2};
}

// 第二种定义解重心
vector<int> calc_masscenter2(const vector<vector<int>>& edges) {
    e = 0;
    int n = edges.size() + 1;
    for (int i = 1; i <= n; ++i) {
        head[i] = -1;
    }
    for (auto& e: edges) {
        int x = e[0], y = e[1];
        add_edge(x, y);
        add_edge(y, x);
    }
    int cnt = INT_MAX;
    int ans1 = 0, ans2 = 0;
    auto dfs = [&](auto&& dfs, int x, int fa) -> int {
        int res = 1;
        bool ok = true;
        for (int e = head[x]; e >= 0; e = nxt[e]) {
            if (int y = to[e]; y != fa) {
                int cntsub = dfs(dfs, y, x);
                res += cntsub;
                ok = ok && cntsub <= n / 2;
            }
        }
        ok = ok && (n - res <= n / 2);
        if (ok) {
            (ans1 == 0 ? ans1 : ans2) = x;
        }
        return res;
    };
    dfs(dfs, 1, 0);
    if (ans2 == 0) {
        return {ans2};
    }
    return {ans1, ans2};
}