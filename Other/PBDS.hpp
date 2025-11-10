/*
pb_ds: Policy-Based Data Structures
常用其中的 rb_tree_tag + tree_order_statistics_node_update，
可调用 find_by_order 和 order_of_key

当然也可以被其他平衡树 / 有序表模板代替
*/

#include <bits/stdc++.h>
using namespace std;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using __gnu_pbds::tree;
using __gnu_pbds::null_type;
using __gnu_pbds::rb_tree_tag;
using __gnu_pbds::tree_order_statistics_node_update;
template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template<typename T1, typename T2>
using ordered_map = tree<T1, T2, less<T1>, rb_tree_tag, tree_order_statistics_node_update>;