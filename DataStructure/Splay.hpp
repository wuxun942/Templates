#include <bits/stdc++.h>
using namespace std;

// 最大节点数量
constexpr int MAXN = 100001;

// 空间使用计数
int cnt = 0;

// 头结点编号
int head = 0;

// 节点的 key
int key[MAXN];

// 节点的父节点
int fa[MAXN];

// 节点的左右儿子
int ls[MAXN];
int rs[MAXN];

// 子树大小
int siz[MAXN];

// 整体初始化
void clear(int n = cnt) {
    head = cnt = 0;    
    fill(key, key + n + 1, 0);
    fill(fa, fa + n + 1, 0);
    fill(ls, ls + n + 1, 0);
    fill(rs, rs + n + 1, 0);
    fill(siz, siz + n + 1, 0);
}

// 更新节点信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
}

int lr(int i) {
    return rs[fa[i]] == i ? 1 : 0;
}

void rotate(int i) {
    int f = fa[i], g = fa[f], soni = lr(i), sonf = lr(f);
    if (soni == 1) {
        rs[f] = ls[i];
        if (rs[f] != 0) {
            fa[rs[f]] = f;
        }
        ls[i] = f;
    } else {
        ls[f] = rs[i];
        if (ls[f] != 0) {
            fa[ls[f]] = f;
        }
        rs[i] = f;
    }
    if (g != 0) {
        if (sonf == 1) {
            rs[g] = i;
        } else {
            ls[g] = i;
        }
    }
    fa[f] = i;
    fa[i] = g;
    up(f);
    up(i);
}

void splay(int i, int goal) {
    int f = fa[i], g = fa[f];
    while (f != goal) {
        if (g != goal) {
            if (lr(i) == lr(f)) {
                rotate(f);
            } else {
                rotate(i);
            }
        }
        rotate(i);
        f = fa[i];
        g = fa[f];
    }
    if (goal == 0) {
        head = i;
    }
}

int find(int rank) {
    int i = head;
    while (i != 0) {
        if (siz[ls[i]] + 1 == rank) {
            return i;
        } else if (siz[ls[i]] >= rank) {
            i = ls[i];
        } else {
            rank -= siz[ls[i]] + 1;
            i = rs[i];
        }
    }
    return 0;
}

// 插入节点
void insert(int x) {
    key[++cnt] = x;
    siz[cnt] = 1;
    if (head == 0) {
        head = cnt;
    } else {
        int f = 0, i = head, son = 0;
        while (i != 0) {
            f = i;
            if (key[i] <= x) {
                son = 1;
                i = rs[i];
            } else {
                son = 0;
                i = ls[i];
            }
        }
        if (son == 1) {
            rs[f] = cnt;
        } else {
            ls[f] = cnt;
        }
        fa[cnt] = f;
        splay(cnt, 0);
    }
}

// 查找排名
int get_rank(int x) {
    int i = head, last = head;
    int ans = 0;
    while (i != 0) {
        last = i;
        if (key[i] >= x) {
            i = ls[i];
        } else {
            ans += siz[ls[i]] + 1;
            i = rs[i];
        }
    }
    splay(last, 0);
    return ans + 1;
}

// 查询第 k 小的数字
int index(int k) {
    int i = find(k);
    splay(i, 0);
    return key[i];
}

// 查找 x 的前驱
int pre(int x) {
    int i = head, last = head;
    int ans = INT_MIN;
    while (i != 0) {
        last = i;
        if (key[i] >= x) {
            i = ls[i];
        } else {
            ans = max(ans, key[i]);
            i = rs[i];
        }
    }
    splay(last, 0);
    return ans;
}

// 查找 x 的后继
int post(int x) {
    int i = head, last = head;
    int ans = INT_MAX;
    while (i != 0) {
        last = i;
        if (key[i] <= x) {
            i = rs[i];
        } else {
            ans = min(ans, key[i]);
            i = ls[i];
        }
    }
    splay(last, 0);
    return ans;
}

// 删除节点
void remove(int x) {
    int kth = get_rank(x);
    if (kth != get_rank(x + 1)) {
        int i = find(kth);
        splay(i, 0);
        if (ls[i] == 0) {
            head = rs[i];
        } else if (rs[i] == 0) {
            head = ls[i];
        } else {
            int j = find(kth + 1);
            splay(j, i);
            ls[j] = ls[i];
            fa[ls[j]] = j;
            up(j);
            head = j;
        }
        if (head != 0) {
            fa[head] = 0;
        }
    }
}