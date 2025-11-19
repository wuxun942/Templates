#include <bits/stdc++.h>
using namespace std;

/*
懒删除堆：采用延迟删除，只有等到被删除元素到达堆顶时再删除，否则只记录待删除信息
需要注意这里的 pop 不同于 STL 中的 pop，是有返回值的
以维护堆中元素和为例
*/ 

template<typename T, typename comp = less<T>, typename F = T>
class LazyHeap {
    priority_queue<T, vector<T>, comp> pq;
    unordered_map<T, int> remove_cnt;
    size_t sz = 0;
    F sum = 0;

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
        sum -= x;
        --sz;
    }

    T top() {
        apply_remove();
        return pq.top();
    }

    F get_sum() {
        return sum;
    }

    void push(T x) {
        if (remove_cnt[x] > 0) {
            --remove_cnt[x];
        } else {
            pq.push(x);
        }
        sum += x;
        ++sz;
    }

    T pop() {
        apply_remove();
        T res = pq.top();
        pq.pop();
        --sz;
        sum -= res;
        return res;
    }

    T pushpop(T x) {
        if (remove_cnt[x] > 0) {
            --remove_cnt[x];
        } else {
            pq.push(x);
        }
        sum += x;
        apply_remove();
        T res = pq.top();
        pq.pop();
        sum -= res;
        return res;
    }
};

/*
对顶堆，通常是懒删除堆；常用于动态维护前 k 大/小的数据
以前 k 大为例
前 k 小只需要把 greater 的位置对换，并且把 remove 的 if 不等号反向
*/

template<typename T, typename F = T>
struct DualHeaps {
    LazyHeap<T, greater<T>, F> h1; // 大一半用最小堆维护
    LazyHeap<T, less<T>, F> h2; // 小一半用最大堆维护
    int k;

    DualHeaps(int k) {
        this->k = k;
    }

    F get_sum1() {
        return h1.sum;
    }

    F get_sum2() {
        return h2.sum;
    }

    void insert(T x) {
        h1.push(x);
        if (h1.size() > k) {
            h2.push(h1.pop());
        }
    }

    void remove(T x) {
        if (x >= h1.top()) {
            h1.remove(x);
            if (h1.size() < k && !h2.empty()) {
                h1.push(h2.pop());
            }
        } else {
            h2.remove(x);
        }
    }
};

/*
更进阶地，可以把对顶堆改成平衡树，可以实现更多功能
以下给出对顶 set 维护前 k 大的实现
前 k 小只需要 comp = greater<>，并且把 remove 的 if 不等号反向
*/
template<typename T, typename comp = less<T>>
struct DualSets {
    set<T, comp> st1, st2;
    int k;

    DualSets(int k) {
        this->k = k;
    }

    void insert(T x) {
        st2.insert(x);
        if (st2.size() > k) {
            auto it = st2.begin();
            st1.insert(*it);
            st2.erase(it);
        }
    }

    void erase(T x) {
        if (x >= *st2.begin()) {
            st2.erase(x);
            if (st2.size() < k) {
                auto it = prev(st1.end());
                st2.insert(*it);
                st1.erase(it);
            }
        } else {
            st1.erase(x);
        }
    }
};