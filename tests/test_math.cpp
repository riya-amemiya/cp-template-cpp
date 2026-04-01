#include "../main.cpp"
#include "test_helper.hpp"

TEST(pow_mod) {
  ASSERT_EQ(math::pow_mod<ll>(2, 10, 1000), 24);
  ASSERT_EQ(math::pow_mod<ll>(3, 13, 1000000007), 1594323);
  ASSERT_EQ(math::pow_mod<ll>(2, 0, 100), 1);
}

TEST(is_prime) {
  ASSERT_FALSE(math::is_prime(0));
  ASSERT_FALSE(math::is_prime(1));
  ASSERT_TRUE(math::is_prime(2));
  ASSERT_TRUE(math::is_prime(3));
  ASSERT_FALSE(math::is_prime(4));
  ASSERT_TRUE(math::is_prime(97));
  ASSERT_FALSE(math::is_prime(100));
  ASSERT_TRUE(math::is_prime(1000000007));
}

TEST(sieve) {
  auto s = math::sieve(20);
  ASSERT_FALSE(s[0]);
  ASSERT_FALSE(s[1]);
  ASSERT_TRUE(s[2]);
  ASSERT_TRUE(s[3]);
  ASSERT_FALSE(s[4]);
  ASSERT_TRUE(s[5]);
  ASSERT_TRUE(s[7]);
  ASSERT_FALSE(s[9]);
  ASSERT_TRUE(s[19]);
  ASSERT_FALSE(s[20]);
}

TEST(prime_list) {
  auto primes = math::prime_list(30);
  vector<int> expected = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
  ASSERT_EQ(primes, expected);
}

TEST(factorize) {
  auto f = math::factorize(12);
  ASSERT_EQ((int)f.size(), 2);
  ASSERT_EQ(f[0].first, 2LL);
  ASSERT_EQ(f[0].second, 2);
  ASSERT_EQ(f[1].first, 3LL);
  ASSERT_EQ(f[1].second, 1);

  auto f2 = math::factorize(1);
  ASSERT_TRUE(f2.empty());
}

TEST(divisors) {
  auto d = math::divisors(12);
  vector<ll> expected = {1, 2, 3, 4, 6, 12};
  ASSERT_EQ(d, expected);
}

TEST(extgcd) {
  ll x, y;
  ll g = math::extgcd(12, 8, x, y);
  ASSERT_EQ(g, 4LL);
  ASSERT_EQ(12 * x + 8 * y, 4LL);
}

TEST(combination) {
  math::combination comb(10);
  ASSERT_EQ(comb(5, 2), 10LL);
  ASSERT_EQ(comb(10, 0), 1LL);
  ASSERT_EQ(comb(10, 10), 1LL);
  ASSERT_EQ(comb(5, 3), 10LL);
  ASSERT_EQ(comb(3, 5), 0LL); // k > n

  ASSERT_EQ(comb.perm(5, 2), 20LL);
  ASSERT_EQ(comb.perm(5, 0), 1LL);
}

TEST(crt) {
  // x ≡ 2 (mod 3), x ≡ 3 (mod 5) => x ≡ 8 (mod 15)
  auto [r, m] = math::crt(2, 3, 3, 5);
  ASSERT_EQ(r, 8LL);
  ASSERT_EQ(m, 15LL);

  // 解なし: x ≡ 0 (mod 2), x ≡ 1 (mod 2)
  auto [r2, m2] = math::crt(0, 2, 1, 2);
  ASSERT_EQ(m2, -1LL);
}

TEST(floor_sum) {
  // sum_{i=0}^{4} floor((6*i + 1) / 5) = 0 + 1 + 2 + 3 + 5 = 11
  ASSERT_EQ(math::floor_sum(5, 5, 6, 1), 11LL);
  ASSERT_EQ(math::floor_sum(0, 1, 1, 1), 0LL);
}

TEST(euler_totient) {
  ASSERT_EQ(math::euler_totient(1), 1LL);
  ASSERT_EQ(math::euler_totient(2), 1LL);
  ASSERT_EQ(math::euler_totient(6), 2LL);
  ASSERT_EQ(math::euler_totient(12), 4LL);
  ASSERT_EQ(math::euler_totient(7), 6LL); // 素数
}

TEST(euler_totient_table) {
  auto phi = math::euler_totient_table(12);
  ASSERT_EQ(phi[1], 1LL);
  ASSERT_EQ(phi[2], 1LL);
  ASSERT_EQ(phi[6], 2LL);
  ASSERT_EQ(phi[12], 4LL);
  ASSERT_EQ(phi[7], 6LL);
}

int main() { RUN_ALL_TESTS(); }
