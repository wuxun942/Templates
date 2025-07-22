#include <bits/stdc++.h>
using namespace std;

// Prim 算法，需要指定起始点；返回生成树权值和
template<typename T>
using min_heap = priority_queue<T, vector<T>, greater<T>>;
long long prim(int n, const vector<vector<int>>& edges, int source) {
    vector<vector<pair<int, int>>> g(n);
    for (auto& e: edges) {
        g[e[0]].emplace_back(e[1], e[2]);
        g[e[1]].emplace_back(e[0], e[2]);
    }
    vector vis(n, false);
    min_heap<pair<int, int>> h;
    for (auto& [x, w]: g[source]) {
        h.emplace(w, x);
    }
    long long res = 0;
    int cnt = 1;
    while (cnt < n) {
        auto [w, x] = h.top();
        h.pop();
        if (vis[x]) {
            continue;
        }
        ++cnt;
        res += w;
        vis[x] = true;
        for (auto& [y, w]: g[x]) {
            if (!vis[y]) {
                h.emplace(w, y);
            }
        }
    }
    return res;
}