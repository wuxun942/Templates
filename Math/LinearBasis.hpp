#include <bits/stdc++.h>
using namespace std;

// 异或空间线性基，时间复杂度 O(n * logU)

/*
普通消元：可以在线，但只能求最大值
思路：按最高位构建
*/
template<typename T>
struct XorBasisNormal {
    vector<T> basis;
    bool has_zero = false;
    XorBasisNormal (int m) : basis(m) {}

    void insert(T x) {
        for (int i = m - 1; i >= 0; --i) {
            if (x >> i & 1) {
                if (basis[i] == 0) {
                    basis[i] = x;
                    break;
                }
                x ^= basis[i];
            }
        }
        has_zero = x == 0;
    }

    T max_xor() {
        T ans = 0;
        for (int i = basis.size() - 1; i >= 0; --i) {
            ans = max(ans, ans ^ basis[i]);
        }
        return ans;
    }
};

constexpr int MAX_B = 64;
using T = long long;
T basis[MAX_B];
int bitlen;
bool has_zero;
void build(int m) {
    has_zero = false;
    bitlen = m;
}

bool insert(T x) {
    for (int i = bitlen - 1; i >= 0; --i) {
        if (x >> i & 1) {
            if (basis[i] == 0) {
                basis[i] = x;
                return true;
            }
            x ^= basis[i];
        }
    }
    has_zero = true;
    return false;
}

T max_xor() {
    T ans = 0;
    for (int i = bitlen - 1; i >= 0; --i) {
        ans = max(ans, ans ^ basis[i]);
    }
    return ans;
}

/*
高斯消元：必须离线，可以求第 k 大
技巧同加法方程组的高斯消元
*/
template<typename T>
struct XorBasisGauss {
    vector<T> basis;
    bool has_zero = false;
    XorBasisGauss (const vector<T> &a) {
        basis = a;
        int bitlen = bit_width<uint64_t>(ranges::max(a)), n = a.size(), len = 0;
        for (int i = bitlen - 1; i >= 0; --i) {
            // 找第 i 位上是 1 的数字
            for (int j = len; j < n; ++j) {
                if (basis[j] >> i & 1) {
                    swap(basis[j], basis[len]);
                    break;
                }
                // 找到了！
                if (basis[len] >> i & 1) {
                    for (int j = 0; j < n; ++j) {
                        if (j != len && basis[j] >> i & 1) {
                            basis[j] ^= basis[len];
                        }
                    }
                    ++len;
                }
            }
        }
        has_zero = len != n;
        basis.resize(len);
        // 把 0 放到最后 + 翻转
        for (int i = 0, j = 0; j < len; ++i) {
            while (basis[i] == 0) {
                ++i;
            }
            basis[j++] = basis[i];
        }
        for (int i = 0, j = len - 1; i < j; ++i, --j) {
            swap(basis[i], basis[j]);
        }
    }

    // 第 k 小的异或和
    T get_kth_xor(uint64_t k) {
        k -= has_zero;
        if (k > 1 << basis.size()) {
            return -1;
        }
        T ans = 0;
        for (; k; k &= k - 1) {
            ans ^= basis[__builtin_ctzll(k)];
        }
        return ans;
    }
};

constexpr int MAX_B = 64;
using T = long long;
T basis[MAX_B];
int b;
bool has_zero;

void build(T *a, int n) {
    b = n;
    for (int i = 0; i < n; ++i) {
        basis[i] = a[i];
    }
    int bitlen = __lg(*max_element(basis, basis + b)) + 1, len = 0;
    for (int i = bitlen - 1; i >= 0; --i) {
        // 找第 i 位上是 1 的数字
        for (int j = len; j < b; ++j) {
            if (basis[j] >> i & 1) {
                swap(basis[j], basis[len]);
                break;
            }
        }
        // 找到了！
        if (basis[len] >> i & 1) {
            for (int j = 0; j < b; ++j) {
                if (j != len && basis[j] >> i & 1) {
                    basis[j] ^= basis[len];
                }
            }
            ++len;
        }
    }
    has_zero = len != b;
    b = len;
    // 把 0 放到最后 + 翻转
    for (int i = 0, j = 0; j < b; ++i) {
        while (basis[i] == 0) {
            ++i;
        }
        basis[j++] = basis[i];
    }
    for (int i = 0, j = b - 1; i < j; ++i, --j) {
        swap(basis[i], basis[j]);
    }
}

// 第 k 小的异或和
T get_kth_xor(uint64_t k) {
    k -= has_zero;
    if (k > 1 << b) {
        return -1; // 超出范围
    }
    T ans = 0;
    for (; k; k &= k - 1) {
        ans ^= basis[__builtin_ctzll(k)];
    }
    return ans;
}

// 向量空间线性基：等效于加法方程组高斯消元
constexpr int MAX_N = 100 + 5, MAX_M = 100 + 5;
constexpr double eps = 1e5;
double mat[MAX_N][MAX_M];
int basis[MAX_N];
int n, m, cnt; // 行数, 列数, 基数量

bool insert(int i) {
    for (int j = 0; j < m; ++j) {
        if (abs(mat[i][j]) >= eps) {
            // 这个位置还没有基
            if (basis[j] == 0) {
                basis[j] = i;
                return true;
            }
            double r = mat[i][j] / mat[basis[j]][j];
            for (int k = j; k < m; ++k) {
                mat[i][k] -= r * mat[basis[j]][k];
            }
        }
    }
    return false;
}

// 假设已经对 mat 赋值了
void build() {
    memset(basis, 0, sizeof(basis));
    cnt = 0;
    for (int i = 0; i < n; ++i) {
        if (insert(i)) {
            ++cnt;
        }
    }
    // 最后得到 basis 数组，basis[j] 表示第一个非零数在第 j 列的基向量所在行
}