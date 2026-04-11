// @begin string_algo
// @exports z_algorithm kmp_table kmp_search suffix_array lcp_array
// 文字列アルゴリズム
namespace string_algo {
// Z-algorithm: z[i] = s[i..] と s の最長共通接頭辞の長さ
vector<int> z_algorithm(const string &s) {
  int n = s.size();
  vector<int> z(n);
  z[0] = n;
  int i = 1, j = 0;
  while (i < n) {
    while (i + j < n && s[j] == s[i + j])
      j++;
    z[i] = j;
    if (j == 0) {
      i++;
      continue;
    }
    int k = 1;
    while (k < j && k + z[k] < j) {
      z[i + k] = z[k];
      k++;
    }
    i += k;
    j -= k;
  }
  return z;
}

// KMP法: prefix function (failure function)
// pi[i] = s[0..i] の最長の proper prefix-suffix の長さ
vector<int> kmp_table(const string &s) {
  int n = s.size();
  vector<int> pi(n, 0);
  for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j])
      j = pi[j - 1];
    if (s[i] == s[j])
      j++;
    pi[i] = j;
  }
  return pi;
}

// KMP法によるパターン検索: text中のpatternの出現位置を返す
vector<int> kmp_search(const string &text, const string &pattern) {
  string s = pattern + "$" + text;
  auto pi = kmp_table(s);
  vector<int> res;
  int m = pattern.size();
  for (int i = 2 * m; i < (int)s.size(); i++) {
    if (pi[i] == m)
      res.push_back(i - 2 * m);
  }
  return res;
}

// Suffix Array (O(n log^2 n))
vector<int> suffix_array(const string &s) {
  int n = s.size();
  vector<int> sa(n), rank_(n), tmp(n);
  iota(all(sa), 0);
  for (int i = 0; i < n; i++)
    rank_[i] = s[i];
  for (int k = 1; k < n; k *= 2) {
    auto cmp = [&](int a, int b) {
      if (rank_[a] != rank_[b])
        return rank_[a] < rank_[b];
      int ra = a + k < n ? rank_[a + k] : -1;
      int rb = b + k < n ? rank_[b + k] : -1;
      return ra < rb;
    };
    sort(all(sa), cmp);
    tmp[sa[0]] = 0;
    for (int i = 1; i < n; i++)
      tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
    rank_ = tmp;
  }
  return sa;
}

// LCP Array (Kasai's algorithm, O(n))
// sa: suffix array, 返り値: lcp[i] = sa[i]とsa[i+1]のLCP長
vector<int> lcp_array(const string &s, const vector<int> &sa) {
  int n = s.size();
  vector<int> rank_(n), lcp(n - 1);
  for (int i = 0; i < n; i++)
    rank_[sa[i]] = i;
  int h = 0;
  for (int i = 0; i < n; i++) {
    if (rank_[i] > 0) {
      int j = sa[rank_[i] - 1];
      while (i + h < n && j + h < n && s[i + h] == s[j + h])
        h++;
      lcp[rank_[i] - 1] = h;
      if (h > 0)
        h--;
    } else {
      h = 0;
    }
  }
  return lcp;
}
} // namespace string_algo
// @end string_algo
