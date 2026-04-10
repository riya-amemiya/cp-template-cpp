// @begin Matrix
// 行列 (行列累乗対応)
template <typename T> struct Matrix {
  int n, m;
  vector<vector<T>> a;
  Matrix(int n_, int m_, T val = 0) : n(n_), m(m_), a(n_, vector<T>(m_, val)) {}
  Matrix(const vector<vector<T>> &a_) : n(a_.size()), m(a_[0].size()), a(a_) {}

  vector<T> &operator[](int i) { return a[i]; }
  const vector<T> &operator[](int i) const { return a[i]; }

  static Matrix E(int n) {
    Matrix res(n, n);
    for (int i = 0; i < n; i++)
      res[i][i] = 1;
    return res;
  }

  Matrix operator*(const Matrix &B) const {
    assert(m == B.n);
    Matrix C(n, B.m);
    for (int i = 0; i < n; i++)
      for (int k = 0; k < m; k++)
        for (int j = 0; j < B.m; j++)
          C[i][j] += a[i][k] * B.a[k][j];
    return C;
  }

  Matrix pow(ll exp) const {
    assert(n == m);
    Matrix ret = E(n), base = *this;
    while (exp > 0) {
      if (exp & 1)
        ret = ret * base;
      base = base * base;
      exp >>= 1;
    }
    return ret;
  }
};
// @end Matrix
