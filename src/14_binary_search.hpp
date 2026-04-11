// @begin binary_search
// @exports binary_search_left binary_search_right binary_search_real
// 二分探索関連
namespace binary_search {
// 整数二分探索（以上）
template <typename F> ll binary_search_left(ll ok, ll ng, const F &f) {
  while (abs(ok - ng) > 1) {
    ll mid = (ok + ng) / 2;
    if (f(mid))
      ok = mid;
    else
      ng = mid;
  }
  return ok;
}

// 整数二分探索（より大きい）
template <typename F> ll binary_search_right(ll ng, ll ok, const F &f) {
  while (abs(ok - ng) > 1) {
    ll mid = (ok + ng) / 2;
    if (f(mid))
      ng = mid;
    else
      ok = mid;
  }
  return ok;
}

// 実数二分探索
template <typename F>
double binary_search_real(double left, double right, const F &f,
                          int iter = 100) {
  for (int i = 0; i < iter; i++) {
    double mid = (left + right) / 2;
    if (f(mid))
      right = mid;
    else
      left = mid;
  }
  return right;
}
} // namespace binary_search
// @end binary_search
