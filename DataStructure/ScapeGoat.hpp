#include <bits/stdc++.h>
using namespace std;

/*
替罪羊树：保证每个子树的大小均小于 alpha * n
1. 如果插入后仍然满足，则不更改根节点
2. 如果插入后不满足，则按中序遍历重构

一般取 alpha = 0.7
*/

using T = long long;
constexpr T INF = LLONG_MAX;

constexpr double ALPHA = 0.7;
constexpr int MAX_N = 100'000 + 5;

// 空间使用计数
int cnt = 0;

// 根节点
int root = 0;

// 节点的 key
T key[MAX_N];

// 节点 key 的计数
int key_count[MAX_N];

// 左右儿子
int ls[MAX_N];
int rs[MAX_N];

// 子树的数字总数
int siz[MAX_N];

// 子树节点总数
int diff[MAX_N];

// 中序收集节点的数组
int collect[MAX_N];

// 中序收集节点的计数
int ci;

// 最上方的不平衡节点
int top;

// top的父节点
int father;

// top是父节点的什么孩子，1代表左孩子，2代表右孩子
int side;

// 整体初始化
void clear(int n = cnt) {
    cnt = root = 0;
    fill(key, key + n + 1, 0);
    fill(key_count, key_count + n + 1, 0);
    fill(ls, ls + n + 1, 0);
    fill(rs, rs + n + 1, 0);
    fill(siz, siz + n + 1, 0);
    fill(diff, diff + n + 1, 0);
}

// 新建一个 key = x 的节点并初始化
int init(T x) {
    key[++cnt] = x;
    ls[cnt] = rs[cnt] = 0;
    key_count[cnt] = siz[cnt] = diff[cnt] = 1;
    return cnt;
}

// 更新节点信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];
    diff[i] = diff[ls[i]] + diff[rs[i]] + 1; // 最后一项也能改成 (key_count[i] > 0 ? 1 : 0)
}

// 中序遍历收集信息
void inorder(int i) {
    if (i != 0) {
        inorder(ls[i]);
        if (key_count[i] > 0) {
            collect[++ci] = i;
        }
        inorder(rs[i]);
    }
}

// 分治重建
int build(int l, int r) {
    if (l > r) {
        return 0;
    }
    int m = (l + r) / 2;
    int h = collect[m];
    ls[h] = build(l, m - 1);
    rs[h] = build(m + 1, r);
    up(h);
    return h;
}

// 重建不平衡子树
void rebuild() {
    if (top != 0) {
        ci = 0;
        inorder(top);
        if (ci > 0) {
            if (father == 0) {
                root = build(1, ci);
            } else if (side == 1) {
                ls[father] = build(1, ci);
            } else {
                rs[father] = build(1, ci);
            }
        }
    }
}

// 平衡性判断
bool balance(int i) {
    return ALPHA * diff[i] >= max(diff[ls[i]], diff[rs[i]]);
}

// 插入节点
// f: 父节点，s: i 是 f 的哪个儿子
void insert(int i, int f, int s, T x) {
    if (i == 0) {
        if (f == 0) {
            root = init(x);
        } else if (s == 1) {
            ls[f] = init(x);
        } else {
            rs[f] = init(x);
        }
    } else {
        if (key[i] == x) {
            key_count[i]++;
        } else if (key[i] > x) {
            insert(ls[i], i, 1, x);
        } else {
            insert(rs[i], i, 2, x);
        }
        up(i);
        if (!balance(i)) {
            top = i;
            father = f;
            side = s;
        }
    }
}

void insert(T x) {
    top = father = side = 0;
    insert(root, 0, 0, x);
    rebuild();
}

// 有几个数比 x 小
int small(int i, T x) {
    if (i == 0) {
        return 0;
    }
    if (key[i] >= x) {
        return small(ls[i], x);
    }
    return siz[ls[i]] + key_count[i] + small(rs[i], x);
}

// 查找排名：返回 x 的名次（从 1 开始）
int get_rank(T x) {
    return small(root, x) + 1;
}

// 查询第 k 小的数字（超过 size 则抛出异常）
T index(int i, int k) {
    if (siz[ls[i]] >= k) {
        return index(ls[i], k);
    } else if (siz[ls[i]] + key_count[i] < k) {
        return index(rs[i], k - siz[ls[i]] - key_count[i]);
    }
    return key[i];
}

T index(int k) {
    if (k > siz[root] || k <= 0) {
        throw overflow_error("ScapeGoat Overflow");
    }
    return index(root, k);
}

// 查找 x 的前驱（严格小于 x 中最大的数）
int pre(T x) {
    int kth = get_rank(x);
    if (kth == 1) {
        return -INF;
    }
    return index(kth - 1);
}

// 查找 x 的后继（严格大于 x 中最小的数）
int post(T x) {
    int kth = get_rank(x + 1);
    if (kth == siz[root] + 1) {
        return INF;
    }
    return index(kth);
}

// 删除节点：如果有多个，只删除一个
void remove(int i, int f, int s, T x) {
    if (key[i] == x) {
        key_count[i]--;
    } else if (key[i] > x) {
        remove(ls[i], i, 1, x);
    } else {
        remove(rs[i], i, 2, x);
    }
    up(i);
    if (!balance(i)) {
        top = i;
        father = f;
        side = s;
    }
}

void remove(T x) {
    // 存在 x
    if (get_rank(x) != get_rank(x + 1)) {
        top = father = side = 0;
        remove(root, 0, 0, x);
        rebuild();
    }
}