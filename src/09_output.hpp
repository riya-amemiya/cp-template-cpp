// 出力ヘルパー
template <typename T>
void print_vec(const vector<T> &v, const string &sep = " ") {
  for (int i = 0; i < (int)v.size(); i++) {
    if (i)
      cout << sep;
    cout << v[i];
  }
  cout << '\n';
}

// TUNING: Yes/No 出力ヘルパー
inline void Yes(bool b = true) { cout << (b ? "Yes" : "No") << '\n'; }
inline void No() { Yes(false); }
inline void YES(bool b = true) { cout << (b ? "YES" : "NO") << '\n'; }
inline void NO() { YES(false); }
