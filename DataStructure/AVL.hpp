#include <bits/stdc++.h>
using namespace std;

// AVL 树

// 静态数组实现

// 最大节点数量
constexpr int MX = 100'001;

// 空间使用计数
int cnt = 0;

// 头结点编号
int head = 0;

// 节点的 key
int key[MX];

// 子树高度
int height[MX]{};

// 左右儿子
int lch[MX]{};
int rch[MX]{};

// 节点 key 的计数
int key_count[MX];

// 子树的数字总数
int sz[MX]{};

void init(int n) {
    head = cnt = 0;
    fill(height, height + n, 0);
    fill(lch, lch + n, 0);
    fill(rch, rch + n, 0);
    fill(sz, sz + n, 0);
}

// 更新节点信息
void up(int i) {
    sz[i] = sz[lch[i]] + sz[rch[i]] + key_count[i];
    height[i] = max(height[lch[i]], height[rch[i]]) + 1;
}

// 左旋
int left_rotate(int i) {
    int r = rch[i];
    rch[i] = lch[r];
    lch[r] = i;
    up(i);
    up(r);
    return r;
}

// 右旋
int right_rotate(int i) {
    int l = lch[i];
    lch[i] = rch[l];
    rch[l] = i;
    up(i);
    up(l);
    return l;
}

// 维护子树平衡性
int maintain(int i) {
    int lh = height[lch[i]], rh = height[rch[i]];
    if (lh - rh > 1) {
        // LL 型违规
        if (height[lch[lch[i]]] >= height[rch[lch[i]]]) {
            i = right_rotate(i);
        // LR 型违规
        } else {
            lch[i] = left_rotate(lch[i]);
            i = right_rotate(i);
        }
    } else if (rh - lh > 1) {
        // RR 型违规
        if (height[rch[rch[i]]] >= height[lch[rch[i]]]) {
            i = left_rotate(i);
        // RL 型违规
        } else {
            rch[i] = right_rotate(rch[i]);
            i = left_rotate(i);
        }
    }
    return i;
}

// 增加节点：找位置 + 保持平衡性
int add(int i, int x) {
    if (i == 0) {
        key[++cnt] = x;
        key_count[cnt] = sz[cnt] = height[cnt] = 1;
        return cnt;
    }
    if (key[i] == x) {
        ++key_count[i];
    } else if (key[i] > x) {
        lch[i] = add(lch[i], x);
    } else {
        rch[i] = add(rch[i], x);
    }
    up(i);
    return maintain(i);
}

void add(int x) {
    head = add(head, x);
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
int get_rank(int i, int x) {
    if (i == 0) {
        return 0;
    }
    // x 太小了，只能往左找
    if (key[i] >= x) {
        return get_rank(lch[i], x);
    }
    // x 足够大，根节点和左子树都更小，还要往右找
    return sz[lch[i]] + key_count[i] + get_rank(rch[i], x);
}

int get_rank(int x) {
    return get_rank(head, x);
}

// 删除右子树的最左节点
int remove_most_left(int i, int most_left) {
    if (i == most_left) {
        return rch[i];
    }
    lch[i] = remove_most_left(lch[i], most_left);
    up(i);
    return maintain(i);
}

// 删除节点
int remove(int i, int x) {
    if (key[i] < x) {
        rch[i] = remove(rch[i], x);
    } else if (key[i] > x) {
        lch[i] = remove(lch[i], x);
    } else {
        if (key_count[i] > 1) {
            --key_count[i];
        } else {
            int l = lch[i], r = rch[i];
            if (l == 0 && r == 0) {
                return 0;
            }
            if (l != 0 && r == 0) {
                i = l;
            } else if (l == 0 && r != 0) {
                i = r;
            } else {
                int most_left = r;
                while (lch[most_left] != 0) {
                    most_left = lch[most_left];
                }
                rch[i] = remove_most_left(rch[i], most_left);
                lch[most_left] = lch[i];
                rch[most_left] = rch[i];
                i = most_left;
            }
        }
    }
    up(i);
    return maintain(i);
}

void remove(int x) {
    if (get_rank(x) != get_rank(x + 1)) {
        head = remove(head, x);
    }
}

// 查询第 k 大的数字（超过 k 则抛出异常）
int index(int i, int k) {
    if (i == 0) {
        throw overflow_error("AVL Overflow");
    }
    int lsz = sz[lch[i]], c = key_count[i];
    if (lsz >= k) {
        return index(lch[i], k);
    }
    if (lsz + c < k) {
        return index(rch[i], k - lsz - c);
    }
    return key[i];
}

int index(int k) {
    return index(head, k);
}

// 查找 x 的前驱（小于 x 中最大的数），相当于 prev(lower_bound(x))
int prefix(int i, int x) {
    // 没有前驱
    if (i == 0) {
        return INT_MIN;
    }
    if (key[i] >= x) {
        return prefix(lch[i], x);
    }
    int res = prefix(rch[i], x);
    return res != INT_MIN ? res : key[i];
}

int prefix(int x) {
    return prefix(head, x);
}

// 查找 x 的后继（大于 x 中最小的数），相当于 upper_bound(x)
int suffix(int i, int x) {
    // 没有后继
    if (i == 0) {
        return INT_MAX;
    }
    if (key[i] <= x) {
        return suffix(rch[i], x);
    }
    int res = suffix(lch[i], x);
    return res != INT_MAX ? res : key[i];
}

int suffix(int x) {
    return suffix(head, x);
}

// 封装类 + vector 实现
template<typename T>
class AVL {
    // 空间使用计数
    int cnt = 0;

    // 头结点编号
    int head = 0;

    // 节点的 key
    vector<T> key;

    // 子树高度
    vector<int> height;

    // 左右儿子
    vector<int> lch[MX];
    vector<int> rch[MX];

    // 节点 key 的计数
    vector<int> key_count[MX];

    // 子树的数字总数
    vector<int> sz[MX];

    // 更新节点信息
    void up(int i) {
        sz[i] = sz[lch[i]] + sz[rch[i]] + key_count[i];
        height[i] = max(height[lch[i]], height[rch[i]]) + 1;
    }

    // 左旋
    int left_rotate(int i) {
        int r = rch[i];
        rch[i] = lch[r];
        lch[r] = i;
        up(i);
        up(r);
        return r;
    }

    // 右旋
    int right_rotate(int i) {
        int l = lch[i];
        lch[i] = rch[l];
        rch[l] = i;
        up(i);
        up(l);
        return l;
    }

    // 维护子树平衡性
    int maintain(int i) {
        int lh = height[lch[i]], rh = height[rch[i]];
        if (lh - rh > 1) {
            // LL 型违规
            if (height[lch[lch[i]]] >= height[rch[lch[i]]]) {
                i = right_rotate(i);
            // LR 型违规
            } else {
                lch[i] = left_rotate(lch[i]);
                i = right_rotate(i);
            }
        } else if (rh - lh > 1) {
            // RR 型违规
            if (height[rch[rch[i]]] >= height[lch[rch[i]]]) {
                i = left_rotate(i);
            // RL 型违规
            } else {
                rch[i] = right_rotate(rch[i]);
                i = left_rotate(i);
            }
        }
        return i;
    }

    // 增加节点：找位置 + 保持平衡性
    int add(int i, T x) {
        if (i == 0) {
            key[++cnt] = x;
            key_count[cnt] = sz[cnt] = height[cnt] = 1;
            return cnt;
        }
        if (key[i] == x) {
            ++key_count[i];
        } else if (key[i] > x) {
            lch[i] = add(lch[i], x);
        } else {
            rch[i] = add(rch[i], x);
        }
        up(i);
        return maintain(i);
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
    int get_rank(int i, T x) {
        if (i == 0) {
            return 0;
        }
        // x 太小了，只能往左找
        if (key[i] >= x) {
            return get_rank(lch[i], x);
        }
        // x 足够大，根节点和左子树都更小，还要往右找
        return sz[lch[i]] + key_count[i] + get_rank(rch[i], x);
    }

    // 删除右子树的最左节点
    int remove_most_left(int i, int most_left) {
        if (i == most_left) {
            return rch[i];
        }
        lch[i] = remove_most_left(lch[i], most_left);
        up(i);
        return maintain(i);
    }

    // 删除节点
    int remove(int i, T x) {
        if (key[i] < x) {
            rch[i] = remove(rch[i], x);
        } else if (key[i] > x) {
            lch[i] = remove(lch[i], x);
        } else {
            if (key_count[i] > 1) {
                --key_count[i];
            } else {
                int l = lch[i], r = rch[i];
                if (l == 0 && r == 0) {
                    return 0;
                }
                if (l != 0 && r == 0) {
                    i = l;
                } else if (l == 0 && r != 0) {
                    i = r;
                } else {
                    int most_left = r;
                    while (lch[most_left] != 0) {
                        most_left = lch[most_left];
                    }
                    rch[i] = remove_most_left(rch[i], most_left);
                    lch[most_left] = lch[i];
                    rch[most_left] = rch[i];
                    i = most_left;
                }
            }
        }
        up(i);
        return maintain(i);
    }

    // 查询第 k 大的数字（超过 k 则抛出异常）
    T index(int i, int k) {
        if (i == 0) {
            throw overflow_error("AVL Overflow");
        }
        int lsz = sz[lch[i]], c = key_count[i];
        if (lsz >= k) {
            return index(lch[i], k);
        }
        if (lsz + c < k) {
            return index(rch[i], k - lsz - c);
        }
        return key[i];
    }

    // 查找 x 的前驱（小于 x 中最大的数），相当于 prev(lower_bound(x))
    T prefix(int i, int x) {
        // 没有前驱
        if (i == 0) {
            return INT_MIN;
        }
        if (key[i] >= x) {
            return prefix(lch[i], x);
        }
        T res = prefix(rch[i], x);
        return res != INT_MIN ? res : key[i];
    }

    // 查找 x 的后继（大于 x 中最小的数），相当于 upper_bound(x)
    T suffix(int i, int x) {
        // 没有后继
        if (i == 0) {
            return INT_MAX;
        }
        if (key[i] <= x) {
            return suffix(rch[i], x);
        }
        T res = suffix(lch[i], x);
        return res != INT_MAX ? res : key[i];
    }

public:
    // 初始化：传入树的大小
    AVL (int n) {
        key.resize(n + 1);
        height.resize(n + 1);
        lch.resize(n + 1);
        rch.resize(n + 1);
        key_count.resize(n + 1);
        sz.resize(n + 1);
    }

    void add(T x) {
        head = add(head, x);
    }

    int get_rank(T x) {
        return get_rank(head, x);
    }

    void remove(T x) {
        if (get_rank(x) != get_rank(x + 1)) {
            head = remove(head, x);
        }
    }

    T index(int k) {
        return index(head, k);
    }

    T prefix(int x) {
        return prefix(head, x);
    }

    T suffix(int x) {
        return suffix(head, x);
    }
};