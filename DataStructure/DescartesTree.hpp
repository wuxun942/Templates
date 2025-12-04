#include <bits/stdc++.h>
using namespace std;

/*
笛卡尔树：key 按二叉搜索树组织，value 按堆组织（不是二叉堆，但满足根节点最大/最小）
以数组（key 从 1-n） + 最小堆为例
如果是其他键，需要离散化；最大堆只需要将 while 不等号反向
*/ 

constexpr int MAX_N = 100'000 + 5;
int n;
int arr[MAX_N]; // 键排好序后，对应的值
int top;
int st[MAX_N];
int head;
int ls[MAX_N], rs[MAX_N];

// 以下的 arr 和 st 均为 1-based
void build(int n, const int *arr) {
    top = 0;
    fill(ls + 1, ls + n + 1, 0);
    fill(rs + 1, rs + n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int pos = top;
        // 维护一个单增的栈（此处为严格单增）；大根堆则改为单减
        while (pos > 0 && arr[st[pos]] >= arr[i]) {
            --pos;
        }
        // 存在更小的值，就将 i 作为其右儿子
        if (pos > 0) {
            rs[st[pos]] = i;
        }
        // 存在更大的值，就将其作为 i 的左儿子
        if (pos < top) {
            ls[i] = st[pos + 1];
        }
        st[++pos] = i;
        top = pos;
    }
    head = st[1];
}