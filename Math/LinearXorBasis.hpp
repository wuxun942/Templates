#include <bits/stdc++.h>
using namespace std;

// 异或线性基，时间复杂度 O(n * logU)

// 普通消元
template<typename T>
class LinearXorBasisNormal {
    vector<T> basis;
    bool has_zero = false;

    LinearXorBasisNormal (int m) : basis(m) {}

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

constexpr int MAXB = 64;
using T = long long;
T basis[MAXB];
int bitlen;
bool has_zero;
void build(int m) {
    has_zero = false;
    bitlen = m;
}

void insert(T x) {
    for (int i = bitlen - 1; i >= 0; --i) {
        if (x >> i & 1) {
            if (basis[i] == 0) {
                basis[i] = x;
                break;
            }
            x ^= basis[i];
        }
    }
    if (x == 0) {
        has_zero = true;
    }
}

T max_xor() {
    T ans = 0;
    for (int i = bitlen - 1; i >= 0; --i) {
        ans = max(ans, ans ^ basis[i]);
    }
    return ans;
}

// 高斯消元
template<typename T>
class LinearXorBasisGauss {
    vector<T> basis;
    bool has_zero = false;

    LinearXorBasisGauss (const vector<T> &a) {
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
        // 把 0 放到最后
        for (int i = 0, j = 0; j < len; ++i) {
            while (basis[i] == 0) {
                ++i;
            }
            basis[j++] = basis[i];
        }
        basis.resize(len);
    }

    // 第 k 小的异或和
    T get_kth_xor(uint64_t k) {
        k -= has_zero;
        if (k > 1 << basis.size()) {
            return -1;
        }
        T ans = 0;
        for (; k; k &= k - 1) {
            ans ^= basis[__builtin_ctz(k)];
        }
        return ans;
    }
};

constexpr int MAXB = 64;
using T = long long;
T basis[MAXB];
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
        ans ^= basis[__builtin_ctz(k)];
    }
    return ans;
}