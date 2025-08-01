#include <bits/stdc++.h>
using namespace std;

// 懒删除堆，需要注意这里的 pop 不同于 STL 中的 pop，是有返回值的。

// 哈希表记录
template<typename T, typename comp = less<T>>
class LazyHeap {
    priority_queue<T, vector<T>, comp> pq;
    unordered_map<T, int> remove_cnt;
    size_t sz = 0;

    void apply_remove() {
        while (!pq.empty() && remove_cnt[pq.top()] > 0) {
            --remove_cnt[pq.top()];
            pq.pop();
        }
    }

public:
    size_t size() {
        return sz;
    }

    bool empty() {
        return sz == 0;
    }

    void remove(T x) {
        ++remove_cnt[x];
        --sz;
    }

    T top() {
        apply_remove();
        return pq.top();
    }

    void push(T x) {
        if (remove_cnt[x] > 0) {
            --remove_cnt[x];
        } else {
            pq.push(x);
        }
        ++sz;
    }

    T pop() {
        apply_remove();
        T res = pq.top();
        pq.pop();
        --sz;
        return res;
    }

    T pushpop(T x) {
        if (remove_cnt[x] > 0) {
            --remove_cnt[x];
        } else {
            pq.push(x);
        }
        apply_remove();
        T res = pq.top();
        pq.pop();
        return res;
    }
};

// 数组记录
template<typename T, typename comp = less<T>>
class LazyHeap {
    priority_queue<T, vector<T>, comp> pq;
    vector<int> remove_cnt;
    size_t sz = 0;
    size_t offset;

    void apply_remove() {
        while (!pq.empty() && remove_cnt[pq.top()] > 0) {
            --remove_cnt[pq.top() - offset];
            pq.pop();
        }
    }

public:
    // 传入堆中数据的最大值和最小值
    LazyHeap(int mx, int mn) {
        remove_cnt.resize(mx - mn + 1);
        offset = mn;
    }

    size_t size() {
        return sz;
    }

    bool empty() {
        return sz == 0;
    }

    void remove(T x) {
        ++remove_cnt[x - offset];
        --sz;
    }

    T top() {
        apply_remove();
        return pq.top();
    }

    void push(T x) {
        if (remove_cnt[x - offset] > 0) {
            --remove_cnt[x - offset];
        } else {
            pq.push(x);
        }
        ++sz;
    }

    T pop() {
        apply_remove();
        T res = pq.top();
        pq.pop();
        --sz;
        return res;
    }

    T pushpop(T x) {
        if (remove_cnt[x - offset] > 0) {
            --remove_cnt[x - offset];
        } else {
            pq.push(x);
        }
        apply_remove();
        T res = pq.top();
        pq.pop();
        return res;
    }
};