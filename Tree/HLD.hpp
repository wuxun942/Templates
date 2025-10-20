#include <bits/stdc++.h>
using namespace std;

/*
树链剖分 Heavy-Light Decompostion
常用的内容有重链剖分和长链剖分
可将树上任意一条路径划分为 O(log n) 条重链
*/

constexpr int MAXN = 100'001;
int head[MAXN]{};
int nxt[MAXN];
int to[MAXN];

int fa[MAXN];
int depth[MAXN]{}; // 或者初始化 depth[0] = 0
int sz[MAXN]{}; // 或者初始化 sz[0] = 0

// 每个子树的重儿子（子树最大）
int son[MAXN];

// 所在重链的头节点
int top[MAXN];

// 按重链分配 dfn
int dfn[MAXN];

// dfn 到原序号的逆映射
int seg[MAXN];

// 第一次遍历，建立 fa, depth, sz, son 信息
void dfs1(int x, int f) {
    fa[x] = f;
    sz[x] = 1;
    son[x] = 0;
    for (int e = head[x], y; e; e = nxt[e]) {
        y = to[e];
        if (y != f) {
            depth[y] = depth[x] + 1;
            dfs1(y, x);
            sz[x] += sz[y];
            if (sz[son[x]] < sz[y]) {
                son[x] = y;
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
            dfs2(y, t);
        }
    }
}