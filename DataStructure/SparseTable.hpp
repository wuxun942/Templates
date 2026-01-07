#include <bits/stdc++.h>
using namespace std;

// ST 表：处理可重复贡献的查询问题，只支持静态查询

// 泛型模板封装
template<typename T>
class ST {
    vector<vector<T>> st;

    T merge_val(T x, T y) {
        return max(x, y); // 看题目修改
    }
    
public:
    ST(const vector<int> &arr) {
        int n = arr.size(), m = bit_width<uint32_t>(n);
        st.resize(n, vector(m, 0)); // 看题目修改
        for (int i = 0; i < n; ++i) {
            st[i][0] = arr[i];
        }
        for (int j = 1; j < m; ++j) {
            for (int i = 0; i + (1 << j) <= n; ++i) { // 区间右端点不越界
                st[i][j] = merge_val(st[i][j - 1], st[i + (1 << j - 1)][j - 1]);
            }
        }
    }

    // 闭区间 [l, r] 查询
    T query(int l, int r) {
        if (l > r) {
            return 0;
        }
        int k = bit_width<uint32_t>(r - l + 1) - 1;
        return merge_val(st[l][k], st[r - (1 << k) + 1][k]);
    }
};

// 静态数组实现
constexpr int MAX_N = 100'000 + 5;
constexpr int MAX_M = 17;
using T = int;
int n, m;
T arr[MAX_N];
T st[MAX_N][MAX_M];

void init() {
    m = bit_width<uint32_t>(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            st[i][j] = 0; // 看题目修改
        }
    }
}

T merge_val(T x, T y) {
    return max(x, y); // 看题目修改
}

void build() {
    init();
    for (int i = 0; i < n - 1; ++i) {
        st[i][0] = arr[i];
    }
    for (int j = 1; j < m; ++j) {
        for (int i = 0; i + (1 << j) <= n; ++i) { // 区间右端点不越界
            st[i][j] = merge_val(st[i][j - 1], st[i + (1 << j - 1)][j - 1]);
        }
    }
}

// 闭区间 [l, r] 查询
T query(int l, int r) {
    if (l > r) {
        return 0;
    }
    int k = bit_width<uint32_t>(r - l + 1);
    return merge_val(st[l][k], st[r - (1 << k) + 1][k]);
}