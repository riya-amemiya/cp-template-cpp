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
