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
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rep1(i, n) for (int i = 1; i <= (n); ++i)
#define rrep(i, n) for (int i = (n) - 1; i >= 0; --i)
#define fore(i, a) for (auto &i : a)
#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define fi first
#define se second
#define endl '\n'

// デバッグマクロ
#ifdef LOCAL
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &p) {
  return os << "(" << p.first << ", " << p.second << ")";
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  os << "[";
  for (int i = 0; i < (int)v.size(); i++) {
    if (i) os << ", ";
    os << v[i];
  }
  return os << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &s) {
  os << "{";
  bool first = true;
  for (const auto &x : s) {
    if (!first) os << ", ";
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
    if (!first) os << ", ";
    first = false;
    os << k << ": " << v;
  }
  return os << "}";
}
void debug_out() { cerr << '\n'; }
template <typename Head, typename... Tail>
void debug_out(Head H, Tail... T) {
  cerr << " " << H;
  if constexpr (sizeof...(T) > 0) cerr << ",";
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
  for (auto &x : v) cin >> x;
  return v;
}
template <typename T> vector<vector<T>> input_vec2(int n, int m) {
  vector<vector<T>> v(n, vector<T>(m));
  for (auto &row : v)
    for (auto &x : row) cin >> x;
  return v;
}

// 出力ヘルパー
template <typename T> void print_vec(const vector<T> &v, const string &sep = " ") {
  for (int i = 0; i < (int)v.size(); i++) {
    if (i) cout << sep;
    cout << v[i];
  }
  cout << '\n';
}

// 2次元ベクトルの初期化
template <typename T> vector<vector<T>> vv(int n, int m, T x = T()) {
  return vector<vector<T>>(n, vector<T>(m, x));
}

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

// 数学関連の関数群
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
    if (is_p[i]) primes.push_back(i);
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
  if (n > 1) res.emplace_back(n, 1);
  return res;
}

// 約数列挙 O(√n)
vector<ll> divisors(ll n) {
  vector<ll> res;
  for (ll i = 1; i * i <= n; i++) {
    if (n % i == 0) {
      res.push_back(i);
      if (i != n / i) res.push_back(n / i);
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
} // namespace math

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
    if (d[x] < 0) return x;
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
    if (x == y) return false;
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
    while (n < n_) n *= 2;
    dat.assign(2 * n - 1, e);
    laz.assign(2 * n - 1, id);
  }

  void push(int k) {
    if (laz[k] == id) return;
    dat[k * 2 + 1] = g(dat[k * 2 + 1], laz[k]);
    dat[k * 2 + 2] = g(dat[k * 2 + 2], laz[k]);
    laz[k * 2 + 1] = h(laz[k * 2 + 1], laz[k]);
    laz[k * 2 + 2] = h(laz[k * 2 + 2], laz[k]);
    laz[k] = id;
  }

  void update(int a, int b, U x, int k, int l, int r) {
    if (r <= a || b <= l) return;
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
    if (r <= a || b <= l) return e;
    if (a <= l && r <= b) return dat[k];
    push(k);
    T vl = query(a, b, k * 2 + 1, l, (l + r) / 2);
    T vr = query(a, b, k * 2 + 2, (l + r) / 2, r);
    return f(vl, vr);
  }

  T query(int a, int b) { return query(a, b, 0, 0, n); }
};
} // namespace structure

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

// グラフ関連
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
      if (dist[u] != -1) continue;
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
    if (dist[i][k] == LINF || dist[k][j] == LINF) continue;
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
    if (indeg[i] == 0) que.push(i);
  vector<int> order;
  while (!que.empty()) {
    int v = que.front();
    que.pop();
    order.push_back(v);
    for (int u : g[v])
      if (--indeg[u] == 0) que.push(u);
  }
  if ((int)order.size() != n) return {}; // DAGでない
  return order;
}

// クラスカル法 (最小全域木)
// 辺: {コスト, {頂点u, 頂点v}}
// 返り値: {最小全域木のコスト, 使った辺のリスト}
pair<ll, vector<pair<ll, pii>>>
kruskal(int n, vector<pair<ll, pii>> &edges) {
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

  LCA(const vector<vector<int>> &g, int root = 0)
      : n(g.size()), LOG(1) {
    while ((1 << LOG) < n) LOG++;
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
        if (visited[u]) continue;
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
    if (depth[u] < depth[v]) swap(u, v);
    // 深さを揃える
    int diff = depth[u] - depth[v];
    for (int k = 0; k < LOG; k++)
      if ((diff >> k) & 1) u = parent[k][u];
    if (u == v) return u;
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
} // namespace graph

// メイン関数
int main() {
  fastio();

  return 0;
}
