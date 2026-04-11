#include <bits/stdc++.h>
using namespace std;

// 型エイリアス
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vs = vector<string>;
// TUNING: 追加の型エイリアス
using vpii = vector<pii>;
using vpll = vector<pll>;
using vld = vector<ld>;
using vull = vector<ull>;

// 定数
constexpr int INF = 1e9;
constexpr ll LINF = 1e18;
constexpr int MOD = 1e9 + 7;
constexpr int MOD998 = 998244353;
constexpr double PI = M_PI;
constexpr double EPS = 1e-9;

// グリッド探索用の方向配列
constexpr int dx4[] = {0, 1, 0, -1};
constexpr int dy4[] = {1, 0, -1, 0};
constexpr int dx8[] = {0, 1, 1, 1, 0, -1, -1, -1};
constexpr int dy8[] = {1, 1, 0, -1, -1, -1, 0, 1};

// マクロ
#define rep(i, n) for (int i = 0; i < (int)(n); ++i)
#define rep1(i, n) for (int i = 1; i <= (int)(n); ++i)
#define rep2(i, a, b) for (int i = (int)(a); i < (int)(b); ++i)
#define rrep(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define rrep1(i, n) for (int i = (int)(n); i >= 1; --i)
#define fore(i, a) for (auto &i : a)
#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()
#define sz(x) ((int)(x).size())
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define fi first
#define se second
#define endl '\n'
#define Sort(v) sort(all(v))
#define Reverse(v) reverse(all(v))
#define Unique(v) v.erase(unique(all(v)), v.end())

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

// chmin/chmax
template <typename T> bool chmin(T &a, const T &b) {
  if (a > b) {
    a = b;
    return true;
  }
  return false;
}
template <typename T> bool chmax(T &a, const T &b) {
  if (a < b) {
    a = b;
    return true;
  }
  return false;
}

// 入出力の高速化
void fastio() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  cout << fixed << setprecision(20);
}

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
#define INT(...)                                                               \
  int __VA_ARGS__;                                                             \
  input_from_cin(__VA_ARGS__)
#define LL(...)                                                                \
  ll __VA_ARGS__;                                                              \
  input_from_cin(__VA_ARGS__)
#define STR(...)                                                               \
  string __VA_ARGS__;                                                          \
  input_from_cin(__VA_ARGS__)
#define CHR(...)                                                               \
  char __VA_ARGS__;                                                            \
  input_from_cin(__VA_ARGS__)
#define DBL(...)                                                               \
  double __VA_ARGS__;                                                          \
  input_from_cin(__VA_ARGS__)
#define VEC(type, name, size)                                                  \
  vector<type> name(size);                                                     \
  for (auto &_x : name)                                                        \
  cin >> _x
#define VEC2(type, name, h, w)                                                 \
  vector<vector<type>> name(h, vector<type>(w));                               \
  for (auto &_r : name)                                                        \
    for (auto &_x : _r)                                                        \
  cin >> _x

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

// 2次元ベクトルの初期化
template <typename T> vector<vector<T>> vv(int n, int m, T x = T()) {
  return vector<vector<T>>(n, vector<T>(m, x));
}

// @begin modint
// @exports modint mint mint998
// modint構造体
template <int MOD_> struct modint {
  static constexpr int MOD = MOD_;
  ll x;
  modint(ll x_ = 0) : x((x_ % MOD + MOD) % MOD) {}
  modint operator-() const { return modint(-x); }
  modint &operator+=(const modint &a) {
    if ((x += a.x) >= MOD)
      x -= MOD;
    return *this;
  }
  modint &operator-=(const modint &a) {
    if ((x += MOD - a.x) >= MOD)
      x -= MOD;
    return *this;
  }
  modint &operator*=(const modint &a) {
    x = x * a.x % MOD;
    return *this;
  }
  modint &operator/=(const modint &a) { return *this *= a.inv(); }
  modint operator+(const modint &a) const { return modint(*this) += a; }
  modint operator-(const modint &a) const { return modint(*this) -= a; }
  modint operator*(const modint &a) const { return modint(*this) *= a; }
  modint operator/(const modint &a) const { return modint(*this) /= a; }
  modint pow(ll n) const {
    modint ret(1), mul(x);
    while (n > 0) {
      if (n & 1)
        ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }
  modint inv() const { return pow(MOD - 2); }
  bool operator==(const modint &a) const { return x == a.x; }
  bool operator!=(const modint &a) const { return x != a.x; }
  bool operator<(const modint &a) const { return x < a.x; }
  friend ostream &operator<<(ostream &os, const modint &a) { return os << a.x; }
  friend istream &operator>>(istream &is, modint &a) {
    ll t;
    is >> t;
    a = modint(t);
    return is;
  }
};
using mint = modint<MOD>;
using mint998 = modint<MOD998>;
// @end modint

// @begin math
// @exports pow_mod is_prime sieve prime_list factorize divisors extgcd
// combination crt floor_sum euler_totient euler_totient_table 数学関連の関数群
namespace math {
// 高速な累乗計算 (繰り返し二乗法)
template <typename T> T pow_mod(T a, T n, T m) {
  T ret = 1;
  while (n > 0) {
    if (n & 1)
      ret = (ret * a) % m;
    a = (a * a) % m;
    n >>= 1;
  }
  return ret;
}

// 素数判定 (試し割り法, O(√n))
bool is_prime(ll n) {
  if (n <= 1)
    return false;
  if (n == 2)
    return true;
  if (n % 2 == 0)
    return false;
  for (ll i = 3; i * i <= n; i += 2) {
    if (n % i == 0)
      return false;
  }
  return true;
}

// エラトステネスの篩
vector<bool> sieve(int n) {
  vector<bool> is_p(n + 1, true);
  is_p[0] = is_p[1] = false;
  for (int i = 2; (ll)i * i <= n; i++)
    if (is_p[i])
      for (int j = i * i; j <= n; j += i)
        is_p[j] = false;
  return is_p;
}

// エラトステネスの篩で素数リストを返す
vector<int> prime_list(int n) {
  auto is_p = sieve(n);
  vector<int> primes;
  for (int i = 2; i <= n; i++)
    if (is_p[i])
      primes.push_back(i);
  return primes;
}

// 素因数分解 O(√n)
vector<pair<ll, int>> factorize(ll n) {
  vector<pair<ll, int>> res;
  for (ll i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      int cnt = 0;
      while (n % i == 0) {
        n /= i;
        cnt++;
      }
      res.emplace_back(i, cnt);
    }
  }
  if (n > 1)
    res.emplace_back(n, 1);
  return res;
}

// 約数列挙 O(√n)
vector<ll> divisors(ll n) {
  vector<ll> res;
  for (ll i = 1; i * i <= n; i++) {
    if (n % i == 0) {
      res.push_back(i);
      if (i != n / i)
        res.push_back(n / i);
    }
  }
  sort(all(res));
  return res;
}

// 拡張ユークリッドの互除法
// ax + by = gcd(a, b) を満たす (x, y) を求める
// 返り値: gcd(a, b)
ll extgcd(ll a, ll b, ll &x, ll &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  ll x1, y1;
  ll g = extgcd(b, a % b, x1, y1);
  x = y1;
  y = x1 - (a / b) * y1;
  return g;
}

// 組み合わせ計算のための前計算
struct combination {
  vector<ll> fact, ifact;
  ll mod;
  combination(int n, ll mod_ = MOD) : fact(n + 1), ifact(n + 1), mod(mod_) {
    fact[0] = 1;
    for (int i = 1; i <= n; ++i)
      fact[i] = fact[i - 1] * i % mod;
    ifact[n] = pow_mod<ll>(fact[n], mod - 2, mod);
    for (int i = n; i >= 1; --i)
      ifact[i - 1] = ifact[i] * i % mod;
  }
  ll operator()(int n, int k) const {
    if (k < 0 || k > n)
      return 0;
    return fact[n] % mod * ifact[k] % mod * ifact[n - k] % mod;
  }
  // 順列 P(n, k)
  ll perm(int n, int k) const {
    if (k < 0 || k > n)
      return 0;
    return fact[n] % mod * ifact[n - k] % mod;
  }
};
// 中国剰余定理 (CRT)
// x ≡ r1 (mod m1), x ≡ r2 (mod m2) を満たす x を求める
// 返り値: {r, m} (x ≡ r (mod m)), 解なしなら {0, -1}
pair<ll, ll> crt(ll r1, ll m1, ll r2, ll m2) {
  ll x, y;
  ll g = extgcd(m1, m2, x, y);
  if ((r2 - r1) % g != 0)
    return {0, -1};
  ll lcm = m1 / g * m2;
  ll r = (r1 + m1 % lcm * ((r2 - r1) / g % (m2 / g)) % lcm * x % lcm) % lcm;
  if (r < 0)
    r += lcm;
  return {r, lcm};
}

// floor_sum: sum_{i=0}^{n-1} floor((a*i + b) / m)
// ACL準拠の実装
ll floor_sum(ll n, ll m, ll a, ll b) {
  assert(n >= 0 && m >= 1);
  ll ans = 0;
  if (a >= m) {
    ans += n * (n - 1) / 2 * (a / m);
    a %= m;
  }
  if (b >= m) {
    ans += n * (b / m);
    b %= m;
  }
  ll y_max = (a * n + b) / m;
  ll x_max = y_max * m - b;
  if (y_max == 0)
    return ans;
  ans += (n - (x_max + a - 1) / a) * y_max;
  ans += floor_sum(y_max, a, m, (a - x_max % a) % a);
  return ans;
}

// オイラーのトーシェント関数 φ(n)
ll euler_totient(ll n) {
  ll res = n;
  for (ll i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      res -= res / i;
      while (n % i == 0)
        n /= i;
    }
  }
  if (n > 1)
    res -= res / n;
  return res;
}

// オイラーのトーシェント関数の篩 (1〜nのφを一括計算)
vector<ll> euler_totient_table(int n) {
  vector<ll> phi(n + 1);
  iota(all(phi), 0LL);
  for (int i = 2; i <= n; i++) {
    if (phi[i] == i) { // iは素数
      for (int j = i; j <= n; j += i) {
        phi[j] -= phi[j] / i;
      }
    }
  }
  return phi;
}
} // namespace math
// @end math

// @begin structure
// @exports BIT UnionFind WeightedUnionFind SegTree LazySegTree SparseTable
// データ構造
namespace structure {
// Binary Indexed Tree (Fenwick Tree)
template <typename T> struct BIT {
  int n;
  vector<T> dat;
  BIT(int n_) : n(n_), dat(n_ + 1, 0) {}

  // i番目(0-indexed)にxを加算
  void add(int i, T x) {
    for (++i; i <= n; i += i & -i)
      dat[i] += x;
  }

  // [0, i) の総和 (0-indexed)
  T sum(int i) const {
    T ret = 0;
    for (; i > 0; i -= i & -i)
      ret += dat[i];
    return ret;
  }

  // [l, r) の総和 (0-indexed)
  T sum(int l, int r) const { return sum(r) - sum(l); }

  // i番目の値を取得 (0-indexed)
  T get(int i) const { return sum(i + 1) - sum(i); }

  // i番目の値をxに更新 (0-indexed)
  void set(int i, T x) { add(i, x - get(i)); }
};

// Union-Find木
struct UnionFind {
  vector<int> d;
  UnionFind(int n = 0) : d(n, -1) {}
  int find(int x) {
    if (d[x] < 0)
      return x;
    return d[x] = find(d[x]);
  }
  bool unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y)
      return false;
    if (d[x] > d[y])
      swap(x, y);
    d[x] += d[y];
    d[y] = x;
    return true;
  }
  bool same(int x, int y) { return find(x) == find(y); }
  int size(int x) { return -d[find(x)]; }
};

// 重み付きUnion-Find木 (ポテンシャル付き)
// weight(y) - weight(x) = w という関係を管理
template <typename T> struct WeightedUnionFind {
  vector<int> d;
  vector<T> w;
  WeightedUnionFind(int n = 0) : d(n, -1), w(n, 0) {}

  int find(int x) {
    if (d[x] < 0)
      return x;
    int root = find(d[x]);
    w[x] += w[d[x]];
    return d[x] = root;
  }

  T weight(int x) {
    find(x);
    return w[x];
  }

  // weight(y) - weight(x) = cost となるように統合
  bool unite(int x, int y, T cost) {
    cost += weight(x) - weight(y);
    x = find(x);
    y = find(y);
    if (x == y)
      return false;
    if (d[x] > d[y]) {
      swap(x, y);
      cost = -cost;
    }
    d[x] += d[y];
    d[y] = x;
    w[y] = cost;
    return true;
  }

  bool same(int x, int y) { return find(x) == find(y); }
  int size(int x) { return -d[find(x)]; }

  // weight(y) - weight(x) を返す (同じ連結成分でなければ未定義)
  T diff(int x, int y) { return weight(y) - weight(x); }
};

// セグメント木
template <typename T> struct SegTree {
  using F = function<T(T, T)>;
  int n;
  vector<T> dat;
  T e;
  F f;

  SegTree(int n_, T e_, F f_) : e(e_), f(f_) {
    n = 1;
    while (n < n_)
      n *= 2;
    dat.assign(2 * n - 1, e);
  }

  void update(int k, T a) {
    k += n - 1;
    dat[k] = a;
    while (k > 0) {
      k = (k - 1) / 2;
      dat[k] = f(dat[k * 2 + 1], dat[k * 2 + 2]);
    }
  }

  T query(int a, int b, int k, int l, int r) {
    if (r <= a || b <= l)
      return e;
    if (a <= l && r <= b)
      return dat[k];
    T vl = query(a, b, k * 2 + 1, l, (l + r) / 2);
    T vr = query(a, b, k * 2 + 2, (l + r) / 2, r);
    return f(vl, vr);
  }

  T query(int a, int b) { return query(a, b, 0, 0, n); }
};

// 遅延伝播セグメント木
// T: データ型, U: 作用素型
// f: T × T → T (モノイドの演算)
// g: T × U → T (作用素の適用)
// h: U × U → U (作用素の合成)
// e: T の単位元
// id: U の単位元
template <typename T, typename U> struct LazySegTree {
  using FTT = function<T(T, T)>;
  using FTU = function<T(T, U)>;
  using FUU = function<U(U, U)>;
  int n;
  vector<T> dat;
  vector<U> laz;
  T e;
  U id;
  FTT f;
  FTU g;
  FUU h;

  LazySegTree(int n_, T e_, U id_, FTT f_, FTU g_, FUU h_)
      : e(e_), id(id_), f(f_), g(g_), h(h_) {
    n = 1;
    while (n < n_)
      n *= 2;
    dat.assign(2 * n - 1, e);
    laz.assign(2 * n - 1, id);
  }

  void push(int k) {
    if (laz[k] == id)
      return;
    dat[k * 2 + 1] = g(dat[k * 2 + 1], laz[k]);
    dat[k * 2 + 2] = g(dat[k * 2 + 2], laz[k]);
    laz[k * 2 + 1] = h(laz[k * 2 + 1], laz[k]);
    laz[k * 2 + 2] = h(laz[k * 2 + 2], laz[k]);
    laz[k] = id;
  }

  void update(int a, int b, U x, int k, int l, int r) {
    if (r <= a || b <= l)
      return;
    if (a <= l && r <= b) {
      dat[k] = g(dat[k], x);
      laz[k] = h(laz[k], x);
      return;
    }
    push(k);
    update(a, b, x, k * 2 + 1, l, (l + r) / 2);
    update(a, b, x, k * 2 + 2, (l + r) / 2, r);
    dat[k] = f(dat[k * 2 + 1], dat[k * 2 + 2]);
  }

  void update(int a, int b, U x) { update(a, b, x, 0, 0, n); }

  T query(int a, int b, int k, int l, int r) {
    if (r <= a || b <= l)
      return e;
    if (a <= l && r <= b)
      return dat[k];
    push(k);
    T vl = query(a, b, k * 2 + 1, l, (l + r) / 2);
    T vr = query(a, b, k * 2 + 2, (l + r) / 2, r);
    return f(vl, vr);
  }

  T query(int a, int b) { return query(a, b, 0, 0, n); }
};
// Sparse Table (静的RMQ, O(n)構築 O(1)クエリ)
template <typename T, typename F = function<T(T, T)>> struct SparseTable {
  vector<vector<T>> table;
  vector<int> log_table;
  F op;

  SparseTable() = default;
  SparseTable(const vector<T> &v, const F &op_) : op(op_) { build(v); }

  void build(const vector<T> &v) {
    int n = v.size();
    int k = 1;
    while ((1 << k) <= n)
      k++;
    table.assign(k, vector<T>(n));
    log_table.assign(n + 1, 0);
    for (int i = 2; i <= n; i++)
      log_table[i] = log_table[i / 2] + 1;
    table[0] = v;
    for (int j = 1; j < k; j++)
      for (int i = 0; i + (1 << j) <= n; i++)
        table[j][i] = op(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
  }

  // [l, r) のクエリ
  T query(int l, int r) const {
    int k = log_table[r - l];
    return op(table[k][l], table[k][r - (1 << k)]);
  }
};
} // namespace structure
// @end structure

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

// @begin RollingHash
// ローリングハッシュ
struct RollingHash {
  static const uint64_t mod = (1ull << 61ull) - 1;
  static vector<uint64_t> power;
  vector<uint64_t> hash;
  const uint64_t base;

  static uint64_t multiply(uint64_t a, uint64_t b) {
    uint64_t au = a >> 31, ad = a & ((1ull << 31) - 1);
    uint64_t bu = b >> 31, bd = b & ((1ull << 31) - 1);
    uint64_t mid = au * bd + ad * bu;
    uint64_t midu = mid >> 30, midd = mid & ((1ull << 30) - 1);
    return au * bu * 2 + midu + (midd << 31) + ad * bd;
  }

  static uint64_t calc_mod(uint64_t x) {
    uint64_t xu = x >> 61;
    uint64_t xd = x & mod;
    uint64_t res = xu + xd;
    if (res >= mod)
      res -= mod;
    return res;
  }

  RollingHash(const string &s, uint64_t base_ = 10007) : base(base_) {
    int n = s.size();
    hash.resize(n + 1, 0);
    power.resize(n + 1, 0);
    power[0] = 1;
    for (int i = 0; i < n; i++) {
      power[i + 1] = calc_mod(multiply(power[i], base));
      hash[i + 1] = calc_mod(multiply(hash[i], base) + s[i]);
    }
  }

  uint64_t get(int l, int r) const {
    uint64_t ret = mod + hash[r] - calc_mod(multiply(hash[l], power[r - l]));
    return calc_mod(ret);
  }

  bool match(int l1, int r1, int l2, int r2) const {
    return get(l1, r1) == get(l2, r2);
  }

  static uint64_t connect(uint64_t h1, uint64_t h2, int h2len) {
    return calc_mod(multiply(h1, power[h2len]) + h2);
  }
};

// 静的メンバ変数の定義
vector<uint64_t> RollingHash::power;
// @end RollingHash

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

// @begin graph
// @dep structure
// @exports dijkstra bfs warshall_floyd topological_sort kruskal LCA
// bellman_ford scc MaxFlow EulerTour グラフ関連
namespace graph {
// ダイクストラ法
vector<ll> dijkstra(const vector<vector<pair<int, ll>>> &g, int s) {
  int n = g.size();
  vector<ll> dist(n, LINF);
  priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> que;
  dist[s] = 0;
  que.push({0, s});
  while (!que.empty()) {
    auto [d, v] = que.top();
    que.pop();
    if (dist[v] < d)
      continue;
    for (auto [u, cost] : g[v]) {
      if (dist[u] > dist[v] + cost) {
        dist[u] = dist[v] + cost;
        que.push({dist[u], u});
      }
    }
  }
  return dist;
}

// BFS (重みなしグラフの最短距離)
vector<int> bfs(const vector<vector<int>> &g, int s) {
  int n = g.size();
  vector<int> dist(n, -1);
  queue<int> que;
  dist[s] = 0;
  que.push(s);
  while (!que.empty()) {
    int v = que.front();
    que.pop();
    for (int u : g[v]) {
      if (dist[u] != -1)
        continue;
      dist[u] = dist[v] + 1;
      que.push(u);
    }
  }
  return dist;
}

// ワーシャルフロイド法 (全頂点間最短距離)
// dist[i][j] = i→j の距離 (辺がなければ LINF)
// 負閉路が存在する場合 dist[i][i] < 0 となる頂点がある
void warshall_floyd(vector<vector<ll>> &dist) {
  int n = dist.size();
  rep(k, n) rep(i, n) rep(j, n) {
    if (dist[i][k] == LINF || dist[k][j] == LINF)
      continue;
    chmin(dist[i][j], dist[i][k] + dist[k][j]);
  }
}

// トポロジカルソート (BFS, Kahn's algorithm)
// 返り値: トポロジカル順序の頂点列 (DAGでなければ空を返す)
vector<int> topological_sort(const vector<vector<int>> &g) {
  int n = g.size();
  vector<int> indeg(n, 0);
  for (int v = 0; v < n; v++)
    for (int u : g[v])
      indeg[u]++;
  queue<int> que;
  for (int i = 0; i < n; i++)
    if (indeg[i] == 0)
      que.push(i);
  vector<int> order;
  while (!que.empty()) {
    int v = que.front();
    que.pop();
    order.push_back(v);
    for (int u : g[v])
      if (--indeg[u] == 0)
        que.push(u);
  }
  if ((int)order.size() != n)
    return {}; // DAGでない
  return order;
}

// クラスカル法 (最小全域木)
// 辺: {コスト, {頂点u, 頂点v}}
// 返り値: {最小全域木のコスト, 使った辺のリスト}
pair<ll, vector<pair<ll, pii>>> kruskal(int n, vector<pair<ll, pii>> &edges) {
  sort(all(edges));
  structure::UnionFind uf(n);
  ll total = 0;
  vector<pair<ll, pii>> used;
  for (auto &[cost, uv] : edges) {
    auto [u, v] = uv;
    if (uf.unite(u, v)) {
      total += cost;
      used.push_back({cost, {u, v}});
    }
  }
  return {total, used};
}

// LCA (最小共通祖先) - ダブリング
struct LCA {
  int n, LOG;
  vector<vector<int>> parent;
  vector<int> depth;

  LCA(const vector<vector<int>> &g, int root = 0) : n(g.size()), LOG(1) {
    while ((1 << LOG) < n)
      LOG++;
    parent.assign(LOG, vector<int>(n, -1));
    depth.assign(n, 0);
    // BFSで深さと親を求める
    queue<int> que;
    depth[root] = 0;
    parent[0][root] = root;
    que.push(root);
    vector<bool> visited(n, false);
    visited[root] = true;
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (int u : g[v]) {
        if (visited[u])
          continue;
        visited[u] = true;
        depth[u] = depth[v] + 1;
        parent[0][u] = v;
        que.push(u);
      }
    }
    // ダブリングテーブル構築
    for (int k = 0; k + 1 < LOG; k++)
      for (int v = 0; v < n; v++)
        if (parent[k][v] != -1)
          parent[k + 1][v] = parent[k][parent[k][v]];
  }

  int lca(int u, int v) const {
    if (depth[u] < depth[v])
      swap(u, v);
    // 深さを揃える
    int diff = depth[u] - depth[v];
    for (int k = 0; k < LOG; k++)
      if ((diff >> k) & 1)
        u = parent[k][u];
    if (u == v)
      return u;
    // 同時に登る
    for (int k = LOG - 1; k >= 0; k--) {
      if (parent[k][u] != parent[k][v]) {
        u = parent[k][u];
        v = parent[k][v];
      }
    }
    return parent[0][u];
  }

  int dist(int u, int v) const {
    return depth[u] + depth[v] - 2 * depth[lca(u, v)];
  }
};
// ベルマンフォード法 (負辺対応最短路)
// 負閉路で到達可能な頂点の距離は -LINF になる
vector<ll> bellman_ford(int n, const vector<tuple<int, int, ll>> &edges,
                        int s) {
  vector<ll> dist(n, LINF);
  dist[s] = 0;
  for (int i = 0; i < n - 1; i++) {
    for (auto &[u, v, w] : edges) {
      if (dist[u] != LINF && dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
      }
    }
  }
  // 負閉路検出: もう一度 n-1 回回して更新があれば負閉路
  for (int i = 0; i < n - 1; i++) {
    for (auto &[u, v, w] : edges) {
      if (dist[u] != LINF && dist[u] + w < dist[v]) {
        dist[v] = -LINF;
      }
    }
  }
  return dist;
}

// 強連結成分分解 (SCC) - Kosaraju's algorithm
// 返り値: comp[v] = 頂点vが属するSCC番号 (トポロジカル順)
vector<int> scc(const vector<vector<int>> &g) {
  int n = g.size();
  vector<vector<int>> rg(n);
  for (int v = 0; v < n; v++)
    for (int u : g[v])
      rg[u].push_back(v);

  vector<int> order, comp(n, -1);
  vector<bool> visited(n, false);

  // 1回目のDFS: 帰りがけ順を記録
  auto dfs1 = [&](auto &self, int v) -> void {
    visited[v] = true;
    for (int u : g[v])
      if (!visited[u])
        self(self, u);
    order.push_back(v);
  };
  for (int i = 0; i < n; i++)
    if (!visited[i])
      dfs1(dfs1, i);

  // 2回目のDFS: 逆グラフ上で帰りがけ順の逆順に探索
  int cnt = 0;
  auto dfs2 = [&](auto &self, int v, int c) -> void {
    comp[v] = c;
    for (int u : rg[v])
      if (comp[u] == -1)
        self(self, u, c);
  };
  for (int i = n - 1; i >= 0; i--)
    if (comp[order[i]] == -1)
      dfs2(dfs2, order[i], cnt++);

  return comp;
}

// 最大流 (Dinic's algorithm)
struct MaxFlow {
  struct Edge {
    int to, rev;
    ll cap;
  };

  int n;
  vector<vector<Edge>> graph;
  vector<int> level, iter;

  MaxFlow(int n_) : n(n_), graph(n_), level(n_), iter(n_) {}

  void add_edge(int from, int to, ll cap) {
    graph[from].push_back({to, (int)graph[to].size(), cap});
    graph[to].push_back({from, (int)graph[from].size() - 1, 0});
  }

  bool bfs(int s, int t) {
    fill(all(level), -1);
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (auto &e : graph[v]) {
        if (e.cap > 0 && level[e.to] < 0) {
          level[e.to] = level[v] + 1;
          que.push(e.to);
        }
      }
    }
    return level[t] >= 0;
  }

  ll dfs(int v, int t, ll f) {
    if (v == t)
      return f;
    for (int &i = iter[v]; i < (int)graph[v].size(); i++) {
      Edge &e = graph[v][i];
      if (e.cap > 0 && level[v] < level[e.to]) {
        ll d = dfs(e.to, t, min(f, e.cap));
        if (d > 0) {
          e.cap -= d;
          graph[e.to][e.rev].cap += d;
          return d;
        }
      }
    }
    return 0;
  }

  ll max_flow(int s, int t) {
    ll flow = 0;
    while (bfs(s, t)) {
      fill(all(iter), 0);
      ll d;
      while ((d = dfs(s, t, LINF)) > 0)
        flow += d;
    }
    return flow;
  }
};

// オイラーツアー (部分木クエリ用)
struct EulerTour {
  vector<int> in, out;
  int timer;

  EulerTour(const vector<vector<int>> &g, int root = 0)
      : in(g.size()), out(g.size()), timer(0) {
    dfs(g, root, -1);
  }

  void dfs(const vector<vector<int>> &g, int v, int p) {
    in[v] = timer++;
    for (int u : g[v])
      if (u != p)
        dfs(g, u, v);
    out[v] = timer;
  }

  // 頂点vの部分木は [in[v], out[v]) に対応
  bool is_ancestor(int u, int v) const {
    return in[u] <= in[v] && out[v] <= out[u];
  }
};
} // namespace graph
// @end graph

// メイン関数
#ifndef TESTING
int main() {
  fastio();

  return 0;
}
#endif
