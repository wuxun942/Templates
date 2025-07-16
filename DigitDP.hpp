#include <bits/stdc++.h>
using namespace std;

// 数位DP v2.1
// 代码示例：返回 [low, high] 中的恰好包含 target 个 0 的数字个数
int digitDP(int low, int high, int target) {
    string low_s = to_string(low);
    string high_s = to_string(high);
    int n = high_s.size();
    int diff_lh = n - low_s.size();
    vector memo(n, vector(target + 1, -1));
    auto dfs = [&](this auto&& dfs, int i, int cnt0, bool limit_low, bool limit_high) -> int {
        if (cnt0 > target) {
            return 0;
        }
        if (i == n) {
            return cnt0 == target;
        }
        if (!limit_low && !limit_high && memo[i][cnt0] >= 0) {
            return memo[i][cnt0];
        }
        int lo = limit_low && i >= diff_lh ? low_s[i - diff_lh] - '0' : 0;
        int hi = limit_high ? high_s[i] - '0' : 9;
        int res = 0;
        int d = lo;
        if (limit_low && i < diff_lh) {
            res += dfs(i + 1, cnt0, true, false);
            d = 1;
        }
        for (; d <= hi; ++d) {
            res += dfs(i + 1, cnt0 + (d == 0), limit_low && d == lo, limit_high && d == hi);
        }
        if (!limit_low && !limit_high) {
            memo[i][cnt0] = res;
        }
        return res;
    };
    return dfs(0, 0, true, true);
}