#include <bits/stdc++.h>
using namespace std;

// // 手写二叉堆（反向索引堆的基础）
// constexpr int MAXN = 100'001;
// int n;
// int64_t h[MAXN];

// // 以下的 up & down 以最小堆为例
// void up(int i) {
//     while (i > 1 && h[i] < h[i / 2]) { // 最小堆条件
//         swap(h[i], h[i / 2]);
//     }
// }

// void down(int i) {
//     while (i * 2 <= n) {
//         int t = i * 2;
//         if (t + 1 <= n && h[t + 1] < h[t]) { // 最小堆条件
//             ++t;
//         }
//         if (h[t] >= h[i]) { // 最小堆条件
//             break;
//         }
//         swap(h[t], h[i]);
//         i = t;
//     }
// }

// void heapify(int sz, const int64_t *a) {
//     n = sz;
//     for (int i = 0; i < n; ++i) {
//         h[i + 1] = a[i];
//     }
//     for (int i = n; i; --i) {
//         down(i);
//     }
// }

// void push(const int64_t &x) {
//     h[++n] = x;
//     up(n);
// }

// int64_t top() {
//     return h[1];
// }

// void pop() {
//     swap(h[1], h[n]);
//     --n;
//     down(1);
// }

/*
反向索引堆：每个索引只能有一个值，索引存在且值可能发生变化时，直接改动索引
可用于 Prim 算法和 Dijkstra 算法，替代懒删除堆
*/
constexpr int MAXN = 100'001;
int n;
typedef pair<int64_t, int> pli;
pli h[MAXN];
int where[MAXN];

void heapswap(pli &x1, pli &x2) {
    swap(where[x1.second], where[x2.second]);
    swap(x1, x2);
}

void up(int i) {
    while (i > 1 && h[i] < h[i / 2]) { // 最小堆条件
        heapswap(h[i], h[i / 2]);
    }
}

void down(int i) {
    while (i * 2 <= n) {
        int t = i * 2;
        if (t + 1 <= n && h[t + 1] < h[t]) { // 最小堆条件
            ++t;
        }
        if (h[t] >= h[i]) { // 最小堆条件
            break;
        }
        heapswap(h[t], h[i]);
        i = t;
    }
}

void build(int n) {
    fill(where, where + n, -1); // 填充初始值，表示该位置还没有初始化
}

// 当索引不存在时，入堆；当新值更小时，原地修改
void push(const pli &p) {
    auto [x, idx] = p;
    int &i = where[idx];
    if (i == -1) {
        i = ++n;
        h[i] = p;
        up(i);
    } else if (h[i].first > x) {
        h[i].first = x;
        down(i);
    }
}

pli top() {
    return h[1];
}

void pop() {
    heapswap(h[1], h[n]);
    where[h[n].second] = -2; // 出堆标记，并且以后不再入堆
    --n;
    down(1);
}