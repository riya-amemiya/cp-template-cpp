// @begin util
// @exports argsort rle rotate90
template <typename T> vector<int> argsort(const vector<T> &v) {
  vector<int> idx(sz(v));
  iota(all(idx), 0);
  sort(all(idx),
       [&](int i, int j) { return v[i] < v[j] || (v[i] == v[j] && i < j); });
  return idx;
}

template <typename T> vector<pair<T, int>> rle(const vector<T> &v) {
  vector<pair<T, int>> res;
  for (const auto &x : v) {
    if (res.empty() || res.back().first != x)
      res.emplace_back(x, 1);
    else
      res.back().second++;
  }
  return res;
}

inline vector<pair<char, int>> rle(const string &s) {
  vector<pair<char, int>> res;
  for (char c : s) {
    if (res.empty() || res.back().first != c)
      res.emplace_back(c, 1);
    else
      res.back().second++;
  }
  return res;
}

template <typename T> vector<vector<T>> rotate90(const vector<vector<T>> &a) {
  int h = sz(a), w = sz(a[0]);
  vector<vector<T>> b(w, vector<T>(h));
  rep(i, h) rep(j, w) b[j][h - 1 - i] = a[i][j];
  return b;
}
// @end util
