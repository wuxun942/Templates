#include <bits/stdc++.h>
using namespace std;

/*
求 LCA，常用于求两点路径、树上差分等问题。
常用方法：
1. Tarjan：理论最快，但是强制离线且不好统计路径信息
2. 倍增：原理简单，可求路径信息，但常数大
3. 树链剖分：原理较难，复杂度和倍增相同且常数小，可作为上位替代

以下类封装方法均为 0-based，全局区实现为 1-based
*/

// Tarjan 算法求 LCA
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

    // tarjan 辅助数组
    vector<vector<pair<int, int>>> groups; // 分组查询，内存紧张时可以改成链式前向星
    vector<uint8_t> vis;
    vector<int> ans;
    void dfs(int x, int f) {
        vis[x] = true;
        for (int y : g[x]) {
            if (y != f) {
                dfs(y, x);
                // 合并
                fa[y] = x;
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
    LcaTarjan(vector<vector<int>> &edges, int root = 0) {
        n = edges.size() + 1;
        this->root = root;
        g.resize(n);
        fa.resize(n);
        ranges::iota(fa, 0);
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

// 倍增法求 LCA
template<typename T>
class LcaBinaryLifting {
    vector<vector<pair<int, int>>> g;
    vector<vector<int>> stjump;
    vector<int> depth;
    vector<T> dis; // 无权图用 depth 代替
    void dfs(int x, int fa) {
        stjump[0][x] = fa;
        for (int i = 1, m = bit_width<uint32_t>(depth[x]); i < m; i++) {
            if (int p = stjump[i - 1][x]; p != -1) {
                stjump[i][x] = stjump[i - 1][p];
            }
        }
        for (auto &[y, w] : g[x]) {
            if (y != fa) {
                depth[y] = depth[x] + 1;
                dis[y] = dis[x] + w;
                dfs(y, x);
            }
        }
    }

public:
    LcaBinaryLifting(const vector<vector<int>> &edges, int root = 0) {
        int n = edges.size() + 1;
        g.resize(n);
        for (auto &e : edges) {
            g[e[0]].emplace_back(e[1], e[2]);
            g[e[1]].emplace_back(e[0], e[2]);
        }
        stjump.resize(bit_width<uint32_t>(n), vector(n, -1));
        depth.resize(n);
        dis.resize(n);
        dfs(root, -1);
    }

    int get_kth_ancestor(int x, int k) {
        for (; k; k &= k - 1) {
            x = stjump[__builtin_ctz(k)][x];
        }
        return x;
    }

    int get_lca(int x, int y) {
        if (depth[x] > depth[y]) {
            swap(x, y);
        }
        y = get_kth_ancestor(y, depth[y] - depth[x]);
        if (x == y) {
            return x;
        }
        for (int i = bit_width<uint32_t>(depth[x]) - 1; i >= 0; i--) {
            if (int px = stjump[i][x], py = stjump[i][y]; px != py) {
                x = px;
                y = py;
            }
        }
        return stjump[0][x];
    }

    // 获取 x 与 y 的距离
    T get_dis(int x, int y) {
        return dis[x] + dis[y] - 2 * dis[get_lca(x, y)];
    }
};

constexpr int MAX_N = 100'000 + 5, MAX_M = 17, MAX_E = MAX_N << 1;
int head[MAX_N];
int nxt[MAX_E];
int to[MAX_E];
// long long weight[MAX_E];
int cnt_edge = 0;
void add_edge(int x, int y, long long w = 0) {
    nxt[++cnt_edge] = head[x];
    to[cnt_edge] = y;
    // weight[cnt_edge] = w;
    head[x] = cnt_edge;
}

int stjump[MAX_M][MAX_N];
int depth[MAX_N];
void dfs(int x, int fa) {
    stjump[0][x] = fa;
    for (int i = 1, m = bit_width<uint32_t>(depth[x]); i < m; i++) {
        if (int p = stjump[i - 1][x]; p != 0) {
            stjump[i][x] = stjump[i - 1][p];
        }
    }
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != fa) {
            depth[y] = depth[x] + 1;
            dfs(y, x);
        }
    }
}

void build(int n, int root = 1) {
    int m = bit_width<uint32_t>(n);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            stjump[i][j] = 0;
        }
    }
    depth[root] = 0;
    dfs(root, 0);
}

int get_kth_ancestor(int x, int k) {
    for (; k; k &= k - 1) {
        x = stjump[__builtin_ctz(k)][x];
    }
    return x;
}

int get_lca(int x, int y) {
    if (depth[x] > depth[y]) {
        swap(x, y);
    }
    y = get_kth_ancestor(y, depth[y] - depth[x]);
    if (x == y) {
        return x;
    }
    for (int i = bit_width<uint32_t>(depth[x]) - 1; i >= 0; i--) {
        if (int px = stjump[i][x], py = stjump[i][y]; px != py) {
            x = px;
            y = py;
        }
    }
    return stjump[0][x];
}

// 树剖求 LCA
class LCA_HDL {
    vector<vector<int>> g;
    vector<int> fa;
    vector<int> depth;
    vector<int> sz;
    vector<int> son;
    vector<int> top;
    void dfs1(int x, int f) {
        fa[x] = f;
        sz[x] = 1;
        int max_size = 0;
        for (int y : g[x]) {
            if (y != f) {
                depth[y] = depth[x] + 1;
                dfs1(y, x);
                sz[x] += sz[y];
                if (max_size < sz[y]) {
                    son[x] = y;
                    max_size = sz[y];
                }
            }
        }
    }

    int clk = 0;
    void dfs2(int x, int t) {
        top[x] = t;
        if (son[x] == -1) {
            return;
        }
        dfs2(son[x], t);
        for (int y : g[x]) {
            if (y != fa[x] && y != son[x]) {
                dfs2(y, y);
            }
        }
    }

public:
    LCA_HDL(vector<vector<int>> &edges, int root = 0) {
        int n = edges.size() + 1;
        g.resize(n);
        for (auto &e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }
        fa.resize(n);
        depth.resize(n);
        sz.resize(n);
        son.resize(n, -1);
        top.resize(n);
        dfs1(root, -1);
        dfs2(root, root);
    }

    int get_lca(int x, int y) {
        while (top[x] != top[y]) {
            if (depth[top[x]] < depth[top[y]]) {
                swap(x, y);
            }
            x = fa[top[x]];
        }
        return depth[x] <= depth[y] ? x : y;
    }
};

constexpr int MAX_N = 100'000 + 5, MAX_E = MAX_N << 1;
int head[MAX_N];
int nxt[MAX_E];
int to[MAX_E];
// long long weight[MAX_E];
int cnt_edge = 0;
void add_edge(int x, int y, long long w = 0) {
    nxt[++cnt_edge] = head[x];
    to[cnt_edge] = y;
    // weight[cnt_edge] = w;
    head[x] = cnt_edge;
}

int fa[MAX_N];
int depth[MAX_N];
int sz[MAX_N];
int son[MAX_N];
int top[MAX_N];

void dfs1(int x, int f) {
    fa[x] = f;
    sz[x] = 1;
    son[x] = 0;
    int max_size = 0;
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != f) {
            depth[y] = depth[x] + 1;
            dfs1(y, x);
            sz[x] += sz[y];
            if (max_size < sz[y]) {
                son[x] = y;
                max_size = sz[y];
            }
        }
    }
}

int clk = 0;
void dfs2(int x, int t) {
    top[x] = t;
    if (son[x] == 0) {
        return;
    }
    dfs2(son[x], t);
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != fa[x] && y != son[x]) {
            dfs2(y, y);
        }
    }
}

int get_lca(int x, int y) {
    while (top[x] != top[y]) {
        if (depth[top[x]] < depth[top[y]]) {
            swap(x, y);
        }
        x = fa[top[x]];
    }
    return depth[x] <= depth[y] ? x : y;
}

/*
在倍增求 LCA 的过程中，我们实现了 O(log n) 求 k 级祖先的方法
然而这个问题还能用长链剖分来优化到 O(1)
*/

constexpr int MAX_N = 500'000 + 5, MAX_E = MAX_N << 1;
int head[MAX_N];
int nxt[MAX_E];
int to[MAX_E];
int cnt_edge = 0;
void add_edge(int x, int y, long long w = 0) {
    nxt[++cnt_edge] = head[x];
    to[cnt_edge] = y;
    head[x] = cnt_edge;
}

int fa[MAX_N];
int depth[MAX_N]; // 或者初始化 depth[0] = 0
int height[MAX_N];

// 每个子树的长儿子（子树最高）
int son[MAX_N];

// 所在长链的头节点
int top[MAX_N];

// 按长链分配 dfn
int dfn[MAX_N];

// dfn 到原序号的逆映射
int seg[MAX_N];

constexpr int MAX_N = 100'000 + 5, MAX_H = 20;
int stjump[MAX_N][MAX_H];
int up[MAX_N];
int down[MAX_N];

// 第一次遍历，建立 fa, depth, sz, son 信息
void dfs1(int x, int f) {
    fa[x] = f;
    depth[x] = depth[f] + 1;
    son[x] = 0;
    int max_height = 0;
    stjump[x][0] = f;
    for (int j = 1; j < bit_width<uint32_t>(depth[x]); ++j) {
        stjump[x][j] = stjump[stjump[x][j - 1]][j - 1];
    }
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != f) {
            dfs1(y, x);
            if (max_height < height[y]) {
                son[x] = y;
                max_height = height[y];
            }
        }
    }
    height[x] = max_height + 1;
}

// 第二次遍历，建立 top, dfn, seg 信息
int clk = 0;
void dfs2(int x, int t) {
    top[x] = t;
    dfn[x] = ++clk; // 此为 1-based
    seg[clk] = x;
    if (son[x] == 0) {
        return;
    }
    dfs2(son[x], t);
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != fa[x] && y != son[x]) {
            dfs2(y, y);
        }
    }
}

void prepare(int n, int root) {
    dfs1(root, 0);
    dfs2(root, root);
    for (int x = 1; x <= n; ++x) {
        if (top[x] == x) {
            for (int i = 0, a = x, b = x; i < height[x]; ++i, a = stjump[a][0], b = son[b]) {
                up[dfn[x] + i] = a;
                down[dfn[x] + i] = b;
            }
        }
    }
}

int query(int x, int k) {
    if (k == 0) {
        return x;
    }
    int hb = bit_width<uint32_t>(k) - 1;
    x = stjump[x][hb];
    k -= 1 << hb;
    k -= depth[x] - depth[top[x]];
    x = top[x];
    if (k >= 0) {
        return up[dfn[x] + k];
    }
    return down[dfn[x] - k];
}