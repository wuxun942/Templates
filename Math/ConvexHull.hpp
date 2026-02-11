#include <bits/stdc++.h>
using namespace std;

// Andrew 算法求凸包（删除所有共线点）
template<typename T>
array<T, 2> sub(array<T, 2> v1, array<T, 2> v2) {
    return {v1[0] - v2[0], v1[1] - v2[1]};
}

template<typename T, typename F = T>
F dot(array<T, 2> v1, array<T, 2> v2) {
    return (F) v1[0] * v2[0] + (F) v1[1] * v2[1];
}

template<typename T, typename F = __int128>
F det(array<T, 2> v1, array<T, 2> v2) {
    return (F) v1[0] * v2[1] - (F) v1[1] * v2[0];
}

vector<array<int, 2>> andrew(vector<vector<int>> &points) {
    int n = points.size();
    vector<array<int, 2>> st;
    st.push_back({points[0][0], points[0][1]});
    for (int i = 1; i < n; ++i) {
        int x = points[i][0], y = points[i][1];
        while (st.size() > 1 && det<int>(sub(st.back(), st[st.size() - 2]), sub({x, y}, st.back())) <= 0) {
            st.pop_back();
        }
        st.push_back({x, y});
    }
    for (int i = n - 2, m = st.size(); i >= 0; --i) {
        int x = points[i][0], y = points[i][1];
        while (st.size() > m && det<int>(sub(st.back(), st[st.size() - 2]), sub({x, y}, st.back())) <= 0) {
            st.pop_back();
        }
        st.push_back({x, y});
    }
    return st;
}