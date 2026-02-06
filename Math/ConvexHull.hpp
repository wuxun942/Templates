#include <bits/stdc++.h>
using namespace std;

// Andrew 算法求凸包（删除所有共线点）
template<typename T>
T dot(T x1, T y1, T x2, T y2, T x3, T y3) {
    return (x2 - x1) * (x3 - x2) + (y2 - y1) * (y3 - y2);
}

template<typename T>
T det(T x1, T y1, T x2, T y2, T x3, T y3) {
    return (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
}

vector<array<int, 2>> andrew(vector<vector<int>> &points) {
    int n = points.size();
    vector<array<int, 2>> st;
    st.push_back({points[0][0], points[0][1]});
    for (int i = 1; i < n; ++i) {
        int x = points[i][0], y = points[i][1];
        while (st.size() > 1 && det<int>(st[st.size() - 2][0], st[st.size() - 2][1], st.back()[0], st.back()[1], x, y) <= 0) {
            st.pop_back();
        }
        st.push_back({x, y});
    }
    for (int i = n - 2, m = st.size(); i >= 0; --i) {
        int x = points[i][0], y = points[i][1];
        while (st.size() > m && det<int>(st[st.size() - 2][0], st[st.size() - 2][1], st.back()[0], st.back()[1], x, y) <= 0) {
            st.pop_back();
        }
        st.push_back({x, y});
    }
    return st;
}