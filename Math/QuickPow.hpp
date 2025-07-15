// 乘法快速幂

// 不取模
long long qpow(unsigned long long x, int n) {
    long long ans = 1;
    for (; n > 0; n >>= 1, x *= x) {
        if (n & 1) {
            ans = ans * x;
        }
    }
    return ans;
}

// 取模
int qpow(long long x, long long n, int mod) {
    int ans = 1;
    x %= mod;
    for (; n > 0; n >>= 1, x = (x * x) % mod) {
        if (n & 1) {
            ans = ans * x % mod;
        }
    }
    return ans;
}