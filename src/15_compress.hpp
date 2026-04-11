// @begin Compress
// 座標圧縮
template <typename T> struct Compress {
  vector<T> xs;
  Compress() = default;
  Compress(const vector<T> &v) { add(v); }

  void add(const T &x) { xs.push_back(x); }
  void add(const vector<T> &v) {
    for (const auto &x : v)
      xs.push_back(x);
  }

  void build() {
    sort(all(xs));
    xs.erase(unique(all(xs)), xs.end());
  }

  int get(const T &x) const { return lower_bound(all(xs), x) - xs.begin(); }
  int size() const { return xs.size(); }
  const T &operator[](int i) const { return xs[i]; }
};
// @end Compress
