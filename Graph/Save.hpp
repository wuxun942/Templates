// 链式前向星建图，以下为 1-index

constexpr int MAXN = 5001, MAXM = 500'001;
int head[MAXN]{};
int nxt[MAXM];
int to[MAXM];
// int weight[MAXM];
int cnt_e = 0;
void add_e(int x, int y, int w = 0) {
    nxt[++cnt_e] = head[x];
    to[cnt_e] = y;
    // weight[cnt_e] = w;
    head[x] = cnt_e;
}