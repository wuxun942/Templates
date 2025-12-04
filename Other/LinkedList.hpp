// 数组模拟链表，常用于链式前向星

// 链式前向星建图，以下为 1-based
constexpr int MAX_N = 5000 + 5, MAX_E = MAX_N << 1;
int head[MAX_N];
int nxt[MAX_E];
int to[MAX_E];
// long long weight[MAX_E];
int cnt_edge = 0;
void insert(int x, int y, long long w = 0) {
    nxt[++cnt_edge] = head[x];
    to[cnt_edge] = y;
    // weight[cnt_edge] = w;
    head[x] = cnt_edge;
}