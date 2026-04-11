// デバッグマクロ
#ifdef LOCAL
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &p) {
  return os << "(" << p.first << ", " << p.second << ")";
}
template <typename T> ostream &operator<<(ostream &os, const vector<T> &v) {
  os << "[";
  for (int i = 0; i < (int)v.size(); i++) {
    if (i)
      os << ", ";
    os << v[i];
  }
  return os << "]";
}
template <typename T> ostream &operator<<(ostream &os, const set<T> &s) {
  os << "{";
  bool first = true;
  for (const auto &x : s) {
    if (!first)
      os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
template <typename K, typename V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
  os << "{";
  bool first = true;
  for (const auto &[k, v] : m) {
    if (!first)
      os << ", ";
    first = false;
    os << k << ": " << v;
  }
  return os << "}";
}
template <typename T> ostream &operator<<(ostream &os, const multiset<T> &s) {
  os << "{";
  bool first = true;
  for (const auto &x : s) {
    if (!first)
      os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
template <typename T> ostream &operator<<(ostream &os, const deque<T> &v) {
  os << "[";
  for (int i = 0; i < (int)v.size(); i++) {
    if (i)
      os << ", ";
    os << v[i];
  }
  return os << "]";
}
void debug_out() { cerr << '\n'; }
template <typename Head, typename... Tail> void debug_out(Head H, Tail... T) {
  cerr << " " << H;
  if constexpr (sizeof...(T) > 0)
    cerr << ",";
  debug_out(T...);
}
#define debug(...) cerr << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
#else
#define debug(...) (void)0
#endif
