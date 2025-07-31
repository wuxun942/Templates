#include <bits/stdc++.h>
using namespace std;

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

    void remove(T x) {
        ++remove_cnt[x];
        --sz;
    }

    T top() {
        apply_remove();
        return pq.top();
    }

    void pop() {
        apply_remove();
        pq.pop();
    }

    void push(T x) {
        if (remove_cnt[x] > 0) {
            --remove_cnt[x];
        } else {
            pq.push(x);
        }
        ++sz;
    }
};

// 数组记录
template<typename T, typename comp = less<T>>
class LazyHeap {
    priority_queue<T, vector<T>, comp> pq;
    vector<int> remove_cnt;
    size_t sz = 0;

    void apply_remove() {
        while (!pq.empty() && remove_cnt[pq.top()] > 0) {
            --remove_cnt[pq.top()];
            pq.pop();
        }
    }

public:
    // 传入堆中数据的最大值
    LazyHeap(int mx) {
        remove_cnt.resize(mx + 1);
    }

    size_t size() {
        return sz;
    }

    void remove(T x) {
        ++remove_cnt[x];
        --sz;
    }

    T top() {
        apply_remove();
        return pq.top();
    }

    void pop() {
        apply_remove();
        pq.pop();
    }

    void push(T x) {
        if (remove_cnt[x] > 0) {
            --remove_cnt[x];
        } else {
            pq.push(x);
        }
        ++sz;
    }
};