#include <bits/stdc++.h>
using namespace std;

/*
Treap：一种弱平衡树，通过随机化权重实现的笛卡尔树
以下保留有选 Treap 和 FHQ Treap 的实现
前者通过左旋和右旋维持平衡
后者通过树的分裂和合并实现可持久化，是做题中更常用的

由于用了rand，需要设置随机种子 srand(time(0))
*/

// 有旋 Treap

using T = long long;
constexpr T INF = LLONG_MAX;

constexpr int MAX_N = 100'000 + 5;

// 空间使用计数
int cnt = 0;

// 根节点
int root = 0;

// 节点的 key
T key[MAX_N];

// 节点 key 的计数
int key_count[MAX_N];

// 左右儿子
int ls[MAX_N];
int rs[MAX_N];

// 子树的数字总数
int siz[MAX_N];

// 随机生成的权重
int priority[MAX_N];

// 整体初始化
void clear(int n = cnt) {
    cnt = root = 0;
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
    root = insert(root, x);
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
    return small(root, x) + 1;
}

// 查询第 k 小的数字
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
    if (k > siz[root] || k <= 0) {
        throw overflow_error("Treap Overflow");
    }
    return index(root, k);
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
    return pre(root, x);
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
    return post(root, x);
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
        root = remove(root, x);
    }
}

/*
FHQ Treap：将 Treap 的旋转操作换成分裂和合并，可实现可持久化
分裂操作可分为按值分裂和按排名分裂两类，两者的复杂度都是 O(log n)
两者的使用场景有所差别，关心插入值则用前者，关心插入位置则用后者
*/

// 有 key_count 版本

using T = long long;
constexpr T INF = LLONG_MAX;
constexpr int MAX_N = 100'000 + 5;

// 空间使用计数
int cnt = 0;

// 根节点
int root = 0;

// 节点的 key
T key[MAX_N];

// 节点 key 的计数
int key_count[MAX_N];

// 左右儿子
int ls[MAX_N];
int rs[MAX_N];

// 子树的数字总数
int siz[MAX_N];

// 随机生成的权重
int priority[MAX_N];

// 整体初始化
void clear(int n = cnt) {
    cnt = root = 0;
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
void split_key(int l, int r, int i, T x) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    }
    if (key[i] <= x) { // key 太小，放在左子树的右儿子
        rs[l] = i;
        split_key(i, r, rs[i], x);
    } else { // key 太大，放在右子树的左儿子
        ls[r] = i;
        split_key(l, i, ls[i], x);
    }
    up(i);
}

// // 按排名分裂
void split_rank(int l, int r, int i, int rank) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    }
    if (siz[ls[i]] + 1 <= rank) {
        rs[l] = i;
        split_rank(i, r, rs[i], rank - siz[ls[i]] - 1);
    } else {
        ls[r] = i;
        split_rank(l, i, ls[i], rank);
    }
    up(i);
}

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
    if (find(root, x) != 0) {
        change_count(root, x, 1);
        return;
    }
    split_key(0, 0, root, x);
    key[++cnt] = x;
    key_count[cnt] = siz[cnt] = 1;
    priority[cnt] = rand();
    root = merge(merge(rs[0], cnt), ls[0]);
}

// 删除节点
void remove(T x) {
    int i = find(root, x);
    if (i != 0) {
        if (key_count[i] > 1) {
            change_count(root, x, -1);
        } else {
            split_key(0, 0, root, x);
            int lm = rs[0];
            int r = ls[0];
            split_key(0, 0, lm, x - 1);
            int l = rs[0];
            root = merge(l, r);
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
    return small(root, x) + 1;
}

// 查询第 k 小的数字（超过 size 则抛出异常）
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
    if (k > siz[root] || k <= 0) {
        throw overflow_error("FHQ Treap Overflow");
    }
    return index(root, k);
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
   return pre(root, x);
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
    return post(root, x);
}

// 无 key_count 版本

using T = long long;
constexpr T INF = LLONG_MAX;
constexpr int MAX_N = 100'000 + 5;

// 空间使用计数
int cnt = 0;

// 根节点
int root = 0;

// 节点的 key
T key[MAX_N];

// 左右儿子
int ls[MAX_N];
int rs[MAX_N];

// 子树的数字总数
int siz[MAX_N];

// 随机生成的权重
int priority[MAX_N];

// 整体初始化
void clear(int n = cnt) {
    cnt = root = 0;
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

// 按值分裂，严格大于 x 的节点放在右树
void split_key(int l, int r, int i, T x) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    }
    if (key[i] <= x) { // key 太小，放在左子树的右儿子
        rs[l] = i;
        split_key(i, r, rs[i], x);
    } else { // key 太大，放在右子树的左儿子，
        ls[r] = i;
        split_key(l, i, ls[i], x);
    }
    up(i);
}

// 按排名分裂
void split_rank(int l, int r, int i, int rank) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    }
    if (siz[ls[i]] + 1 <= rank) {
        rs[l] = i;
        split_rank(i, r, rs[i], rank - siz[ls[i]] - 1);
    } else {
        ls[r] = i;
        split_rank(l, i, ls[i], rank);
    }
    up(i);
}

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
    split_key(0, 0, root, x);
    key[++cnt] = x;
    siz[cnt] = 1;
    priority[cnt] = rand();
    root = merge(merge(rs[0], cnt), ls[0]);
}

// 删除节点：先按 x 分裂，再按 x - 1 分裂，删除第二次分裂的根节点
void remove(T x) {
    split_key(0, 0, root, x);
    int lm = rs[0];
    int r = ls[0];
    split_key(0, 0, lm, x - 1);
    int l = rs[0];
    int m = ls[0];
    root = merge(merge(l, merge(ls[m], rs[m])), r);
}

// 查找排名：返回 x 的名次（从 1 开始）
int get_rank(T x) {
    // 按 x - 1 分裂，小于等于 x - 1 的树的大小加 1 即为排名
    split_key(0, 0, root, x - 1);
    int ans = siz[rs[0]] + 1;
    root = merge(rs[0], ls[0]);
    return ans;
}

// 查询第 k 小的数字（超过 size 则抛出异常）
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
    if (k > siz[root] || k <= 0) {
        throw overflow_error("FHQ Treap Overflow");
    }
    return index(root, k);
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
   return pre(root, x);
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
    return post(root, x);
}

// 可持久化 FHQ Treap（只保留无 key_count 的版本）

using T = long long;
constexpr T INF = LLONG_MAX;

// 每个版本的最大节点数
constexpr int MAX_N = 100'000 + 5;

// 节点总数 = 每个版本的最大节点数 * 倍数
// 通常来说，这个数至少是树高的 5 倍；实际上这个数往往会更小
constexpr int MAXM = MAX_N * 85;

// 节点编号
int cnt = 0;

// 不同版本的根节点
int roots[MAX_N];

// 节点的 key
T key[MAXM];

// 左右儿子
int ls[MAXM];
int rs[MAXM];

// 子树大小
int siz[MAXM];

// 随机生成的权重
int priority[MAXM];

// 拷贝节点，返回新节点编号
int copy(int i) {
    key[++cnt] = key[i];
    ls[cnt] = ls[i];
    rs[cnt] = rs[i];
    siz[cnt] = siz[i];
    priority[cnt] = priority[i];
    return cnt;
}

// 维护子树信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
}

// 按值分裂
void split(int l, int r, int i, T x) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
        return;
    } 
    i = copy(i);
    if (key[i] <= x) {
        rs[l] = i;
        split(i, r, rs[i], x);
    } else {
        ls[r] = i;
        split(l, i, ls[i], x);
    }
    up(i);
}

// 合并
int merge(int l, int r) {
    if (l == 0 || r == 0) {
        return l + r;
    }
    if (priority[l] >= priority[r]) {
        l = copy(l);
        rs[l] = merge(rs[l], r);
        up(l);
        return l;
    }
    r = copy(r);
    ls[r] = merge(l, ls[r]);
    up(r);
    return r;
}

// 以 i 为根节点的树上插入 x，作为 v 版本
void insert(int v, int i, T x) {
    split(0, 0, i, x);
    int l = rs[0];
    int r = ls[0];
    ls[0] = rs[0] = 0;
    ++cnt;
    key[cnt] = x;
    siz[cnt] = 1;
    priority[cnt] = rand();
    roots[v] = merge(merge(l, cnt), r);
}

// 以 i 为根节点的树上删除 x（保证存在），作为 v 版本
void remove(int v, int i, T x) {
    split(0, 0, i, x);
    int lm = rs[0];
    int r = ls[0];
    split(0, 0, lm, x - 1);
    int l = rs[0];
    int m = ls[0];
    ls[0] = rs[0] = 0;
    roots[v] = merge(merge(l, merge(ls[m], rs[m])), r);
}

// 比 x 小的数字数量；如需查询排名，需要在此基础上加 1
int small(int i, T x) {
    if (i == 0) {
        return 0;
    }
    if (key[i] >= x) {
        return small(ls[i], x);
    }
    return siz[ls[i]] + 1 + small(rs[i], x);
}

// 查询第 k 小的数字
T index(int i, int k) {
    if (siz[ls[i]] >= k) {
        return index(ls[i], k);
    }
     if (siz[ls[i]] + 1 < k) {
        return index(rs[i], k - siz[ls[i]] - 1);
    }
    return key[i];
}

// 查找 x 的前驱
T pre(int i, T x) {
    if (i == 0) {
        return -INF;
    }
    if (key[i] >= x) {
        return pre(ls[i], x);
    }
    return max(key[i], pre(rs[i], x));
}

// 查找 x 的后继
T post(int i, T x) {
    if (i == 0) {
        return INF;
    }
    if (key[i] <= x) {
        return post(rs[i], x);
    }
    return min(key[i], post(ls[i], x));
}