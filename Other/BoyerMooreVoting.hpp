#include <bits/stdc++.h>
using namespace std;

/*
摩尔投票算法：每次把两个不同数字消除，如果存在绝对众数（出现次数大于 n/2），必然会留下来。
显然哈希表计数也能完成，不过本算法可以实现空间 O(1)
*/
int voting(vector<int> &nums) {
    int candidate, cnt = 0;
    for (int x : nums) {
        if (cnt == 0) {
            candidate = x;
            cnt = 1;
        } else if (x != candidate) {
            --cnt;
        } else {
            ++cnt;
        }
    }
    // 检查是否是绝对众数
    cnt = 0;
    for (int x : nums) {
        cnt += x == candidate;
    }
    return cnt > nums.size() / 2 ? candidate : -1;
}