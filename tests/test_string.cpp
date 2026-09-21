#include "../main.cpp"
#include "test_helper.hpp"

TEST(rolling_hash_basic) {
  RollingHash rh("abcabc");
  ASSERT_TRUE(rh.match(0, 3, 3, 6));  // "abc" == "abc"
  ASSERT_FALSE(rh.match(0, 3, 1, 4)); // "abc" != "bca"
}

TEST(rolling_hash_get) {
  RollingHash rh("abcde");
  ASSERT_EQ(rh.get(0, 3), rh.get(0, 3));
  // Different substrings should (very likely) have different hashes
  ASSERT_TRUE(rh.get(0, 3) != rh.get(1, 4));
}

TEST(z_algorithm_basic) {
  auto z = string_algo::z_algorithm("aabaaab");
  // z[0] = 7 (whole string), z[1]=1, z[2]=0, z[3]=2, z[4]=3, z[5]=1, z[6]=0
  ASSERT_EQ(z[0], 7);
  ASSERT_EQ(z[1], 1);
  ASSERT_EQ(z[2], 0);
  ASSERT_EQ(z[3], 2);
  ASSERT_EQ(z[4], 3);
  ASSERT_EQ(z[5], 1);
  ASSERT_EQ(z[6], 0);
}

TEST(z_algorithm_all_same) {
  auto z = string_algo::z_algorithm("aaaa");
  ASSERT_EQ(z[0], 4);
  ASSERT_EQ(z[1], 3);
  ASSERT_EQ(z[2], 2);
  ASSERT_EQ(z[3], 1);
}

TEST(kmp_table_basic) {
  auto pi = string_algo::kmp_table("abcabd");
  // pi = [0, 0, 0, 1, 2, 0]
  ASSERT_EQ(pi[0], 0);
  ASSERT_EQ(pi[1], 0);
  ASSERT_EQ(pi[2], 0);
  ASSERT_EQ(pi[3], 1);
  ASSERT_EQ(pi[4], 2);
  ASSERT_EQ(pi[5], 0);
}

TEST(kmp_search_basic) {
  auto res = string_algo::kmp_search("abababab", "abab");
  vector<int> expected = {0, 2, 4};
  ASSERT_EQ(res, expected);
}

TEST(kmp_search_no_match) {
  auto res = string_algo::kmp_search("abcdef", "xyz");
  ASSERT_TRUE(res.empty());
}

TEST(kmp_search_dollar_in_text) {
  auto res = string_algo::kmp_search("a$a$a", "a$a");
  vector<int> expected = {0, 2};
  ASSERT_EQ(res, expected);
}

TEST(suffix_array_basic) {
  auto sa = string_algo::suffix_array("banana");
  // Suffixes sorted: "a"(5), "ana"(3), "anana"(1), "banana"(0), "na"(4),
  // "nana"(2)
  vector<int> expected = {5, 3, 1, 0, 4, 2};
  ASSERT_EQ(sa, expected);
}

TEST(lcp_array_basic) {
  string s = "banana";
  auto sa = string_algo::suffix_array(s);
  auto lcp = string_algo::lcp_array(s, sa);
  // LCP between consecutive suffixes in SA:
  // "a" vs "ana" = 1
  // "ana" vs "anana" = 3
  // "anana" vs "banana" = 0
  // "banana" vs "na" = 0
  // "na" vs "nana" = 2
  vector<int> expected = {1, 3, 0, 0, 2};
  ASSERT_EQ(lcp, expected);
}

TEST(manacher_odd_even) {
  auto r = string_algo::manacher("aba");
  // t = # a # b # a #
  //     0 1 2 3 4 5 6
  ASSERT_EQ((int)r.size(), 7);
  ASSERT_EQ(r[3], 3); // center 'b' covers whole "aba"
  ASSERT_EQ(r[1], 1); // 'a'
  auto r2 = string_algo::manacher("aa");
  // t = # a # a #
  ASSERT_EQ(r2[2], 2); // even palindrome "aa" centered at '#'
}

int main() { RUN_ALL_TESTS(); }
