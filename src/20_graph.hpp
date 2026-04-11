// @begin graph
// @dep structure
// @exports dijkstra bfs warshall_floyd topological_sort kruskal LCA
// bellman_ford scc MaxFlow EulerTour グラフ関連
namespace graph {
// ダイクストラ法
vector<ll> dijkstra(const vector<vector<pair<int, ll>>> &g, int s) {
  int n = g.size();
  vector<ll> dist(n, LINF);
  priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> que;
  dist[s] = 0;
  que.push({0, s});
  while (!que.empty()) {
    auto [d, v] = que.top();
    que.pop();
    if (dist[v] < d)
      continue;
    for (auto [u, cost] : g[v]) {
      if (dist[u] > dist[v] + cost) {
        dist[u] = dist[v] + cost;
        que.push({dist[u], u});
      }
    }
  }
  return dist;
}

// BFS (重みなしグラフの最短距離)
vector<int> bfs(const vector<vector<int>> &g, int s) {
  int n = g.size();
  vector<int> dist(n, -1);
  queue<int> que;
  dist[s] = 0;
  que.push(s);
  while (!que.empty()) {
    int v = que.front();
    que.pop();
    for (int u : g[v]) {
      if (dist[u] != -1)
        continue;
      dist[u] = dist[v] + 1;
      que.push(u);
    }
  }
  return dist;
}

// ワーシャルフロイド法 (全頂点間最短距離)
// dist[i][j] = i→j の距離 (辺がなければ LINF)
// 負閉路が存在する場合 dist[i][i] < 0 となる頂点がある
void warshall_floyd(vector<vector<ll>> &dist) {
  int n = dist.size();
  rep(k, n) rep(i, n) rep(j, n) {
    if (dist[i][k] == LINF || dist[k][j] == LINF)
      continue;
    chmin(dist[i][j], dist[i][k] + dist[k][j]);
  }
}

// トポロジカルソート (BFS, Kahn's algorithm)
// 返り値: トポロジカル順序の頂点列 (DAGでなければ空を返す)
vector<int> topological_sort(const vector<vector<int>> &g) {
  int n = g.size();
  vector<int> indeg(n, 0);
  for (int v = 0; v < n; v++)
    for (int u : g[v])
      indeg[u]++;
  queue<int> que;
  for (int i = 0; i < n; i++)
    if (indeg[i] == 0)
      que.push(i);
  vector<int> order;
  while (!que.empty()) {
    int v = que.front();
    que.pop();
    order.push_back(v);
    for (int u : g[v])
      if (--indeg[u] == 0)
        que.push(u);
  }
  if ((int)order.size() != n)
    return {}; // DAGでない
  return order;
}

// クラスカル法 (最小全域木)
// 辺: {コスト, {頂点u, 頂点v}}
// 返り値: {最小全域木のコスト, 使った辺のリスト}
pair<ll, vector<pair<ll, pii>>> kruskal(int n, vector<pair<ll, pii>> &edges) {
  sort(all(edges));
  structure::UnionFind uf(n);
  ll total = 0;
  vector<pair<ll, pii>> used;
  for (auto &[cost, uv] : edges) {
    auto [u, v] = uv;
    if (uf.unite(u, v)) {
      total += cost;
      used.push_back({cost, {u, v}});
    }
  }
  return {total, used};
}

// LCA (最小共通祖先) - ダブリング
struct LCA {
  int n, LOG;
  vector<vector<int>> parent;
  vector<int> depth;

  LCA(const vector<vector<int>> &g, int root = 0) : n(g.size()), LOG(1) {
    while ((1 << LOG) < n)
      LOG++;
    parent.assign(LOG, vector<int>(n, -1));
    depth.assign(n, 0);
    // BFSで深さと親を求める
    queue<int> que;
    depth[root] = 0;
    parent[0][root] = root;
    que.push(root);
    vector<bool> visited(n, false);
    visited[root] = true;
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (int u : g[v]) {
        if (visited[u])
          continue;
        visited[u] = true;
        depth[u] = depth[v] + 1;
        parent[0][u] = v;
        que.push(u);
      }
    }
    // ダブリングテーブル構築
    for (int k = 0; k + 1 < LOG; k++)
      for (int v = 0; v < n; v++)
        if (parent[k][v] != -1)
          parent[k + 1][v] = parent[k][parent[k][v]];
  }

  int lca(int u, int v) const {
    if (depth[u] < depth[v])
      swap(u, v);
    // 深さを揃える
    int diff = depth[u] - depth[v];
    for (int k = 0; k < LOG; k++)
      if ((diff >> k) & 1)
        u = parent[k][u];
    if (u == v)
      return u;
    // 同時に登る
    for (int k = LOG - 1; k >= 0; k--) {
      if (parent[k][u] != parent[k][v]) {
        u = parent[k][u];
        v = parent[k][v];
      }
    }
    return parent[0][u];
  }

  int dist(int u, int v) const {
    return depth[u] + depth[v] - 2 * depth[lca(u, v)];
  }
};
// ベルマンフォード法 (負辺対応最短路)
// 負閉路で到達可能な頂点の距離は -LINF になる
vector<ll> bellman_ford(int n, const vector<tuple<int, int, ll>> &edges,
                        int s) {
  vector<ll> dist(n, LINF);
  dist[s] = 0;
  for (int i = 0; i < n - 1; i++) {
    for (auto &[u, v, w] : edges) {
      if (dist[u] != LINF && dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
      }
    }
  }
  // 負閉路検出: もう一度 n-1 回回して更新があれば負閉路
  for (int i = 0; i < n - 1; i++) {
    for (auto &[u, v, w] : edges) {
      if (dist[u] != LINF && dist[u] + w < dist[v]) {
        dist[v] = -LINF;
      }
    }
  }
  return dist;
}

// 強連結成分分解 (SCC) - Kosaraju's algorithm
// 返り値: comp[v] = 頂点vが属するSCC番号 (トポロジカル順)
vector<int> scc(const vector<vector<int>> &g) {
  int n = g.size();
  vector<vector<int>> rg(n);
  for (int v = 0; v < n; v++)
    for (int u : g[v])
      rg[u].push_back(v);

  vector<int> order, comp(n, -1);
  vector<bool> visited(n, false);

  // 1回目のDFS: 帰りがけ順を記録
  auto dfs1 = [&](auto &self, int v) -> void {
    visited[v] = true;
    for (int u : g[v])
      if (!visited[u])
        self(self, u);
    order.push_back(v);
  };
  for (int i = 0; i < n; i++)
    if (!visited[i])
      dfs1(dfs1, i);

  // 2回目のDFS: 逆グラフ上で帰りがけ順の逆順に探索
  int cnt = 0;
  auto dfs2 = [&](auto &self, int v, int c) -> void {
    comp[v] = c;
    for (int u : rg[v])
      if (comp[u] == -1)
        self(self, u, c);
  };
  for (int i = n - 1; i >= 0; i--)
    if (comp[order[i]] == -1)
      dfs2(dfs2, order[i], cnt++);

  return comp;
}

// 最大流 (Dinic's algorithm)
struct MaxFlow {
  struct Edge {
    int to, rev;
    ll cap;
  };

  int n;
  vector<vector<Edge>> graph;
  vector<int> level, iter;

  MaxFlow(int n_) : n(n_), graph(n_), level(n_), iter(n_) {}

  void add_edge(int from, int to, ll cap) {
    graph[from].push_back({to, (int)graph[to].size(), cap});
    graph[to].push_back({from, (int)graph[from].size() - 1, 0});
  }

  bool bfs(int s, int t) {
    fill(all(level), -1);
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (auto &e : graph[v]) {
        if (e.cap > 0 && level[e.to] < 0) {
          level[e.to] = level[v] + 1;
          que.push(e.to);
        }
      }
    }
    return level[t] >= 0;
  }

  ll dfs(int v, int t, ll f) {
    if (v == t)
      return f;
    for (int &i = iter[v]; i < (int)graph[v].size(); i++) {
      Edge &e = graph[v][i];
      if (e.cap > 0 && level[v] < level[e.to]) {
        ll d = dfs(e.to, t, min(f, e.cap));
        if (d > 0) {
          e.cap -= d;
          graph[e.to][e.rev].cap += d;
          return d;
        }
      }
    }
    return 0;
  }

  ll max_flow(int s, int t) {
    ll flow = 0;
    while (bfs(s, t)) {
      fill(all(iter), 0);
      ll d;
      while ((d = dfs(s, t, LINF)) > 0)
        flow += d;
    }
    return flow;
  }
};

// オイラーツアー (部分木クエリ用)
struct EulerTour {
  vector<int> in, out;
  int timer;

  EulerTour(const vector<vector<int>> &g, int root = 0)
      : in(g.size()), out(g.size()), timer(0) {
    dfs(g, root, -1);
  }

  void dfs(const vector<vector<int>> &g, int v, int p) {
    in[v] = timer++;
    for (int u : g[v])
      if (u != p)
        dfs(g, u, v);
    out[v] = timer;
  }

  // 頂点vの部分木は [in[v], out[v]) に対応
  bool is_ancestor(int u, int v) const {
    return in[u] <= in[v] && out[v] <= out[u];
  }
};
} // namespace graph
// @end graph
