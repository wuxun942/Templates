#include <bits/stdc++.h>
using namespace std;

// Tarjan 算法求 LCA，这里为 0 开头的节点编号
class LcaTarjan {
    int n, root;
    vector<vector<int>> g;
    // 并查集
    vector<int> fa;
    int find(int x) {
        if (x != fa[x]) {
            fa[x] = find(fa[x]);
        }
        return fa[x];
    }
public:
    LcaTarjan(vector<vector<int>>& edges, int s) {
        n = edges.size() + 1;
        root = s;
        g.resize(n);
        fa.resize(n);
        ranges::iota(fa, 0);
        for (auto& e: edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }
    }

    vector<int> get_lca(vector<vector<int>>& queries) {
        // 分组查询，内存紧张时可以改成链式前向星
        vector<vector<pair<int, int>>> groups(n);
        for (int i = 0, qn = queries.size(); i < qn; ++i) {
            groups[queries[i][0]].emplace_back(queries[i][1], i);
            groups[queries[i][1]].emplace_back(queries[i][0], i);
        }
        vector vis(n, false);
        vector ans(queries.size(), 0);
        auto dfs = [&](auto&& dfs, int x, int f) -> void {
            vis[x] = true;
            for (int y: g[x]) {
                if (y != f) {
                    dfs(dfs, y, x);
                    // 合并
                    fa[y] = x;
                }
            }
            for (auto [y, idx]: groups[x]) {
                // 访问过的点才能查询
                if (vis[y]) {
                    ans[idx] = find(y);
                }
            }
        };
        dfs(dfs, root, -1);
        return ans;
    }
};