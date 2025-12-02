#include <bits/stdc++.h>
using namespace std;

// SOS(Sum of Subsets) DP / 高维前缀和
// 代码示例：把每个数看成集合，求出每个集合的子集个数
vector<int> sosdp(vector<int> &nums) {
    int m = bit_width<uint32_t>(ranges::max(nums));
    int u = 1 << m;
    vector f(u, 0);
    for (int x : nums) {
        ++f[x];
    }
    for (int i = 0; i < m; ++i) {
        for (int s = 0; s < u; ++s) {
            s |= 1 << i;
            f[s] += f[s ^ 1 << i];
        }
    }
    return f;
}