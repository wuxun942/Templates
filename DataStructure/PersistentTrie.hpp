#include <bits/stdc++.h>
using namespace std;

/*
字典树的模板见字符串分类

可持久化字典树：类似于单点修改的可持久化线段树
由于树高没有线段树的限制，所以一般使用时都需要注意树高
不过多数情况下只用在可持久化 0-1 Trie，所以也不需要过于注意
使用时需要记录每个节点的经过次数，如果次数变为 0 也视为删除

以下只保留可持久化 0-1 Trie 的模板
*/

// 最大版本数量
constexpr int MAX_Q = 200'000 + 5;

// 最大 bit_width
constexpr int MAX_B = 17;

// 可持久化 0-1 Trie 的使用空间 = q * log U
constexpr int MAX_M = MAX_Q * MAX_B;

// 空间使用计数
int cnt = 0;

// 每个版本的根节点
int roots[MAX_Q];

int tree[MAX_M][2];
int passby[MAX_M];

// 清空
void clear(int sz = cnt) {
    cnt = 0;
    memset(tree, 0, (sz + 1) * 2 * sizeof(int));
    fill(passby + 1, passby + sz + 1, 0);
}

int copy_node(int i) {
    tree[++cnt][0] = tree[i][0];
    tree[cnt][1] = tree[i][1];
    passby[cnt] = passby[i];
    return cnt;
}

// 建立 0 号版本
void build() {
    roots[0] = 0;
}

// 在 qv 版本的 0-1 Trie 上，添加数字 x，存储为 v 版本
void insert(int v, int qv, int x) {
    roots[v] = copy_node(roots[qv]);
    int cur = roots[v];
    for (int i = MAX_B - 1; i >= 0; --i) {
        int bit = x >> i & 1;
        tree[cur][bit] = copy_node(tree[cur][bit]);
        cur = tree[cur][bit];
        ++passby[cur];
    }
}

// 在 qv 版本的 0-1 Trie 上，查找与 x 的最大异或和
int query(int qv, int x) {
    int cur = roots[qv];
    int res = 0;
    for (int i = MAX_B - 1; i >= 0; --i) {
        int bit = x >> i & 1;
        if (int son = tree[cur][bit ^ 1]; son && passby[son]) {
            cur = son;
            res = res * 2 + 1;
        } else {
            cur = tree[cur][bit];
            res = res * 2;
        }
    }
    return res;
}