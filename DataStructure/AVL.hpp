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
long long key[MX];

// 子树高度
int height[MX]{};

// 左右儿子
int left_child[MX]{};
int right_child[MX]{};

// 节点 key 的计数
int key_count[MX];

// 子树的数字总数
int sz[MX]{};

void init(int n) {
    head = cnt = 0;
    fill(height, height + n + 1, 0);
    fill(left_child, left_child + n + 1, 0);
    fill(right_child, right_child + n + 1, 0);
    fill(sz, sz + n + 1, 0);
}

// 更新节点信息
void up(int i) {
    sz[i] = sz[left_child[i]] + sz[right_child[i]] + key_count[i];
    height[i] = max(height[left_child[i]], height[right_child[i]]) + 1;
}

// 左旋
int left_rotate(int i) {
    int r = right_child[i];
    right_child[i] = left_child[r];
    left_child[r] = i;
    up(i);
    up(r);
    return r;
}

// 右旋
int right_rotate(int i) {
    int l = left_child[i];
    left_child[i] = right_child[l];
    right_child[l] = i;
    up(i);
    up(l);
    return l;
}

// 维护子树平衡性
int maintain(int i) {
    int lh = height[left_child[i]], rh = height[right_child[i]];
    if (lh - rh > 1) {
        // LL 型违规
        if (height[left_child[left_child[i]]] >= height[right_child[left_child[i]]]) {
            i = right_rotate(i);
        // LR 型违规
        } else {
            left_child[i] = left_rotate(left_child[i]);
            i = right_rotate(i);
        }
    } else if (rh - lh > 1) {
        // RR 型违规
        if (height[right_child[right_child[i]]] >= height[left_child[right_child[i]]]) {
            i = left_rotate(i);
        // RL 型违规
        } else {
            right_child[i] = right_rotate(right_child[i]);
            i = left_rotate(i);
        }
    }
    return i;
}

// 增加节点：找位置 + 保持平衡性
int add(int i, long long x) {
    if (i == 0) {
        key[++cnt] = x;
        key_count[cnt] = sz[cnt] = height[cnt] = 1;
        return cnt;
    }
    if (key[i] == x) {
        ++key_count[i];
    } else if (key[i] > x) {
        left_child[i] = add(left_child[i], x);
    } else {
        right_child[i] = add(right_child[i], x);
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
int get_rank(int i, long long x) {
    if (i == 0) {
        return 0;
    }
    // x 太小了，只能往左找
    if (key[i] >= x) {
        return get_rank(left_child[i], x);
    }
    // x 足够大，根节点和左子树都更小，还要往右找
    return sz[left_child[i]] + key_count[i] + get_rank(right_child[i], x);
}

int get_rank(long long x) {
    return get_rank(head, x);
}

// 删除右子树的最左节点
int remove_most_left(int i, int most_left) {
    if (i == most_left) {
        return right_child[i];
    }
    left_child[i] = remove_most_left(left_child[i], most_left);
    up(i);
    return maintain(i);
}

// 删除节点：如果有多个，只删除一个
int remove(int i, long long x) {
    if (key[i] < x) {
        right_child[i] = remove(right_child[i], x);
    } else if (key[i] > x) {
        left_child[i] = remove(left_child[i], x);
    } else {
        if (key_count[i] > 1) {
            --key_count[i];
        } else {
            int l = left_child[i], r = right_child[i];
            if (l == 0 && r == 0) {
                return 0;
            }
            if (l != 0 && r == 0) {
                i = l;
            } else if (l == 0 && r != 0) {
                i = r;
            } else {
                int most_left = r;
                while (left_child[most_left] != 0) {
                    most_left = left_child[most_left];
                }
                right_child[i] = remove_most_left(right_child[i], most_left);
                left_child[most_left] = left_child[i];
                right_child[most_left] = right_child[i];
                i = most_left;
            }
        }
    }
    up(i);
    return maintain(i);
}

void remove(long long x) {
    if (get_rank(x) != get_rank(x + 1)) {
        head = remove(head, x);
    }
}

// 查询第 k 大的数字（超过 size 则抛出异常）
long long index(int i, int k) {
    int lsz = sz[left_child[i]], c = key_count[i];
    if (lsz >= k) {
        return index(left_child[i], k);
    }
    if (lsz + c < k) {
        return index(right_child[i], k - lsz - c);
    }
    return key[i];
}

int index(long long k) {
    if (k > sz[head] || k <= 0) {
        throw overflow_error("AVL Overflow");
    }
    return index(head, k);
}

// 查找 x 的前驱（小于 x 中最大的数），相当于 prev(lower_bound(x))
long long prefix(int i, long long x) {
    // 找不到
    if (i == 0) {
        return LLONG_MIN;
    }
    if (key[i] >= x) {
        return prefix(left_child[i], x);
    }
    long long res = prefix(right_child[i], x);
    return res != LLONG_MIN ? res : key[i];
}

long long prefix(long long x) {
    return prefix(head, x);
}

// 查找 x 的后继（大于 x 中最小的数），相当于 upper_bound(x)
long long suffix(int i, long long x) {
    // 找不到
    if (i == 0) {
        return LLONG_MAX;
    }
    if (key[i] <= x) {
        return suffix(right_child[i], x);
    }
    int res = suffix(left_child[i], x);
    return res != LLONG_MAX ? res : key[i];
}

long long suffix(long long x) {
    return suffix(head, x);
}

// 封装类 + vector 实现
template<typename T, size_t MX>
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
    vector<int> left_child[MX];
    vector<int> right_child[MX];

    // 节点 key 的计数
    vector<int> key_count[MX];

    // 子树的数字总数
    vector<int> sz[MX];

    // 更新节点信息
    void up(int i) {
        sz[i] = sz[left_child[i]] + sz[right_child[i]] + key_count[i];
        height[i] = max(height[left_child[i]], height[right_child[i]]) + 1;
    }

    // 左旋
    int left_rotate(int i) {
        int r = right_child[i];
        right_child[i] = left_child[r];
        left_child[r] = i;
        up(i);
        up(r);
        return r;
    }

    // 右旋
    int right_rotate(int i) {
        int l = left_child[i];
        left_child[i] = right_child[l];
        right_child[l] = i;
        up(i);
        up(l);
        return l;
    }

    // 维护子树平衡性
    int maintain(int i) {
        int lh = height[left_child[i]], rh = height[right_child[i]];
        if (lh - rh > 1) {
            // LL 型违规
            if (height[left_child[left_child[i]]] >= height[right_child[left_child[i]]]) {
                i = right_rotate(i);
            // LR 型违规
            } else {
                left_child[i] = left_rotate(left_child[i]);
                i = right_rotate(i);
            }
        } else if (rh - lh > 1) {
            // RR 型违规
            if (height[right_child[right_child[i]]] >= height[left_child[right_child[i]]]) {
                i = left_rotate(i);
            // RL 型违规
            } else {
                right_child[i] = right_rotate(right_child[i]);
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
            left_child[i] = add(left_child[i], x);
        } else {
            right_child[i] = add(right_child[i], x);
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
            return get_rank(left_child[i], x);
        }
        // x 足够大，根节点和左子树都更小，还要往右找
        return sz[left_child[i]] + key_count[i] + get_rank(right_child[i], x);
    }

    // 删除右子树的最左节点
    int remove_most_left(int i, int most_left) {
        if (i == most_left) {
            return right_child[i];
        }
        left_child[i] = remove_most_left(left_child[i], most_left);
        up(i);
        return maintain(i);
    }

    // 删除节点
    int remove(int i, T x) {
        if (key[i] < x) {
            right_child[i] = remove(right_child[i], x);
        } else if (key[i] > x) {
            left_child[i] = remove(left_child[i], x);
        } else {
            if (key_count[i] > 1) {
                --key_count[i];
            } else {
                int l = left_child[i], r = right_child[i];
                if (l == 0 && r == 0) {
                    return 0;
                }
                if (l != 0 && r == 0) {
                    i = l;
                } else if (l == 0 && r != 0) {
                    i = r;
                } else {
                    int most_left = r;
                    while (left_child[most_left] != 0) {
                        most_left = left_child[most_left];
                    }
                    right_child[i] = remove_most_left(right_child[i], most_left);
                    left_child[most_left] = left_child[i];
                    right_child[most_left] = right_child[i];
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
        int lsz = sz[left_child[i]], c = key_count[i];
        if (lsz >= k) {
            return index(left_child[i], k);
        }
        if (lsz + c < k) {
            return index(right_child[i], k - lsz - c);
        }
        return key[i];
    }

    // 查找 x 的前驱（小于 x 中最大的数），相当于 prev(lower_bound(x))
    T prefix(int i, T x) {
        // 没有前驱
        if (i == 0) {
            return INT_MIN;
        }
        if (key[i] >= x) {
            return prefix(left_child[i], x);
        }
        T res = prefix(right_child[i], x);
        return res != INT_MIN ? res : key[i];
    }

    // 查找 x 的后继（大于 x 中最小的数），相当于 upper_bound(x)
    T suffix(int i, T x) {
        // 没有后继
        if (i == 0) {
            return INT_MAX;
        }
        if (key[i] <= x) {
            return suffix(right_child[i], x);
        }
        T res = suffix(left_child[i], x);
        return res != INT_MAX ? res : key[i];
    }

public:
    // 初始化：传入树的大小
    AVL (int n) {
        key.resize(n + 1);
        height.resize(n + 1);
        left_child.resize(n + 1);
        right_child.resize(n + 1);
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

    T prefix(T x) {
        return prefix(head, x);
    }

    T suffix(T x) {
        return suffix(head, x);
    }
};