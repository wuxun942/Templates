#include <bits/stdc++.h>
using namespace std;

/*
可持久化 FHQ Treap（只保留无 key_count 的版本）
时间复杂度不变，空间复杂度为 O(q * log n)，其中 q 为版本数量，n 为树中存储最大节点数
保守估计，预留空间为 q * 5 * log n，实际情况只会比这个更小
*/

using T = long long;
constexpr T INF = LLONG_MAX;

// 每个版本的最大节点数
constexpr int MAX_N = 100'000 + 5;

// 最大版本数
constexpr int MAX_Q = 100'000 + 5;

// 可持久化 FHQ Treap 使用空间 = q * 5 * log n
constexpr int MAX_V = MAX_Q * 5 * 17;

// 节点编号
int cnt = 0;

// 不同版本的根节点
int roots[MAX_N];

// 节点的 key
T key[MAX_V];

// 左右儿子
int ls[MAX_V];
int rs[MAX_V];

// 子树大小
int siz[MAX_V];

// 随机生成的权重
int priority[MAX_V];

// 拷贝节点，返回新节点编号
int copy_node(int i) {
    key[++cnt] = key[i];
    ls[cnt] = ls[i];
    rs[cnt] = rs[i];
    siz[cnt] = siz[i];
    priority[cnt] = priority[i];
    return cnt;
}

// 维护子树信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
}

// 按值分裂
void split(int l, int r, int i, T x) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    } 
    i = copy_node(i);
    if (key[i] <= x) {
        rs[l] = i;
        split(i, r, rs[i], x);
    } else {
        ls[r] = i;
        split(l, i, ls[i], x);
    }
    up(i);
}

// 合并
int merge(int l, int r) {
    if (l == 0 || r == 0) {
        return l + r;
    }
    if (priority[l] >= priority[r]) {
        l = copy_node(l);
        rs[l] = merge(rs[l], r);
        up(l);
        return l;
    }
    r = copy_node(r);
    ls[r] = merge(l, ls[r]);
    up(r);
    return r;
}

// 在以 i 为根节点的树上插入 x，作为 v 版本
void insert(int v, int i, T x) {
    split(0, 0, i, x);
    int l = rs[0];
    int r = ls[0];
    ls[0] = rs[0] = 0;
    ++cnt;
    key[cnt] = x;
    siz[cnt] = 1;
    priority[cnt] = rand();
    roots[v] = merge(merge(l, cnt), r);
}

// 在以 i 为根节点的树上删除 x（保证存在），作为 v 版本
void remove(int v, int i, T x) {
    split(0, 0, i, x);
    int lm = rs[0];
    int r = ls[0];
    split(0, 0, lm, x - 1);
    int l = rs[0];
    int m = ls[0];
    ls[0] = rs[0] = 0;
    roots[v] = merge(merge(l, merge(ls[m], rs[m])), r);
}

// 在以 i 为根节点的树上查找比 x 小的数字数量；如需查询排名，需要在此基础上加 1
int small(int i, T x) {
    if (i == 0) {
        return 0;
    }
    if (key[i] >= x) {
        return small(ls[i], x);
    }
    return siz[ls[i]] + 1 + small(rs[i], x);
}

// 在以 i 为根节点的树上查询第 k 小的数字
T index(int i, int k) {
    if (siz[ls[i]] >= k) {
        return index(ls[i], k);
    }
     if (siz[ls[i]] + 1 < k) {
        return index(rs[i], k - siz[ls[i]] - 1);
    }
    return key[i];
}

// 在以 i 为根节点的树上查找 x 的前驱
T pre(int i, T x) {
    if (i == 0) {
        return -INF;
    }
    if (key[i] >= x) {
        return pre(ls[i], x);
    }
    return max(key[i], pre(rs[i], x));
}

// 在以 i 为根节点的树上查找 x 的后继
T post(int i, T x) {
    if (i == 0) {
        return INF;
    }
    if (key[i] <= x) {
        return post(rs[i], x);
    }
    return min(key[i], post(ls[i], x));
}