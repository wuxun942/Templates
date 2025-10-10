#include <bits/stdc++.h>
using namespace std;

// 倍增法求 LCA
template<typename T>
class LcaBinaryLifting {
    vector<vector<int>> pa;
    vector<int> depth;
    vector<T> dis; // 无权图用 depth 代替
public:
    LcaBinaryLifting(const vector<vector<int>> &edges) {
        int n = edges.size() + 1;
        vector<vector<pair<int, int>>> g(n);
        for (auto& e: edges) {
            g[e[0]].emplace_back(e[1], e[2]);
            g[e[1]].emplace_back(e[0], e[2]);
        }
        int m = bit_width<uint32_t>(n);
        pa.resize(m, vector(n, -1));
        depth.resize(n);
        dis.resize(n);
        auto dfs = [&](auto &&dfs, int x, int fa) -> void {
            pa[0][x] = fa;
            for (auto &[y, w]: g[x]) {
                if (y != fa) {
                    depth[y] = depth[x] + 1;
                    dis[y] = dis[x] + w;
                    dfs(dfs, y, x);
                }
            }
        };
        dfs(dfs, 0, -1);
        for (int i = 1; i < m; i++) {
            for (int x = 0; x < n; x++) {
                if (int p = pa[i - 1][x]; p != -1) {
                    pa[i][x] = pa[i - 1][p];
                }
            }
        }
    }

    int get_kth_ancestor(int x, int k) {
        for (; k; k &= k - 1) {
            x = pa[__builtin_ctz(k)][x];
        }
        return x;
    }

    int get_lca(int x, int y) {
        if (depth[x] > depth[y]) swap(x, y);
        y = get_kth_ancestor(y, depth[y] - depth[x]);
        if (x == y) {
            return x;
        }
        for (int i = bit_width((unsigned) depth[x]) - 1; i >= 0; i--) {
            if (int px = pa[i][x], py = pa[i][y]; px != py) {
                x = px;
                y = py;
            }
        }
        return pa[0][x];
    }

    // 获取 x 与 y 的距离
    T get_dis(int x, int y) {
        return dis[x] + dis[y] - 2 * dis[get_lca(x, y)];
    }

    // 从 x 出发向上跳至多 d 距离
    // int upto_dis(int x, T d) {
    //     for (int i = bit_width((unsigned) depth[x]) - 1; i >= 0; --i) {
    //         if (int p = pa[i][x]; p >= 0 && dis[x] - dis[p] <= d) {
    //             d -= dis[x] - dis[p];
    //             x = p;
    //         }
    //     }
    //     return x;
    // }
};