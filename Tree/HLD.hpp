#include <bits/stdc++.h>
using namespace std;

/*
树链剖分 Heavy-Light Decompostion
常用的内容有重链剖分和长链剖分
*/

/*
1. 重链剖分
从任意节点沿着长链向上跳，到达根节点至多需要 O(log n) 步

推论：
1.1 可将树上任意一条路径划分为 O(log n) 条重链
1.2 容易证明，一棵树的重链数量为 O(log n)
*/

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
int depth[MAX_N]{}; // 或者初始化 depth[0] = 0
int sz[MAX_N];

// 每个子树的重儿子（子树最大）
int son[MAX_N];

// 所在重链的头节点
int top[MAX_N];

// 按重链分配 dfn
int dfn[MAX_N];

// dfn 到原序号的逆映射
int seg[MAX_N];

// 第一次遍历，建立 fa, depth, sz, son 信息
void dfs1(int x, int f) {
    fa[x] = f;
    sz[x] = 1;
    son[x] = 0;
    int max_size = 0;
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != f) {
            depth[y] = depth[x] + 1;
            // arr[y] = weight[e]; // 处理边权问题时，转化为点权的技巧
            dfs1(y, x);
            sz[x] += sz[y];
            if (max_size < sz[y]) {
                son[x] = y;
                max_size = sz[y];
            }
        }
    }
}

// 第二次遍历，建立 top, dfn, seg 信息
int clk = 0;
void dfs2(int x, int t) {
    top[x] = t;
    dfn[x] = ++clk; // 此为 1-index
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

// 树链剖分通常结合线段树，完成路径更新和路径查询
// 一般需要注意的是，线段树建立时的底层赋值为 tree[o] = arr[seg[l]]

int arr[MAX_N]; // 如果是边权问题，需要加上 arr[1] = 0;
void path_update(int x, int y) {
    while (top[x] != top[y]) {
        if (depth[top[x]] < depth[top[y]]) {
            swap(x, y);
        }
        // update(dfn[top[x]], dfn[x]);
        x = fa[top[x]];
    }
    if (depth[x] > depth[y]) {
        swap(x, y);
    }
    // update(dfn[x], dfn[y]); // 此为点权写法，边权需要改为 (dfn[x] + 1, dfn[y])
}

/*
2. 长链剖分：把重儿子改成长儿子（常用于树形 DP 的优化）
从任意节点沿着长链向上跳，到达根节点至多需要 O(sqrt(n)) 步

只需要把 sz 数组改成 height 数组，表示子树的高度
并且把选择 son 的逻辑改成最大 height 即可

同时这也启示我们，剖分的权值可以根据题目要求进行调整
*/

constexpr int MAX_N = 100'000 + 5, MAX_E = MAX_N << 1;
int head[MAX_N]{};
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
int depth[MAX_N]{}; // 或者初始化 depth[0] = 0
int height[MAX_N];

int son[MAX_N];

int top[MAX_N];

int dfn[MAX_N];

int seg[MAX_N];

void dfs1(int x, int f) {
    fa[x] = f;
    son[x] = 0;
    int max_height = 0;
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != f) {
            depth[y] = depth[x] + 1;
            dfs1(y, x);
            if (max_height < height[y]) {
                son[x] = y;
                max_height = height[y];
            }
        }
    }
    height[x] = max_height + 1;
}

int clk = 0;
void dfs2(int x, int t) {
    top[x] = t;
    dfn[x] = ++clk; // 此为 1-index
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