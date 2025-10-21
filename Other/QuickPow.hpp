#include <bits/stdc++.h>
using namespace std;

// 乘法快速幂

// 不取模
long long qpow(long long x, long long n) {
    long long ans = 1;
    for (; n > 0; n >>= 1, x *= x) {
        if (n & 1) {
            ans *= x;
        }
    }
    return ans;
}

// 取模
long long qpow(long long x, long long n, int mod) {
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
    vector res(m, vector(n, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int &x = res[i][j];
            for (int k = 0; k < p; ++k) {
                x += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return res;
}

// 矩阵快速幂：只有方阵才有乘方

// 不取模，原地写法
constexpr int SIZE = 100;
using matrix = array<array<int, SIZE>, SIZE>;
void multiply(matrix &mat1, matrix &mat2, matrix &res, int sz) {
    for (auto &row : res) {
        row.fill(0);
    }
    for (int i = 0; i < sz; ++i) {
        for (int k = 0; k < sz; ++k) {
            if (mat1[i][k] == 0) {
                continue;
            }
            for (int j = 0; j < sz; ++j) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

matrix qpow(matrix mat, int n, int sz) {
    matrix ans{}, tmp;
    for (int i = 0; i < sz; ++i) {
        ans[i][i] = 1;
    }
    for (; n > 0; n >>= 1, multiply(mat, mat, tmp, sz), mat = tmp) {
        if (n & 1) {
            multiply(ans, mat, tmp, sz);
            ans = tmp;
        }
    }
    return ans;
}

// 不取模，非原地写法
constexpr int SIZE = 100;
using matrix = array<array<int, SIZE>, SIZE>;
matrix multiply(matrix& mat1, matrix& mat2, int sz) {
    matrix res{};
    for (int i = 0; i < sz; ++i) {
        for (int k = 0; k < sz; ++k) {
            if (mat1[i][k] == 0) {
                continue;
            }
            for (int j = 0; j < SIZE; ++j) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return res;
}

matrix qpow(matrix mat, int n, int sz) {
    matrix ans{};
    for (int i = 0; i < sz; ++i) {
        ans[i][i] = 1;
    }
    for (; n > 0; n >>= 1, mat = multiply(mat, mat, sz)) {
        if (n & 1) {
            ans = multiply(ans, mat, sz);
        }
    }
    return ans;
}

// 取模，原地写法
constexpr int SIZE = 100;
using matrix = array<array<int, SIZE>, SIZE>;
void multiply(matrix &mat1, matrix &mat2, int sz, int mod, matrix &res) {
    for (int i = 0; i < sz; ++i) {
        for (int k = 0; k < sz; ++k) {
            if (mat1[i][k] == 0) {
                continue;
            }
            for (int j = 0; j < sz; ++j) {
                res[i][j] = (res[i][j] + 1LL * mat1[i][k] * mat2[k][j]) % mod;
            }
        }
    }
}

matrix qpow(matrix mat, int n, int sz, int mod) {
    matrix ans{}, tmp;
    for (int i = 0; i < SIZE; ++i) {
        ans[i][i] = 1;
    }
    for (; n > 0; n >>= 1, multiply(mat, mat, sz, mod, tmp), mat = tmp) {
        if (n & 1) {
            multiply(ans, mat, sz, mod, tmp);
            ans = tmp;
        }
    }
    return ans;
}

// 取模，非原地写法
constexpr int SIZE = 100;
using matrix = array<array<int, SIZE>, SIZE>;
matrix multiply(matrix& mat1, matrix& mat2, int sz, int mod) {
    matrix res{};
    for (int i = 0; i < SIZE; ++i) {
        for (int k = 0; k < SIZE; ++k) {
            if (mat1[i][k] == 0) {
                continue;
            }
            for (int j = 0; j < SIZE; ++j) {
                res[i][j] = (res[i][j] + 1LL * mat1[i][k] * mat2[k][j]) % mod;
            }
        }
    }
    return res;
}

matrix qpow(matrix mat, int n, int sz, int mod) {
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