#include <bits/stdc++.h>
using namespace std;

class ST {
    vector<vector<int>> st;

    int merge_val(int x, int y) {
        return max(x, y);
    }
public:
    ST(const vector<int>& a) {
        int n = a.size(), m = bit_width((unsigned) n);
        st.resize(n, vector(m, 0));
        for (int i = 0; i < n - 1; ++i) {
            st[i][0] = a[i];
        }
        for (int j = 1; j < m; ++j) {
            for (int i = 0; i + (1 << j) <= n; ++i) { // 区间右端点不越界
                merge_val(st[i][j - 1], st[i + (1 << j - 1)][j - 1]);
            }
        }
    }

    // 闭区间查询
    int query(int l, int r) {
        if (l > r) {
            return 0;
        }
        int k = bit_width((unsigned) r - l + 1) - 1;
        return merge_val(st[l][k], st[r - (1 << k) + 1][k]);
    }
};