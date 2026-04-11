// @begin CumulativeSum
// @exports CumulativeSum CumulativeSum2D
// 累積和 (1次元)
template <typename T> struct CumulativeSum {
  vector<T> dat;
  bool built = false;
  CumulativeSum(int n) : dat(n + 1, 0) {}

  void add(int i, T x) { dat[i + 1] += x; }

  void build() {
    for (int i = 0; i < (int)dat.size() - 1; i++)
      dat[i + 1] += dat[i];
    built = true;
  }

  // [l, r) の総和
  T sum(int l, int r) const {
    assert(built);
    return dat[r] - dat[l];
  }
};

// 累積和 (2次元)
template <typename T> struct CumulativeSum2D {
  vector<vector<T>> dat;
  bool built = false;
  CumulativeSum2D(int h, int w) : dat(h + 1, vector<T>(w + 1, 0)) {}

  void add(int i, int j, T x) { dat[i + 1][j + 1] += x; }

  void build() {
    int h = dat.size(), w = dat[0].size();
    for (int i = 1; i < h; i++)
      for (int j = 1; j < w; j++)
        dat[i][j] += dat[i - 1][j] + dat[i][j - 1] - dat[i - 1][j - 1];
    built = true;
  }

  // [i1, i2) x [j1, j2) の総和
  T sum(int i1, int j1, int i2, int j2) const {
    assert(built);
    return dat[i2][j2] - dat[i1][j2] - dat[i2][j1] + dat[i1][j1];
  }
};
// @end CumulativeSum
