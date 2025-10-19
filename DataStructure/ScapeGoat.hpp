#include <bits/stdc++.h>
using namespace std;

/*
替罪羊树：保证每个子树的大小均小于 alpha * n
1. 如果插入后仍然满足，则不更改根节点
2. 如果插入后不满足，则按中序遍历重构

一般取 alpha = 0.7
*/