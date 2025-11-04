#include <bits/stdc++.h>
using namespace std;

// AVL 树

// 静态数组实现

// 最大节点数量
constexpr int MAXN = 100'001;

// 空间使用计数
int cnt = 0;

// 头结点编号
int head = 0;

// 节点的 key
int64_t key[MAXN];

// 子树高度
int height[MAXN]{};

// 左右儿子
int ls[MAXN]{};
int rs[MAXN]{};

// 节点 key 的计数
int key_count[MAXN];

// 子树的数字总数
int sz[MAXN]{};

// 整体初始化
void clear(int n) {
    head = cnt = 0;
    fill(height, height + n + 1, 0);
    fill(ls, ls + n + 1, 0);
    fill(rs, rs + n + 1, 0);
    fill(sz, sz + n + 1, 0);
}

// 更新节点信息
void up(int i) {
    sz[i] = sz[ls[i]] + sz[rs[i]] + key_count[i];
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
int insert(int i, int64_t x) {
    if (i == 0) {
        key[++cnt] = x;
        key_count[cnt] = sz[cnt] = height[cnt] = 1;
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

// 查找排名：有几个数比 x 小
int get_rank(int i, int64_t x) {
    if (i == 0) {
        return 0;
    }
    // x 太小了，只能往左找
    if (key[i] >= x) {
        return get_rank(ls[i], x);
    }
    // x 足够大，根节点和左子树都更小，还要往右找
    return sz[ls[i]] + key_count[i] + get_rank(rs[i], x);
}

int get_rank(int64_t x) {
    return get_rank(head, x) + 1;
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
int remove(int i, int64_t x) {
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

void remove(int64_t x) {
    if (get_rank(x) != get_rank(x + 1)) {
        head = remove(head, x);
    }
}

// 查询第 k 大的数字（超过 size 则抛出异常）
int64_t index(int i, int k) {
    int lsz = sz[ls[i]], c = key_count[i];
    if (lsz >= k) {
        return index(ls[i], k);
    }
    if (lsz + c < k) {
        return index(rs[i], k - lsz - c);
    }
    return key[i];
}

int index(int64_t k) {
    if (k > sz[head] || k <= 0) {
        throw overflow_error("AVL Overflow");
    }
    return index(head, k);
}

// 查找 x 的前驱（小于 x 中最大的数）
int64_t pre(int i, int64_t x) {
    // 找不到
    if (i == 0) {
        return LLONG_MIN;
    }
    if (key[i] >= x) {
        return pre(ls[i], x);
    }
    int64_t res = pre(rs[i], x);
    return res != LLONG_MIN ? res : key[i];
}

int64_t pre(int64_t x) {
    return pre(head, x);
}

// 查找 x 的后继（大于 x 中最小的数）
int64_t post(int i, int64_t x) {
    // 找不到
    if (i == 0) {
        return LLONG_MAX;
    }
    if (key[i] <= x) {
        return post(rs[i], x);
    }
    int res = post(ls[i], x);
    return res != LLONG_MAX ? res : key[i];
}

int64_t post(int64_t x) {
    return post(head, x);
}