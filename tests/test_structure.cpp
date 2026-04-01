#include "../main.cpp"
#include "test_helper.hpp"

TEST(BIT_basic) {
  structure::BIT<ll> bit(5);
  bit.add(0, 3);
  bit.add(1, 5);
  bit.add(2, 7);
  bit.add(3, 1);
  bit.add(4, 4);
  ASSERT_EQ(bit.sum(5), 20LL);
  ASSERT_EQ(bit.sum(3), 15LL);
  ASSERT_EQ(bit.sum(1, 4), 13LL);
  ASSERT_EQ(bit.get(2), 7LL);

  bit.set(2, 10);
  ASSERT_EQ(bit.get(2), 10LL);
  ASSERT_EQ(bit.sum(5), 23LL);
}

TEST(UnionFind_basic) {
  structure::UnionFind uf(5);
  ASSERT_FALSE(uf.same(0, 1));
  uf.unite(0, 1);
  ASSERT_TRUE(uf.same(0, 1));
  ASSERT_EQ(uf.size(0), 2);

  uf.unite(2, 3);
  uf.unite(0, 3);
  ASSERT_TRUE(uf.same(1, 2));
  ASSERT_EQ(uf.size(0), 4);
  ASSERT_FALSE(uf.same(0, 4));
}

TEST(WeightedUnionFind_basic) {
  structure::WeightedUnionFind<ll> wuf(4);
  wuf.unite(0, 1, 3); // weight(1) - weight(0) = 3
  wuf.unite(1, 2, 5); // weight(2) - weight(1) = 5
  ASSERT_TRUE(wuf.same(0, 2));
  ASSERT_EQ(wuf.diff(0, 2), 8LL); // weight(2) - weight(0) = 8
  ASSERT_EQ(wuf.diff(0, 1), 3LL);
  ASSERT_FALSE(wuf.same(0, 3));
}

TEST(SegTree_min) {
  structure::SegTree<int> seg(5, INF, [](int a, int b) { return min(a, b); });
  seg.update(0, 3);
  seg.update(1, 1);
  seg.update(2, 4);
  seg.update(3, 1);
  seg.update(4, 5);
  ASSERT_EQ(seg.query(0, 5), 1);
  ASSERT_EQ(seg.query(0, 1), 3);
  ASSERT_EQ(seg.query(2, 5), 1);
  ASSERT_EQ(seg.query(0, 3), 1);
}

TEST(LazySegTree_range_assign_range_min) {
  // Range assign, range min
  const ll NONE = LINF + 1; // lazy identity
  int n = 5;
  structure::LazySegTree<ll, ll> seg(
      n, LINF, NONE,
      [](ll a, ll b) { return min(a, b); },                // f: merge
      [](ll a, ll b) -> ll { return b == LINF + 1 ? a : b; }, // g: apply
      [](ll a, ll b) -> ll { return b == LINF + 1 ? a : b; }); // h: compose

  // 各要素をセット
  seg.update(0, 1, 3);
  seg.update(1, 2, 1);
  seg.update(2, 3, 4);
  seg.update(3, 4, 1);
  seg.update(4, 5, 5);
  ASSERT_EQ(seg.query(0, 5), 1LL);
  ASSERT_EQ(seg.query(0, 1), 3LL);
  ASSERT_EQ(seg.query(2, 5), 1LL);

  // 区間 [1,4) を 10 に更新
  seg.update(1, 4, 10);
  ASSERT_EQ(seg.query(0, 5), 3LL); // min(3, 10, 10, 10, 5) = 3
  ASSERT_EQ(seg.query(1, 4), 10LL);
}

TEST(SparseTable_min) {
  vector<int> v = {5, 3, 7, 1, 4, 2, 8};
  structure::SparseTable<int> st(v, [](int a, int b) { return min(a, b); });
  ASSERT_EQ(st.query(0, 7), 1);
  ASSERT_EQ(st.query(0, 2), 3);
  ASSERT_EQ(st.query(3, 6), 1);
  ASSERT_EQ(st.query(4, 7), 2);
  ASSERT_EQ(st.query(0, 1), 5);
}

int main() { RUN_ALL_TESTS(); }
