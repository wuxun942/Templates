#include <bits/stdc++.h>
using namespace std;

// 乘法快速幂
constexpr int MOD = 1'000'000'007;
long long qpow(long long x, long long n, int mod = MOD) {
    int ans = 1;
    x %= mod;
    for (; n > 0; n >>= 1, x = x * x % mod) {
        if (n & 1) {
            ans = ans * x % mod;
        }
    }
    return ans;
}

// 矩阵乘法
vector<vector<int>> multiply(vector<vector<int>> &mat1, vector<vector<int>> &mat2) {
    int m = mat1.size(), p = mat1[0].size(), n = mat2[0].size();
    if (mat2.size() != p) {
        throw runtime_error("Invalid Input");
    }
    vector ans(m, vector(n, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int &x = ans[i][j];
            for (int k = 0; k < p; ++k) {
                x += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return ans;
}

// 矩阵快速幂：只有方阵才有乘方
constexpr int MOD = 1'000'000'007;
constexpr int SIZE = 100;
using matrix = array<array<long long, SIZE>, SIZE>;
matrix multiply(matrix &mat1, matrix &mat2, int sz = SIZE, int mod = MOD) {
    matrix ans{};
    for (int i = 0; i < SIZE; ++i) {
        for (int k = 0; k < SIZE; ++k) {
            if (mat1[i][k] == 0) {
                continue;
            }
            for (int j = 0; j < SIZE; ++j) {
                ans[i][j] = (ans[i][j] + mat1[i][k] * mat2[k][j]) % mod;
            }
        }
    }
    return ans;
}

matrix qpow(matrix mat, int n, int sz = SIZE, int mod = MOD) {
    matrix ans{};
    for (int i = 0; i < SIZE; ++i) {
        ans[i][i] = 1;
    }
    for (; n > 0; n >>= 1, mat = multiply(mat, mat, sz, mod)) {
        if (n & 1) {
            ans = multiply(ans, mat, sz, mod);
        }
    }
    return ans;
}