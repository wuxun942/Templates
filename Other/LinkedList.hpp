// 数组模拟链表，常用于链式前向星

// 链式前向星建图，以下为 1-based
constexpr int MAXN = 5001, MAXE = MAXN << 1;
int head[MAXN];
int nxt[MAXE];
int to[MAXE];
// long long weight[MAXE];
int cnt_edge = 0;
void insert(int x, int y, long long w = 0) {
    nxt[++cnt_edge] = head[x];
    to[cnt_edge] = y;
    // weight[cnt_edge] = w;
    head[x] = cnt_edge;
}