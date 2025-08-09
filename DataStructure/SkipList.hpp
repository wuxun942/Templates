#include <bits/stdc++.h>
using namespace std;

// 跳表

// 静态数组实现

// 最大层数
constexpr int MAXL = 20;

// 最大节点数量
constexpr int MAXN = 100'001;

// 空间使用计数
int cnt;

// 节点的 key
int key[MAXN];

// 节点 key 的计数
int key_count[MAXN]{};

// 节点层数
int level[MAXN];

// 每层节点的下一跳
int next_node[MAXN][MAXL + 1]{};

// 每层节点一跳的长度
int len[MAXN][MAXL + 1]{};

// 建立跳表，即建立 -inf 节点
void build() {
    cnt = 1;
    key[cnt] = INT_MIN;
    level[cnt] = MAXL;
}

// 清空跳表
void init(int n = MAXN) {
    build();
    fill(key_count, key_count + n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        memset(next_node[i], 0, sizeof(next_node[i]));
        memset(len[i], 0, sizeof(len[i]));
    }
}

// 生成随机层数
int random_level() {
    int ans = 1;
    while (ans <= MAXL && rand() / (double) RAND_MAX < 0.5) {
        ++ans;
    }
    return ans;
}

// 查找节点，返回节点编号
int find(int i, int h, int x) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < x) {
        i = next_node[i][h];
    }
    if (h == 1) {
        // 找到了
        if (next_node[i][h] != 0 && key[next_node[i][h]] == x) {
            return next_node[i][h];
        }
        // 没找到
        return 0;
    }
    return find(i, h - 1, x);
}

/*
添加一个数，分两类处理
1. 如果数字存在，那么增加出现次数
2. 如果数字不存在，那么添加节点
*/

// 增加次数：在 i 号节点的 h 层，x 出现次数加一
// 执行前保证 x 一定存在
void add_count(int i, int h, int x) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < x) {
        i = next_node[i][h];
    }
    if (h == 1) {
        ++key_count[next_node[i][h]]; // 下一个 key 一定是 x
    } else {
        add_count(i, h - 1, x);
    }
    ++len[i][h];
}

// 添加节点：在 i 号节点的 h 层，插入空间编号为 j 的节点
// 返回从 i 号节点出发，直到把空间编号为 j 的节点插入，底层总共跨过多少数字
int add_node(int i, int h, int j) {
    int right_cnt = 0;
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        right_cnt += len[i][h];
        i = next_node[i][h];
    }
    // 到达第一层，插入节点
    if (h == 1) {
        next_node[j][h] = next_node[i][h];
        next_node[i][h] = j;
        len[j][h] = key_count[next_node[j][h]];
        len[i][h] = key_count[next_node[i][h]];
        return right_cnt;
    }
    // 没到第一层
    int down_cnt = add_node(i, h - 1, j);
    if (h > level[j]) { // 层数太高，不需要插入
        ++len[i][h];
    } else {
        next_node[j][h] = next_node[i][h];
        next_node[i][h] = j;
        len[j][h] = len[i][h] + 1 - down_cnt - key_count[j];
        len[i][h] = down_cnt + key_count[j];
    }
    return right_cnt + down_cnt;
}

void add(int x) {
    if (find(1, MAXL, x) != 0) {
        add_count(1, MAXL, x);
    } else {
        key[++cnt] = x;
        key_count[cnt] = 1;
        level[cnt] = random_level();
        add_node(1, MAXL, cnt);
    }
}

// 删除节点：如果有多个，只删除一个
// 减少次数：当前在 i 号节点的 h 层，x 减少一个词频
void remove_count(int i, int h, int x) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < x) {
        i = next_node[i][h];
    }
    if (h == 1) {
        --key_count[next_node[i][h]];
    } else {
        remove_count(i, h - 1, x);
    }
    --len[i][h];
}

// 移除节点：当前在 i 号节点的 h 层，删除空间编号为 j 的节点
void remove_node(int i, int h, int j) {
    if (h == 0) {
        return;
    }
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        i = next_node[i][h];
    }
    if (h > level[j]) { // 层数太高
        --len[i][h];
    } else {
        next_node[i][h] = next_node[j][h];
        len[i][h] += len[j][h] - 1;
    }
    remove_node(i, h - 1, j);
}

void remove(int x) {
    int j = find(1, MAXL, x);
    // 节点存在
    if (j != 0) {
        if (key_count[j] > 1) {
            remove_count(1, MAXL, x);
        } else {
            remove_node(1, MAXL, j);
        }
    }
}

// 查找排名：有几个数比 x 小
int get_rank(int i, int h, int x) {
    if (h == 0) {
        return 0;
    }
    int right_cnt = 0;
    while (next_node[i][h] != 0 && key[next_node[i][h]] < x) {
        right_cnt += len[i][h];
        i = next_node[i][h];
    }
    return right_cnt + get_rank(i, h - 1, x);
}

int get_rank(int x) {
    return get_rank(1, MAXL, x) + 1;
}

// 查询第 k 大的数字（超过 size 则抛出异常）
int index(int i, int h, int x) {
    if (h == 0) {
        throw overflow_error("SkipList Overflow");
    }
    int c = 0;
    while (next_node[i][h] != 0 && c + len[i][h] < x) {
        c += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        return key[next_node[i][h]];
    }
    return index(i, h - 1, x - c);
}

int index(int x) {
    return index(1, MAXL, x);
}

// 查找 x 的前驱（小于 x 中最大的数），相当于 prev(lower_bound(x))
int prefix(int i, int h, int x) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < x) {
        i = next_node[i][h];
    }
    // 到最底层
    if (h == 1) {
        return key[i];
    }
    return prefix(i, h - 1, x);
}

int prefix(int x) {
    return prefix(1, MAXL, x);
}

// 查找 x 的后继（大于 x 中最小的数），相当于 upper_bound(x)
int suffix(int i, int h, int x) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < x) {
        i = next_node[i][h];
    }
    if (h == 1) {
        // 找不到
        if (next_node[i][h] == 0) {
            return INT_MAX;
        }
        if (key[next_node[i][h]] > x) {
            return key[next_node[i][h]];
        }
        // 下一个就是 x
        i = next_node[i][h];
        // 找不到
        if (next_node[i][h] == 0) {
            return INT_MAX;
        }
        return key[next_node[i][h]];
        
    }
    return suffix(i, h - 1, x);
}

int suffix(int x) {
    return suffix(1, MAXL, x);
}