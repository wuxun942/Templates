#include <bits/stdc++.h>
using namespace std;

// Prim 算法，需要指定起始点；返回生成树权值和
long long prim(int n, const vector<vector<int>>& edges, int source) {
    vector<vector<pair<int, int>>> g(n);
    for (auto& e: edges) {
        g[e[0]].emplace_back(e[1], e[2]);
        g[e[1]].emplace_back(e[0], e[2]);
    }
    vector vis(n, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> h;
    for (auto& [x, w]: g[source]) {
        h.emplace(w, x);
    }
    long long s = 0;
    for (int i = 1; i < n; ++i) {
        while (vis[h.top().second]) {
            h.pop();
        }
        auto [x, w] = h.top();
        h.pop();
        s += w;
        vis[x] = true;
    }
    return s;
}