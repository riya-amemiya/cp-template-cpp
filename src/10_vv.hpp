// 2次元ベクトルの初期化
template <typename T> vector<vector<T>> vv(int n, int m, T x = T()) {
  return vector<vector<T>>(n, vector<T>(m, x));
}
