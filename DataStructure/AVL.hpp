#include <bits/stdc++.h>
using namespace std;

/*
AVL 树：一种自平衡树
*/ 

using T = long long;
constexpr T INF = LLONG_MAX;

// 最大节点数量
constexpr int MAXN = 100'001;

// 空间使用计数
int cnt = 0;

// 头结点编号
int head = 0;

// 节点的 key
T key[MAXN];

// 节点 key 的计数
int key_count[MAXN];

// 子树高度
int height[MAXN]{};

// 左右儿子
int ls[MAXN]{};
int rs[MAXN]{};

// 子树的数字总数
int siz[MAXN]{};

// 整体初始化
void clear(int n = cnt) {
    head = cnt = 0;
    fill(key, key + n + 1, 0);
    fill(key_count, key_count + n + 1, 0);
    fill(height, height + n + 1, 0);
    fill(ls, ls + n + 1, 0);
    fill(rs, rs + n + 1, 0);
    fill(siz, siz + n + 1, 0);
}

// 更新节点信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];
    height[i] = max(height[ls[i]], height[rs[i]]) + 1;
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

// 维护子树平衡性
int maintain(int i) {
    int lh = height[ls[i]], rh = height[rs[i]];
    if (lh - rh > 1) {
        // LL 型违规
        if (height[ls[ls[i]]] >= height[rs[ls[i]]]) {
            i = right_rotate(i);
        // LR 型违规
        } else {
            ls[i] = left_rotate(ls[i]);
            i = right_rotate(i);
        }
    } else if (rh - lh > 1) {
        // RR 型违规
        if (height[rs[rs[i]]] >= height[ls[rs[i]]]) {
            i = left_rotate(i);
        // RL 型违规
        } else {
            rs[i] = right_rotate(rs[i]);
            i = left_rotate(i);
        }
    }
    return i;
}

// 增加节点：找位置 + 保持平衡性
int insert(int i, T x) {
    if (i == 0) {
        key[++cnt] = x;
        key_count[cnt] = siz[cnt] = height[cnt] = 1;
        return cnt;
    }
    if (key[i] == x) {
        ++key_count[i];
    } else if (key[i] > x) {
        ls[i] = insert(ls[i], x);
    } else {
        rs[i] = insert(rs[i], x);
    }
    up(i);
    return maintain(i);
}

void insert(int x) {
    head = insert(head, x);
}

/*
删除节点的流程：
1. 查找节点 key 的排名，确认节点是否存在；如不存在，就直接退出
2. 节点存在，执行 remove 逻辑
2.1 叶子节点，直接删除
2.2 只有左子树或者只有右子树，删除后把子树挂到根节点
2.3 既有左子树又有右子树，根节点替换为把右子树的最左节点（也可以是左子树的最右节点）
*/

// 有几个数比 x 小
int small(int i, T x) {
    if (i == 0) {
        return 0;
    }
    // x 太小了，只能往左找
    if (key[i] >= x) {
        return small(ls[i], x);
    }
    // x 足够大，根节点和左子树都更小，还要往右找
    return siz[ls[i]] + key_count[i] + small(rs[i], x);
}

// 查找排名：返回 x 的名次（从 1 开始）
int get_rank(T x) {
    return small(head, x) + 1;
}

// 删除右子树的最左节点
int remove_most_left(int i, int most_left) {
    if (i == most_left) {
        return rs[i];
    }
    ls[i] = remove_most_left(ls[i], most_left);
    up(i);
    return maintain(i);
}

// 删除节点：如果有多个，只删除一个
int remove(int i, T x) {
    if (key[i] < x) {
        rs[i] = remove(rs[i], x);
    } else if (key[i] > x) {
        ls[i] = remove(ls[i], x);
    } else {
        if (key_count[i] > 1) {
            --key_count[i];
        } else {
            int l = ls[i], r = rs[i];
            if (l == 0 && r == 0) {
                return 0;
            }
            if (l != 0 && r == 0) {
                i = l;
            } else if (l == 0 && r != 0) {
                i = r;
            } else {
                int most_left = r;
                while (ls[most_left] != 0) {
                    most_left = ls[most_left];
                }
                rs[i] = remove_most_left(rs[i], most_left);
                ls[most_left] = ls[i];
                rs[most_left] = rs[i];
                i = most_left;
            }
        }
    }
    up(i);
    return maintain(i);
}

void remove(T x) {
    if (get_rank(x) != get_rank(x + 1)) {
        head = remove(head, x);
    }
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

int index(T k) {
    if (k > siz[head] || k <= 0) {
        throw overflow_error("AVL Overflow");
    }
    return index(head, k);
}

// 查找 x 的前驱（小于 x 中最大的数）
T pre(int i, T x) {
    // 找不到
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
    // 找不到
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