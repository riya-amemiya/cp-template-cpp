#include "../main.cpp"
#include "test_helper.hpp"

TEST(chmin_chmax) {
  int a = 5;
  ASSERT_TRUE(chmin(a, 3));
  ASSERT_EQ(a, 3);
  ASSERT_FALSE(chmin(a, 10));
  ASSERT_EQ(a, 3);

  int b = 5;
  ASSERT_TRUE(chmax(b, 10));
  ASSERT_EQ(b, 10);
  ASSERT_FALSE(chmax(b, 3));
  ASSERT_EQ(b, 10);
}

TEST(Compress_basic) {
  Compress<int> comp;
  comp.add(30);
  comp.add(10);
  comp.add(20);
  comp.add(10);
  comp.build();
  ASSERT_EQ(comp.size(), 3);
  ASSERT_EQ(comp.get(10), 0);
  ASSERT_EQ(comp.get(20), 1);
  ASSERT_EQ(comp.get(30), 2);
  ASSERT_EQ(comp[0], 10);
}

TEST(Compress_vector) {
  vector<int> v = {5, 1, 3, 1, 5};
  Compress<int> comp(v);
  comp.build();
  ASSERT_EQ(comp.size(), 3);
  ASSERT_EQ(comp.get(1), 0);
  ASSERT_EQ(comp.get(3), 1);
  ASSERT_EQ(comp.get(5), 2);
}

TEST(CumulativeSum_1d) {
  CumulativeSum<ll> cs(5);
  cs.add(0, 1);
  cs.add(1, 2);
  cs.add(2, 3);
  cs.add(3, 4);
  cs.add(4, 5);
  cs.build();
  ASSERT_EQ(cs.sum(0, 5), 15LL);
  ASSERT_EQ(cs.sum(1, 4), 9LL);
  ASSERT_EQ(cs.sum(0, 1), 1LL);
}

TEST(CumulativeSum2D_basic) {
  CumulativeSum2D<ll> cs(3, 3);
  // 1 2 3
  // 4 5 6
  // 7 8 9
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      cs.add(i, j, i * 3 + j + 1);
  cs.build();
  ASSERT_EQ(cs.sum(0, 0, 3, 3), 45LL); // all
  ASSERT_EQ(cs.sum(0, 0, 1, 1), 1LL);  // top-left
  ASSERT_EQ(cs.sum(1, 1, 3, 3), 28LL); // 5+6+8+9
  ASSERT_EQ(cs.sum(0, 0, 2, 2), 12LL); // 1+2+4+5
}

TEST(binary_search_left) {
  // Find minimum x in [0, 100] where x*x >= 50
  auto f = [](ll x) { return x * x >= 50; };
  ll ans = binary_search::binary_search_left(100LL, 0LL, f);
  ASSERT_EQ(ans, 8LL); // 8*8=64 >= 50, 7*7=49 < 50
}

TEST(binary_search_right) {
  // binary_search_right(ng, ok, f): f(mid)==true -> ng=mid, else ok=mid
  // Find maximum x where x*x >= 50 is false (i.e., x*x < 50)
  // f(x) = x*x >= 50: f(8)=true, f(7)=false
  // ng=100 (known f=true), ok=0 (known f=false)
  auto f = [](ll x) { return x * x >= 50; };
  ll ans = binary_search::binary_search_right(100LL, 0LL, f);
  ASSERT_EQ(ans, 7LL); // 7*7=49 < 50, 8*8=64 >= 50
}

TEST(binary_search_real) {
  // Find sqrt(2) ≈ 1.4142...
  auto f = [](double x) { return x * x >= 2.0; };
  double ans = binary_search::binary_search_real(0.0, 2.0, f);
  ASSERT_NEAR(ans, sqrt(2.0), 1e-9);
}

TEST(Matrix_multiply) {
  Matrix<ll> A({{1, 2}, {3, 4}});
  Matrix<ll> B({{5, 6}, {7, 8}});
  auto C = A * B;
  ASSERT_EQ(C[0][0], 19LL);
  ASSERT_EQ(C[0][1], 22LL);
  ASSERT_EQ(C[1][0], 43LL);
  ASSERT_EQ(C[1][1], 50LL);
}

TEST(Matrix_pow) {
  // Fibonacci via matrix exponentiation
  // [[1,1],[1,0]]^n gives fib(n+1), fib(n)
  Matrix<ll> fib({{1, 1}, {1, 0}});
  auto f10 = fib.pow(10);
  ASSERT_EQ(f10[0][1], 55LL); // fib(10) = 55
  ASSERT_EQ(f10[1][1], 34LL); // fib(9) = 34
}

TEST(Matrix_identity) {
  auto I = Matrix<ll>::E(3);
  ASSERT_EQ(I[0][0], 1LL);
  ASSERT_EQ(I[1][1], 1LL);
  ASSERT_EQ(I[2][2], 1LL);
  ASSERT_EQ(I[0][1], 0LL);
}

TEST(vv_helper) {
  auto v = vv<int>(3, 4, -1);
  ASSERT_EQ((int)v.size(), 3);
  ASSERT_EQ((int)v[0].size(), 4);
  ASSERT_EQ(v[2][3], -1);
}

int main() { RUN_ALL_TESTS(); }
