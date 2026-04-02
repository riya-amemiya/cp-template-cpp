#include "../main.cpp"
#include "test_helper.hpp"

TEST(modint_basic_arithmetic) {
  mint a(3), b(5);
  ASSERT_EQ((a + b).x, 8LL);
  ASSERT_EQ((a - b).x, (ll)MOD - 2);
  ASSERT_EQ((a * b).x, 15LL);
}

TEST(modint_negative) {
  mint a(-1);
  ASSERT_EQ(a.x, (ll)MOD - 1);
  mint b = -mint(3);
  ASSERT_EQ(b.x, (ll)MOD - 3);
}

TEST(modint_pow) {
  mint a(2);
  ASSERT_EQ(a.pow(10).x, 1024LL);
  ASSERT_EQ(a.pow(0).x, 1LL);
}

TEST(modint_inv) {
  mint a(2);
  mint inv_a = a.inv();
  ASSERT_EQ((a * inv_a).x, 1LL);

  mint b(123456789);
  ASSERT_EQ((b * b.inv()).x, 1LL);
}

TEST(modint_division) {
  mint a(10), b(2);
  ASSERT_EQ((a / b).x, 5LL);
}

TEST(modint_comparison) {
  mint a(3), b(3), c(5);
  ASSERT_TRUE(a == b);
  ASSERT_TRUE(a != c);
}

TEST(mint998_basic) {
  mint998 a(MOD998 - 1), b(2);
  ASSERT_EQ((a + b).x, 1LL);
}

int main() { RUN_ALL_TESTS(); }
