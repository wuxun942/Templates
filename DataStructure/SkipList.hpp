#include <bits/stdc++.h>
using namespace std;

// 跳表

// 静态数组实现

// 最大层数
constexpr int MAXL = 20;

// 最大节点数量
constexpr int MAXN = 100'001;

// 空间使用计数
int cnt = 0;

// 节点的 key
int key[MAXN];

// 节点 key 的计数
int key_count[MAXN];

// 节点层数
int level[MAXN];

// 每层节点的下一跳
int next_node[MAXN][MAXL + 1];

// 每层节点一跳的长度
int len[MAXN][MAXL + 1];

// 清空跳表
void init(int n = MAXN) {
    cnt = 0;
    fill(key, key + n + 1, 0);
    fill(key_count, key_count + n + 1, 0);
    fill(level, level + n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        memset(next_node[i], 0, sizeof(next_node[i]));
        memset(len[i], 0, sizeof(len[i]));
    }
}

// 建立跳表，即建立 -inf 节点
void build(int n) {
    cnt = 1;
    key[cnt] = INT_MIN;
    level[cnt] = MAXL;
}

// 随机层数生成
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

// 增加次数：执行前保证 x 一定存在
void add_count(int i, int h, int x) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < x) {
        i = next_node[i][h];
    }
    if (h == 1) {
        ++key_count[next_node[i][h]];
    } else {
        add_count(i, h - 1, x);
    }
    ++len[i][h];
}

// 添加节点
int add_node(int i, int h, int j) {
    int right_cnt = 0;
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        right_cnt += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        next_node[j][h] = next_node[i][h];
        next_node[i][h] = j;
        len[j][h] = key_count[next_node[j][h]];
        len[i][h] = key_count[next_node[i][h]];
        return right_cnt;
    }
    int down_cnt = add_node(i, h - 1, j);
    if (h > level[j]) {
        ++len[i][h];
    } else {
        next_node[j][h] = next_node[i][h];
        next_node[i][h] = j;
        len[j][h] = len[i][h] + 1 - down_cnt - key_count[j];
        len[i][h] = down_cnt + key_count[j];
    }
    return right_cnt + down_cnt;
}

void add(int i, int h, int x) {
    if (find(1, MAXL, x) != 0) {
        add_count(1, MAXL, x);
    } else {
        key[++cnt] = x;
        key_count[cnt] = 1;
        level[cnt] = random_level();
        add_node(1, MAXL, cnt);
    }
}

// 减少次数
void remove_count(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        key_count[next_node[i][h]]--;
    } else {
        remove_count(i, h - 1, num);
    }
    len[i][h]--;
}

// 移除节点
void remove_node(int i, int h, int j) {
    if (h < 1) {
        return;
    }
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        i = next_node[i][h];
    }
    if (h > level[j]) {
        len[i][h]--;
    } else {
        next_node[i][h] = next_node[j][h];
        len[i][h] += len[j][h] - 1;
    }
    remove_node(i, h - 1, j);
}

void remove(int num) {
    int j = find(1, MAXL, num);
    if (j != 0) {
        if (key_count[j] > 1) {
            remove_count(1, MAXL, num);
        } else {
            remove_node(1, MAXL, j);
        }
    }
}

// 查找排名：有几个数比 x 小
int get_rank(int i, int h, int num) {
    int rightCnt = 0;
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        rightCnt += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        return rightCnt;
    } else {
        return rightCnt + get_rank(i, h - 1, num);
    }
}

int get_rank(int num) {
    return get_rank(1, MAXL, num) + 1;
}

// 查询第 k 大的数字（需要保证存在）
int index(int i, int h, int x) {
    int c = 0;
    while (next_node[i][h] != 0 && c + len[i][h] < x) {
        c += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        return key[next_node[i][h]];
    } else {
        return index(i, h - 1, x - c);
    }
}

int index(int x) {
    return index(1, MAXL, x);
}

// 查找 x 的前驱（小于 x 中最大的数），相当于 prev(lower_bound(x))
int prefix(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        return i == 1 ? INT_MIN : key[i];
    } else {
        return prefix(i, h - 1, num);
    }
}

int prefix(int num) {
    return prefix(1, MAXL, num);
}

// 查找 x 的后继（大于 x 中最小的数），相当于 upper_bound(x)
int suffix(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        if (next_node[i][h] == 0) {
            return INT_MAX;
        }
        if (key[next_node[i][h]] > num) {
            return key[next_node[i][h]];
        }
        i = next_node[i][h];
        if (next_node[i][h] == 0) {
            return INT_MAX;
        } else {
            return key[next_node[i][h]];
        }
    } else {
        return suffix(i, h - 1, num);
    }
}

int suffix(int num) {
    return suffix(1, MAXL, num);
}