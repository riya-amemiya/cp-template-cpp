#include "../main.cpp"
#include "test_helper.hpp"

TEST(dijkstra_basic) {
  // 0 --(1)--> 1 --(2)--> 2
  // 0 --(5)--> 2
  vector<vector<pair<int, ll>>> g(3);
  g[0].push_back({1, 1});
  g[0].push_back({2, 5});
  g[1].push_back({2, 2});
  auto dist = graph::dijkstra(g, 0);
  ASSERT_EQ(dist[0], 0LL);
  ASSERT_EQ(dist[1], 1LL);
  ASSERT_EQ(dist[2], 3LL);
}

TEST(bfs_basic) {
  // 0 -- 1 -- 2
  //      |
  //      3
  vector<vector<int>> g(4);
  g[0].push_back(1);
  g[1].push_back(0);
  g[1].push_back(2);
  g[2].push_back(1);
  g[1].push_back(3);
  g[3].push_back(1);
  auto dist = graph::bfs(g, 0);
  ASSERT_EQ(dist[0], 0);
  ASSERT_EQ(dist[1], 1);
  ASSERT_EQ(dist[2], 2);
  ASSERT_EQ(dist[3], 2);
}

TEST(warshall_floyd_basic) {
  int n = 3;
  vector<vector<ll>> dist(n, vector<ll>(n, LINF));
  for (int i = 0; i < n; i++)
    dist[i][i] = 0;
  dist[0][1] = 1;
  dist[1][2] = 2;
  dist[0][2] = 10;
  graph::warshall_floyd(dist);
  ASSERT_EQ(dist[0][2], 3LL);
  ASSERT_EQ(dist[0][1], 1LL);
  ASSERT_EQ(dist[1][2], 2LL);
}

TEST(topological_sort_basic) {
  // 0 -> 1 -> 2
  //      |
  //      v
  //      3
  vector<vector<int>> g(4);
  g[0].push_back(1);
  g[1].push_back(2);
  g[1].push_back(3);
  auto order = graph::topological_sort(g);
  ASSERT_EQ((int)order.size(), 4);
  // 0 must come before 1, 1 before 2, 1 before 3
  map<int, int> pos;
  for (int i = 0; i < (int)order.size(); i++)
    pos[order[i]] = i;
  ASSERT_TRUE(pos[0] < pos[1]);
  ASSERT_TRUE(pos[1] < pos[2]);
  ASSERT_TRUE(pos[1] < pos[3]);
}

TEST(topological_sort_cycle) {
  // 0 -> 1 -> 2 -> 0 (cycle)
  vector<vector<int>> g(3);
  g[0].push_back(1);
  g[1].push_back(2);
  g[2].push_back(0);
  auto order = graph::topological_sort(g);
  ASSERT_TRUE(order.empty());
}

TEST(kruskal_basic) {
  // Triangle: 0--1 (cost 1), 1--2 (cost 2), 0--2 (cost 3)
  vector<pair<ll, pii>> edges = {{1, {0, 1}}, {2, {1, 2}}, {3, {0, 2}}};
  auto [cost, used] = graph::kruskal(3, edges);
  ASSERT_EQ(cost, 3LL);
  ASSERT_EQ((int)used.size(), 2);
}

TEST(LCA_basic) {
  // Tree: 0-1, 0-2, 1-3, 1-4
  vector<vector<int>> g(5);
  g[0].push_back(1);
  g[1].push_back(0);
  g[0].push_back(2);
  g[2].push_back(0);
  g[1].push_back(3);
  g[3].push_back(1);
  g[1].push_back(4);
  g[4].push_back(1);

  graph::LCA lca(g, 0);
  ASSERT_EQ(lca.lca(3, 4), 1);
  ASSERT_EQ(lca.lca(3, 2), 0);
  ASSERT_EQ(lca.lca(1, 2), 0);
  ASSERT_EQ(lca.dist(3, 4), 2);
  ASSERT_EQ(lca.dist(3, 2), 3);
}

TEST(bellman_ford_basic) {
  // 0 --(1)--> 1 --(2)--> 2
  // 0 --(5)--> 2
  vector<tuple<int, int, ll>> edges = {{0, 1, 1}, {1, 2, 2}, {0, 2, 5}};
  auto dist = graph::bellman_ford(3, edges, 0);
  ASSERT_EQ(dist[0], 0LL);
  ASSERT_EQ(dist[1], 1LL);
  ASSERT_EQ(dist[2], 3LL);
}

TEST(bellman_ford_negative_cycle) {
  // 0 -> 1 (1), 1 -> 2 (-3), 2 -> 0 (1) : negative cycle
  // 0 -> 3 (10)
  vector<tuple<int, int, ll>> edges = {
      {0, 1, 1}, {1, 2, -3}, {2, 0, 1}, {0, 3, 10}};
  auto dist = graph::bellman_ford(4, edges, 0);
  ASSERT_EQ(dist[0], -LINF);
  ASSERT_EQ(dist[3], -LINF);
}

TEST(scc_basic) {
  // 0 -> 1 -> 2 -> 0 (SCC {0,1,2})
  // 2 -> 3 (SCC {3})
  vector<vector<int>> g(4);
  g[0].push_back(1);
  g[1].push_back(2);
  g[2].push_back(0);
  g[2].push_back(3);
  auto comp = graph::scc(g);
  // 0,1,2 should be in the same component
  ASSERT_EQ(comp[0], comp[1]);
  ASSERT_EQ(comp[1], comp[2]);
  // 3 should be in a different component
  ASSERT_TRUE(comp[3] != comp[0]);
  // Topological order: SCC{0,1,2} -> SCC{3}
  ASSERT_TRUE(comp[0] < comp[3]);
}

TEST(max_flow_basic) {
  // s=0, t=3
  // 0 --(10)--> 1 --(5)--> 3
  // 0 --(8)-->  2 --(7)--> 3
  // 1 --(6)-->  2
  graph::MaxFlow mf(4);
  mf.add_edge(0, 1, 10);
  mf.add_edge(0, 2, 8);
  mf.add_edge(1, 3, 5);
  mf.add_edge(2, 3, 7);
  mf.add_edge(1, 2, 6);
  ASSERT_EQ(mf.max_flow(0, 3), 12LL);
}

TEST(euler_tour_basic) {
  // Tree: 0-1, 0-2, 1-3, 1-4
  vector<vector<int>> g(5);
  g[0].push_back(1);
  g[1].push_back(0);
  g[0].push_back(2);
  g[2].push_back(0);
  g[1].push_back(3);
  g[3].push_back(1);
  g[1].push_back(4);
  g[4].push_back(1);

  graph::EulerTour et(g, 0);
  // 0 is ancestor of all
  ASSERT_TRUE(et.is_ancestor(0, 1));
  ASSERT_TRUE(et.is_ancestor(0, 3));
  ASSERT_TRUE(et.is_ancestor(0, 4));
  ASSERT_TRUE(et.is_ancestor(0, 2));
  // 1 is ancestor of 3 and 4
  ASSERT_TRUE(et.is_ancestor(1, 3));
  ASSERT_TRUE(et.is_ancestor(1, 4));
  // 2 is not ancestor of 1
  ASSERT_FALSE(et.is_ancestor(2, 1));
  ASSERT_FALSE(et.is_ancestor(2, 3));
}

TEST(bfs01_basic) {
  // 0 -0- 1 -1- 2
  // 0 -1- 2
  vector<vector<pair<int, int>>> g(3);
  g[0].push_back({1, 0});
  g[1].push_back({2, 1});
  g[0].push_back({2, 1});
  auto dist = graph::bfs01(g, 0);
  ASSERT_EQ(dist[0], 0LL);
  ASSERT_EQ(dist[1], 0LL);
  ASSERT_EQ(dist[2], 1LL);
}

TEST(tree_diameter_basic) {
  // 3-1-0-2-4  diameter 4
  vector<vector<int>> g(5);
  auto add = [&](int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
  };
  add(0, 1);
  add(0, 2);
  add(1, 3);
  add(2, 4);
  auto [diam, uv] = graph::tree_diameter(g);
  ASSERT_EQ(diam, 4);
  ASSERT_EQ(graph::LCA(g, 0).dist(uv.first, uv.second), 4);
}

TEST(twosat_basic) {
  graph::TwoSat ts(2);
  ts.add_clause(0, false, 1, true); // ~x0 or x1
  ts.add_clause(0, true, 1, true);  // x0 or x1
  ASSERT_TRUE(ts.satisfiable());
  ASSERT_TRUE(ts.ans[1]);
}

TEST(twosat_unsat) {
  graph::TwoSat ts(1);
  ts.add_clause(0, true, 0, true);   // x0
  ts.add_clause(0, false, 0, false); // ~x0
  ASSERT_FALSE(ts.satisfiable());
}

TEST(min_cost_flow_basic) {
  graph::MinCostFlow mcf(4);
  mcf.add_edge(0, 1, 2, 3);
  mcf.add_edge(0, 2, 2, 5);
  mcf.add_edge(1, 3, 2, 1);
  mcf.add_edge(2, 3, 2, 1);
  auto [flow, cost] = mcf.min_cost_flow(0, 3, 3);
  ASSERT_EQ(flow, 3LL);
  ASSERT_EQ(cost, 14LL); // 2*(3+1) + 1*(5+1)
}

int main() { RUN_ALL_TESTS(); }
