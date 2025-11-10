#include <bits/stdc++.h>
using namespace std;

// FHQ Treap：将 Treap 的旋转操作换成分裂和合并，可实现可持久化

using T = long long;
constexpr int MAXN = 100'001;

// 空间使用计数
int cnt = 0;

// 头节点
int head = 0;

// 节点的 key
T key[MAXN];

// 节点 key 的计数
int key_count[MAXN];

// 左右儿子
int ls[MAXN];
int rs[MAXN];

// 子树的数字总数
int siz[MAXN];

// 随机生成的权重
int priority[MAXN];

// 整体初始化
void clear(int n = cnt) {
    cnt = head = 0;
    fill(key, key + n + 1, 0);
    fill(key_count, key_count + n + 1, 0);
    fill(ls + 1, ls + n + 1, 0);
    fill(rs + 1, rs + n + 1, 0);
    fill(siz + 1, siz + n + 1, 0);
    fill(priority, priority + n + 1, 0);
}

// 更新节点信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];
}

// 分裂
void split(int l, int r, int i, T x) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
    } else {
        if (key[i] <= x) {
            rs[l] = i;
            split(i, r, rs[i], x);
        } else {
            ls[r] = i;
            split(l, i, ls[i], x);
        }
        up(i);
    }
}

// 合并
int merge(int l, int r) {
    if (l == 0 || r == 0) {
        return l + r;
    }
    if (priority[l] >= priority[r]) {
        rs[l] = merge(rs[l], r);
        up(l);
        return l;
    } else {
        ls[r] = merge(l, ls[r]);
        up(r);
        return r;
    }
}

// 查找节点，返回节点编号
int find(int i, T x) {
    if (i == 0) {
        return 0;
    }
    if (key[i] == x) {
        return i;
    } else if (key[i] > x) {
        return find(ls[i], x);
    } else {
        return find(rs[i], x);
    }
}

void change_count(int i, T x, int change) {
    if (key[i] == x) {
        key_count[i] += change;
    } else if (key[i] > x) {
        change_count(ls[i], x, change);
    } else {
        change_count(rs[i], x, change);
    }
    up(i);
}

// 插入节点
void insert(T x) {
    if (find(head, x) != 0) {
        change_count(head, x, 1);
    } else {
        split(0, 0, head, x);
        key[++cnt] = x;
        key_count[cnt] = siz[cnt] = 1;
        priority[cnt] = rand();
        head = merge(merge(rs[0], cnt), ls[0]);
    }
}

// 删除节点
void remove(T x) {
    int i = find(head, x);
    if (i != 0) {
        if (key_count[i] > 1) {
            change_count(head, x, -1);
        } else {
            split(0, 0, head, x);
            int lm = rs[0];
            int r = ls[0];
            split(0, 0, lm, x - 1);
            int l = rs[0];
            head = merge(l, r);
        }
    }
}

// 有几个数比 x 小
int small(int i, T x) {
    if (i == 0) {
        return 0;
    }
    if (key[i] >= x) {
        return small(ls[i], x);
    } else {
        return siz[ls[i]] + key_count[i] + small(rs[i], x);
    }
}

// 查找排名：返回 x 的名次（从 1 开始）
int get_rank(T x) {
    return small(head, x) + 1;
}

// 查询第 k 大的数字（超过 size 则抛出异常）
T index(int i, int k) {
    if (siz[ls[i]] >= k) {
        return index(ls[i], k);
    } else if (siz[ls[i]] + key_count[i] < k) {
        return index(rs[i], k - siz[ls[i]] - key_count[i]);
    }
    return key[i];
}

T index(int k) {
    if (k > siz[head] || k <= 0) {
        throw overflow_error("FHQ Treap Overflow");
    }
    return index(head, k);
}

// 查找 x 的前驱（严格小于 x 中最大的数）
T pre(int i, T x) {
    if (i == 0) {
        return INT_MIN;
    }
    if (key[i] >= x) {
        return pre(ls[i], x);
    } else {
        return max(key[i], pre(rs[i], x));
    }
}

T pre(T x) {
   return pre(head, x);
}

// 查找 x 的后继（严格大于 x 中最小的数）
T post(int i, T x) {
    if (i == 0) {
        return INT_MAX;
    }
    if (key[i] <= x) {
        return post(rs[i], x);
    } else {
        return min(key[i], post(ls[i], x));
    }
}

T post(T x) {
    return post(head, x);
}