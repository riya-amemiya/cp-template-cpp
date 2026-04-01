#pragma once

// main.cppの #define endl '\n' が std::endl を壊すので解除
#ifdef endl
#undef endl
#endif

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace test {
struct TestCase {
  std::string name;
  std::function<void()> func;
};

inline std::vector<TestCase> &test_cases() {
  static std::vector<TestCase> cases;
  return cases;
}

inline int &fail_count() {
  static int count = 0;
  return count;
}

struct TestRegistrar {
  TestRegistrar(const std::string &name, std::function<void()> func) {
    test_cases().push_back({name, func});
  }
};

inline void assert_fail(const std::string &file, int line,
                        const std::string &msg) {
  std::cerr << "  FAIL: " << file << ":" << line << " " << msg << std::endl;
  fail_count()++;
}

inline int run_all_tests() {
  int passed = 0, failed = 0;
  for (auto &[name, func] : test_cases()) {
    int before = fail_count();
    std::cerr << "[ RUN  ] " << name << std::endl;
    func();
    if (fail_count() == before) {
      std::cerr << "[ PASS ] " << name << std::endl;
      passed++;
    } else {
      std::cerr << "[ FAIL ] " << name << std::endl;
      failed++;
    }
  }
  std::cerr << "\n" << passed << " passed, " << failed << " failed"
            << std::endl;
  return failed > 0 ? 1 : 0;
}
} // namespace test

#define TEST(name)                                                             \
  void test_##name();                                                          \
  static test::TestRegistrar reg_##name(#name, test_##name);                   \
  void test_##name()

#define ASSERT_EQ(a, b)                                                        \
  do {                                                                         \
    if ((a) != (b)) {                                                          \
      test::assert_fail(__FILE__, __LINE__, #a " == " #b);                     \
    }                                                                          \
  } while (0)

#define ASSERT_TRUE(cond)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      test::assert_fail(__FILE__, __LINE__, #cond);                            \
    }                                                                          \
  } while (0)

#define ASSERT_FALSE(cond)                                                     \
  do {                                                                         \
    if ((cond)) {                                                              \
      test::assert_fail(__FILE__, __LINE__, "!(" #cond ")");                   \
    }                                                                          \
  } while (0)

#define ASSERT_NEAR(a, b, eps)                                                 \
  do {                                                                         \
    auto _a = (a);                                                             \
    auto _b = (b);                                                             \
    if (std::abs(_a - _b) > (eps)) {                                           \
      test::assert_fail(__FILE__, __LINE__, #a " ≈ " #b);                      \
    }                                                                          \
  } while (0)

#define RUN_ALL_TESTS() return test::run_all_tests()
