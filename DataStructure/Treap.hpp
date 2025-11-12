#include <bits/stdc++.h>
using namespace std;

/*
Treap：一种弱平衡树，通过随机化权重实现的笛卡尔树
以下保留有选 Treap 和 FHQ Treap 的实现
前者通过左旋和右旋维持平衡
后者通过树的分裂和合并实现可持久化，是做题中更常用的
*/

// 有旋 Treap

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

// 插入节点
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

// 查找 x 的前驱（严格小于 x 中最大的数）
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

// 查找 x 的后继（严格大于 x 中最小的数）
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

/*
FHQ Treap：将 Treap 的旋转操作换成分裂和合并，可实现可持久化
分裂操作可分为按值分裂和按排名分裂两类，两者的复杂度都是 O(log n)
对于后者，它能维护中序遍历顺序不变
*/

// 有 key_count 版本

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

// 按值分裂
void split(int l, int r, int i, T x) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    }
    if (key[i] <= x) { // key 太小，放在左子树的右儿子
        rs[l] = i;
        split(i, r, rs[i], x);
    } else { // key 太大，放在右子树的左儿子
        ls[r] = i;
        split(l, i, ls[i], x);
    }
    up(i);
}

// // 按排名分裂（实际使用时两种方法只能保留一个）
// void split(int l, int r, int i, int rank) {
//     if (i == 0) {
//         rs[l] = ls[r] = 0;
//         return;
//     }
//     if (siz[ls[i]] + 1 <= rank) {
//         rs[l] = i;
//         split(i, r, rs[i], rank - siz[ls[i]] - 1);
//     } else {
//         ls[r] = i;
//         split(l, i, ls[i], rank);
//     }
//     up(i);
// }

// 合并
int merge(int l, int r) {
    // 其中一个为空，返回
    if (l == 0 || r == 0) {
        return l + r;
    }
    // 左树优先级大
    if (priority[l] >= priority[r]) {
        rs[l] = merge(rs[l], r);
        up(l);
        return l;
    }
    // 右树优先级大
    ls[r] = merge(l, ls[r]);
    up(r);
    return r;
}

// 查找节点，返回节点编号
int find(int i, T x) {
    if (i == 0) {
        return 0;
    }
    if (key[i] == x) {
        return i;
    }
    if (key[i] > x) {
        return find(ls[i], x);
    }
    return find(rs[i], x);
}

// 更改节点计数
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
    // 存在 key = x 的节点
    if (find(head, x) != 0) {
        change_count(head, x, 1);
        return;
    }
    split(0, 0, head, x);
    key[++cnt] = x;
    key_count[cnt] = siz[cnt] = 1;
    priority[cnt] = rand();
    head = merge(merge(rs[0], cnt), ls[0]);
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
    }
    return siz[ls[i]] + key_count[i] + small(rs[i], x);
}

// 查找排名：返回 x 的名次（从 1 开始）
int get_rank(T x) {
    return small(head, x) + 1;
}

// 查询第 k 大的数字（超过 size 则抛出异常）
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
        throw overflow_error("FHQ Treap Overflow");
    }
    return index(head, k);
}

// 查找 x 的前驱（严格小于 x 中最大的数）
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

// 查找 x 的后继（严格大于 x 中最小的数）
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

// 无 key_count 版本

using T = long long;
constexpr T INF = LLONG_MAX;
constexpr int MAXN = 100'001;

// 空间使用计数
int cnt = 0;

// 头节点
int head = 0;

// 节点的 key
T key[MAXN];

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
    fill(ls + 1, ls + n + 1, 0);
    fill(rs + 1, rs + n + 1, 0);
    fill(siz + 1, siz + n + 1, 0);
    fill(priority, priority + n + 1, 0);
}

// 更新节点信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
}

// 按值分裂
void split(int l, int r, int i, T x) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    }
    if (key[i] <= x) { // key 太小，放在左子树的右儿子
        rs[l] = i;
        split(i, r, rs[i], x);
    } else { // key 太大，放在右子树的左儿子，
        ls[r] = i;
        split(l, i, ls[i], x);
    }
    up(i);
}

// 按排名分裂（实际使用时两种方法只能保留一个）
// void split(int l, int r, int i, int rank) {
//     if (i == 0) {
//         rs[l] = ls[r] = 0;
//         return;
//     }
//     if (siz[ls[i]] + 1 <= rank) {
//         rs[l] = i;
//         split(i, r, rs[i], rank - siz[ls[i]] - 1);
//     } else {
//         ls[r] = i;
//         split(l, i, ls[i], rank);
//     }
//     up(i);
// }

// 合并
int merge(int l, int r) {
    // 其中一个为空，返回
    if (l == 0 || r == 0) {
        return l + r;
    }
    // 左树优先级大
    if (priority[l] >= priority[r]) {
        rs[l] = merge(rs[l], r);
        up(l);
        return l;
    }
    // 右树优先级大
    ls[r] = merge(l, ls[r]);
    up(r);
    return r;
}

// 插入节点：按 x 分裂，再插入，合并
void insert(T x) {
    split(0, 0, head, x);
    key[++cnt] = x;
    siz[cnt] = 1;
    priority[cnt] = rand();
    head = merge(merge(rs[0], cnt), ls[0]);
}

// 删除节点：先按 x 分裂，再按 x - 1 分裂，删除第二次分裂的头节点
void remove(T x) {
    split(0, 0, head, x);
    int lm = rs[0];
    int r = ls[0];
    split(0, 0, lm, x - 1);
    int l = rs[0];
    int m = ls[0];
    head = merge(merge(l, merge(ls[m], rs[m])), r);
}

// 查找排名：返回 x 的名次（从 1 开始）
int get_rank(T x) {
    // 按 x - 1 分裂，小于等于 x - 1 的树的大小加 1 即为排名
    split(0, 0, head, x - 1);
    int ans = siz[rs[0]] + 1;
    head = merge(rs[0], ls[0]);
    return ans;
}

// 查询第 k 大的数字（超过 size 则抛出异常）
T index(int i, int k) {
    if (siz[ls[i]] >= k) {
        return index(ls[i], k);
    }
    if (siz[ls[i]] + 1 < k) {
        return index(rs[i], k - siz[ls[i]] - 1);
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

// 查找 x 的后继（严格大于 x 中最小的数）
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