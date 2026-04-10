// 入力ヘルパー
template <typename T> T input() {
  T x;
  cin >> x;
  return x;
}
template <typename T> vector<T> input_vec(int n) {
  vector<T> v(n);
  for (auto &x : v)
    cin >> x;
  return v;
}
template <typename T> vector<vector<T>> input_vec2(int n, int m) {
  vector<vector<T>> v(n, vector<T>(m));
  for (auto &row : v)
    for (auto &x : row)
      cin >> x;
  return v;
}

// TUNING: クイック入力マクロ
inline void input_from_cin() {}
template <typename Head, typename... Tail>
inline void input_from_cin(Head &head, Tail &...tail) {
  cin >> head;
  input_from_cin(tail...);
}
#define INT(...) int __VA_ARGS__; input_from_cin(__VA_ARGS__)
#define LL(...) ll __VA_ARGS__; input_from_cin(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; input_from_cin(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__; input_from_cin(__VA_ARGS__)
#define DBL(...) double __VA_ARGS__; input_from_cin(__VA_ARGS__)
#define VEC(type, name, size) vector<type> name(size); for(auto &_x : name) cin >> _x
#define VEC2(type, name, h, w) vector<vector<type>> name(h, vector<type>(w)); for(auto &_r : name) for(auto &_x : _r) cin >> _x
