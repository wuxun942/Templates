#include <bits/stdc++.h>
using namespace std;

/*
扩展欧几里得算法：
1. 计算 gcd(a,b)
2. 计算 ax + by = gcd(a,b) 的一组解
*/
template<typename T>
T exgcd(T a, T b, T &x, T &y) {
  if (b == 0) {
      return a;
  }
  T g = exgcd(b, a % b, x, y);
  T tmp = x;
  x = y;
  y = tmp - a / b * y;
  return g;
}

// 迭代版本的扩展欧几里得算法
template<typename T>
T gcd(T a, T b, T &x, T &y) {
  x = 1, y = 0;
  T x1 = 0, y1 = 1, a1 = a, b1 = b;
  while (b1) {
    T q = a1 / b1;
    tie(x, x1) = make_tuple(x1, x - q * x1);
    tie(y, y1) = make_tuple(y1, y - q * y1);
    tie(a1, b1) = make_tuple(b1, a1 - q * b1);
  }
  return a1;
}

// 扩展欧几里得算法求逆元：要求 gcd(a,b) = 1
template<typename T>
T get_inv(T x, T mod) {
  T a, b;
  T g = exgcd(x, mod, a, b);
  if (g != 1) {
      throw runtime_error("Invalid Input");
  }
  return (a % mod + mod) % mod;
}