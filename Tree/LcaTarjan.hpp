#include <bits/stdc++.h>
using namespace std;

// Tarjan 算法求 LCA，这里为 0 开头的节点编号
class LcaTarjan {
    vector<int> head, nxt, to, pa;
    int n, root;
    int find(int x) {
        if (x != pa[x]) {
            pa[x] = find(pa[x]);
        }
        return pa[x];
    }
public:
    LcaTarjan(vector<vector<int>>& edges, int s) {
        n = edges.size() + 1;
        root = s;
        head.resize(n, -1);
        nxt.resize(2 * n - 2);
        to.resize(2 * n - 2);
        pa.resize(n);
        ranges::iota(pa, 0);
        for (int i = 0; i < n - 1; ++i) {
            int x = edges[i][0], y = edges[i][1], j = 2 * i;
            nxt[j] = head[x];
            to[j] = y;
            head[x] = j;
            nxt[++j] = head[y];
            to[j] = x;
            head[y] = j;
        }
    }

    vector<int> get_lca(vector<vector<int>>& queries) {
        vector<vector<pair<int, int>>> groups(n);
        for (int i = 0, qn = queries.size(); i < qn; ++i) {
            groups[queries[i][0]].emplace_back(queries[i][1], i);
            groups[queries[i][1]].emplace_back(queries[i][0], i);
        }
        vector vis(n, false);
        vector ans(queries.size(), 0);
        auto dfs = [&](auto&& dfs, int x, int fa) -> void {
            vis[x] = true;
            for (int e = head[x]; e >= 0; e = nxt[e]) {
                int y = to[e];
                if (y != fa) {
                    dfs(dfs, y, x);
                    pa[y] = x;
                }
            }
            for (auto [y, idx]: groups[x]) {
                if (vis[y]) {
                    ans[idx] = find(y);
                }
            }
        };
        dfs(dfs, root, -1);
        return ans;
    }
};