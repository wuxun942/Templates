#include <bits/stdc++.h>
using namespace std;

// Floyd 算法
vector<vector<long long>> floyd(int n, const vector<vector<int>>& edges) {
    vector f(n, vector(n, LLONG_MAX / 2));
    for (auto& e: edges) {
        int i = e[0], j = e[1], w = e[2];
        f[i][j] = w;
        f[j][i] = w;
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (f[i][k] == LLONG_MAX / 2) {
                continue;
            }
            for (int j = 0; j < n; ++j) {
                f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
            }
        }
    }
    return f;
}