#include <bits/stdc++.h>
using namespace std;

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

// 节点的父节点
int fa[MAXN];

// 节点的左右儿子
int ls[MAXN];
int rs[MAXN];

// 子树大小
int siz[MAXN];

// 整体初始化
void clear(int n = cnt) {
    head = cnt = 0;    
    fill(key, key + n + 1, 0);
    fill(fa, fa + n + 1, 0);
    fill(ls, ls + n + 1, 0);
    fill(rs, rs + n + 1, 0);
    fill(siz, siz + n + 1, 0);
}

// 更新节点信息
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
}

// 判断节点是左儿子还是右儿子
// 规定：头节点是左儿子
int lr(int i) {
    return rs[fa[i]] == i ? 1 : 0;
}

// 通过旋转让 i 上升一步
void rotate(int i) {
    int f = fa[i], g = fa[f], soni = lr(i), sonf = lr(f);
    if (soni == 1) { // i 是右儿子，得左旋
        rs[f] = ls[i]; // f 的右儿子设为 i 的左儿子
        if (rs[f] != 0) {
            fa[rs[f]] = f;
        }
        ls[i] = f;
    } else { // i 是左儿子，得右旋
        ls[f] = rs[i]; // f 的左儿子设为 i 的右儿子
        if (ls[f] != 0) {
            fa[ls[f]] = f;
        }
        rs[i] = f;
    }
    // 如果存在爷爷节点 g，需要额外设置 g 和 i 的关系
    if (g != 0) {
        if (sonf == 1) {
            rs[g] = i;
        } else {
            ls[g] = i;
        }
    }
    fa[f] = i;
    fa[i] = g;
    up(f);
    up(i);
}

// 提根操作：使 i 成为 goal 的儿子
// goal = 0 时，i 直接成为头节点
void splay(int i, int goal) {
    int f = fa[i], g = fa[f];
    while (f != goal) {
        // 爷爷不是目标节点，至少要上升两步
        if (g != goal) {
            if (lr(i) == lr(f)) { // 一字型 zig-zig，先调整 f，再调整 i
                rotate(f);
            } else { // 之字形 zig-zag，i 上升两步
                rotate(i);
            }
        }
        rotate(i);
        f = fa[i];
        g = fa[f];
    }
    if (goal == 0) {
        head = i;
    }
}

// 查找第 k 小节点的编号，如果不存在则返回 0
int find(int k) {
    int i = head;
    while (i != 0) {
        if (siz[ls[i]] + 1 == k) {
            return i;
        } else if (siz[ls[i]] >= k) {
            i = ls[i];
        } else {
            k -= siz[ls[i]] + 1;
            i = rs[i];
        }
    }
    return 0;
}

// 插入节点
void insert(T x) {
    key[++cnt] = x;
    siz[cnt] = 1;
    if (head == 0) { // 空树
        head = cnt;
    } else {
        // 当前节点 i
        int i = head;
        // i 的父亲
        int f = 0;
        // i 是 f 的哪个儿子
        int son = 0;
        while (i != 0) {
            f = i;
            if (key[i] <= x) {
                son = 1;
                i = rs[i];
            } else {
                son = 0;
                i = ls[i];
            }
        }
        if (son == 1) {
            rs[f] = cnt;
        } else {
            ls[f] = cnt;
        }
        fa[cnt] = f;
        // 插入后提根
        splay(cnt, 0);
    }
}

// 查找排名
int get_rank(T x) {
    int i = head, last = head;
    int ans = 0;
    while (i != 0) {
        last = i;
        if (key[i] >= x) {
            i = ls[i];
        } else {
            ans += siz[ls[i]] + 1;
            i = rs[i];
        }
    }
    splay(last, 0);
    return ans + 1;
}

// 查询第 k 小的数字
T index(int k) {
    int i = find(k);
    splay(i, 0);
    return key[i];
}

// 查找 x 的前驱
T pre(T x) {
    int i = head, last = head;
    T ans = -INF;
    while (i != 0) {
        last = i;
        if (key[i] >= x) {
            i = ls[i];
        } else {
            ans = max(ans, key[i]);
            i = rs[i];
        }
    }
    splay(last, 0);
    return ans;
}

// 查找 x 的后继
T post(T x) {
    int i = head, last = head;
    T ans = INF;
    while (i != 0) {
        last = i;
        if (key[i] <= x) {
            i = rs[i];
        } else {
            ans = min(ans, key[i]);
            i = ls[i];
        }
    }
    splay(last, 0);
    return ans;
}

// 删除节点
void remove(T x) {
    int k = get_rank(x);
    // 存在 key = x 的节点
    if (k != get_rank(x + 1)) {
        // 获得节点编号
        int i = find(k);
        // 先提根
        splay(i, 0);
        if (ls[i] == 0) {
            head = rs[i];
        } else if (rs[i] == 0) {
            head = ls[i];
        } else { // 既有左儿子又有右儿子
            // 查找第 k + 1 小的节点
            int j = find(k + 1);
            // 把 j 变成 i 的儿子，那么 j 一定没有左儿子
            splay(j, i);
            // i 的左儿子变成 j 的左儿子
            ls[j] = ls[i];
            fa[ls[j]] = j;
            up(j);
            // 头节点变为 j，i 不再被访问（相当于删除）
            head = j;
        }
        if (head != 0) {
            fa[head] = 0;
        }
    }
}