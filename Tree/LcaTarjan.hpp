#include <bits/stdc++.h>
using namespace std;

// Tarjan 算法求 LCA，这里为 0 开头的节点编号
class LcaTarjan {
    int n, root;
    vector<vector<int>> g;
    // 并查集
    vector<int> pa;
    int find(int x) {
        if (x != pa[x]) {
            pa[x] = find(pa[x]);
        }
        return pa[x];
    }

    // tarjan 辅助数组
    vector<vector<pair<int, int>>> groups; // 分组查询，内存紧张时可以改成链式前向星
    vector<uint8_t> vis;
    vector<int> ans;
    void dfs(int x, int fa) {
        vis[x] = true;
        for (int y : g[x]) {
            if (y != fa) {
                dfs(y, x);
                // 合并
                pa[y] = x;
            }
        }
        for (auto &[y, idx] : groups[x]) {
            // 访问过的点才能查询
            if (vis[y]) {
                ans[idx] = find(y);
            }
        }
    }
public:
    LcaTarjan(vector<vector<int>> &edges, int s) {
        n = edges.size() + 1;
        root = s;
        g.resize(n);
        pa.resize(n);
        ranges::iota(pa, 0);
        for (auto &e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }
    }

    vector<int> get_lca(vector<vector<int>> &queries) {
        groups.resize(n);
        for (int i = 0, qn = queries.size(); i < qn; ++i) {
            groups[queries[i][0]].emplace_back(queries[i][1], i);
            groups[queries[i][1]].emplace_back(queries[i][0], i);
        }
        vis.resize(n);
        ans.resize(queries.size());
        dfs(root, -1);
        return ans;
    }
};