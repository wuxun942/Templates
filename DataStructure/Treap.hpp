#include <bits/stdc++.h>
using namespace std;

/*
Treap：一种弱平衡树，通过随机化权重实现的笛卡尔树
此处保留有旋 Treap 和 FHQ Treap 的实现
*/

using T = long long;
constexpr T INF = LLONG_MAX;

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
int ls[MAXN]{};
int rs[MAXN]{};

// 子树的数字总数
int siz[MAXN]{};

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

// 左旋
int left_rotate(int i) {
    int r = rs[i];
    rs[i] = ls[r];
    ls[r] = i;
    up(i);
    up(r);
    return r;
}

// 右旋
int right_rotate(int i) {
   int l = ls[i];
   ls[i] = rs[l];
   rs[l] = i;
   up(i);
   up(l);
   return l;
}

// 增加节点
int insert(int i, T x) {
    if (i == 0) {
        key[++cnt] = x;
        key_count[cnt] = siz[cnt] = 1;
        priority[cnt] = rand();
        return cnt;
    }
    if (key[i] == x) {
        key_count[i]++;
    } else if (key[i] > x) {
        ls[i] = insert(ls[i], x);
    } else {
        rs[i] = insert(rs[i], x);
    }
    up(i);
    if (ls[i] != 0 && priority[ls[i]] > priority[i]) {
        return right_rotate(i);
    }
    if (rs[i] != 0 && priority[rs[i]] > priority[i]) {
        return left_rotate(i);
    }
    return i;
}

void insert(T x) {
    head = insert(head, x);
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
    return small(head, x) + 1;
}

// 查询第 k 大的数字
T index(int i, int k) {
    if (siz[ls[i]] >= k) {
        return index(ls[i], k);
    }
    if (siz[ls[i]] + key_count[i] < k) {
        return index(rs[i], k - siz[ls[i]] - key_count[i]);
    }
    return key[i];
}

T index(int k) {
    if (k > siz[head] || k <= 0) {
        throw overflow_error("Treap Overflow");
    }
    return index(head, k);
}

// 查找 x 的前驱（小于 x 中最大的数）
T pre(int i, T x) {
    if (i == 0) {
        return -INF;
    }
    if (key[i] >= x) {
        return pre(ls[i], x);
    }
    return max(key[i], pre(rs[i], x));
}

T pre(T x) {
    return pre(head, x);
}

// 查找 x 的后继（大于 x 中最小的数）
T post(int i, T x) {
    if (i == 0) {
        return INF;
    }
    if (key[i] <= x) {
        return post(rs[i], x);
    }
    return min(key[i], post(ls[i], x));
}

T post(T x) {
    return post(head, x);
}

// 删除节点：如果有多个，只删除一个
int remove(int i, T x) {
    if (key[i] < x) {
        rs[i] = remove(rs[i], x);
    } else if (key[i] > x) {
        ls[i] = remove(ls[i], x);
    } else {
        if (key_count[i] > 1) {
            key_count[i]--;
        } else {
            if (ls[i] == 0 && rs[i] == 0) {
                return 0;
            } else if (ls[i] != 0 && rs[i] == 0) {
                i = ls[i];
            } else if (ls[i] == 0 && rs[i] != 0) {
                i = rs[i];
            } else {
                if (priority[ls[i]] >= priority[rs[i]]) {
                    i = right_rotate(i);
                    rs[i] = remove(rs[i], x);
                } else {
                    i = left_rotate(i);
                    ls[i] = remove(ls[i], x);
                }
            }
        }
    }
    up(i);
    return i;
}

void remove(T x) {
    if (get_rank(x) != get_rank(x + 1)) {
        head = remove(head, x);
    }
}