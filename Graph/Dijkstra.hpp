#include <bits/stdc++.h>
using namespace std;

// 堆优化 Dijkstra，无向图 + 从 0 开始编号
vector<long long> dijkstra(int n, const vector<vector<int>>& edges, int source) {
    vector<vector<pair<int, int>>> g(n);
    for (auto& e: edges) {
        g[e[0]].emplace_back(e[1], e[2]);
        g[e[1]].emplace_back(e[0], e[2]); // 有向图得把本行删掉
    }
    vector dis(n, LLONG_MAX);
    dis[source] = 0;
    using T = pair<long long, int>;
    priority_queue<T, vector<T>, greater<T>> pq;
    pq.emplace(0, source);
    while (!pq.empty()) {
        auto [dx, x] = pq.top();
        pq.pop();
        if (dx > dis[x]) {
            continue;
        }
        for (auto& [y, w]: g[x]) {
            if (int new_d = dx + w; new_d < dis[y]) {
                dis[y] = new_d;
                pq.emplace(new_d, y);
            }
        }
    }
    return dis;
}