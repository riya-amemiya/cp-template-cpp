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

  // prefix sum >= x となる最小の 0-indexed i。全体和 < x なら n
  int lower_bound(T x) const {
    if (x <= 0)
      return 0;
    int i = 0;
    int k = 1;
    while ((k << 1) <= n)
      k <<= 1;
    T s = 0;
    for (; k > 0; k >>= 1) {
      if (i + k <= n && s + dat[i + k] < x) {
        s += dat[i + k];
        i += k;
      }
    }
    return i;
  }
};

// Union-Find木
struct UnionFind {
  vector<int> d;
  UnionFind(int n = 0) : d(n, -1) {}
  int find(int x) {
    int r = x;
    while (d[r] >= 0)
      r = d[r];
    while (d[x] >= 0) {
      int p = d[x];
      d[x] = r;
      x = p;
    }
    return r;
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

// セグメント木 (非再帰, ACL と同じ 2n レイアウト)
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
    dat.assign(2 * n, e);
  }

  SegTree(const vector<T> &v, T e_, F f_) : e(e_), f(f_) {
    int n_ = (int)v.size();
    n = 1;
    while (n < n_)
      n *= 2;
    dat.assign(2 * n, e);
    for (int i = 0; i < n_; i++)
      dat[n + i] = v[i];
    for (int i = n - 1; i >= 1; i--)
      dat[i] = f(dat[i << 1], dat[i << 1 | 1]);
  }

  void update(int k, T a) {
    k += n;
    dat[k] = a;
    for (k >>= 1; k; k >>= 1)
      dat[k] = f(dat[k << 1], dat[k << 1 | 1]);
  }

  T get(int k) const { return dat[k + n]; }

  T query(int a, int b) {
    T vl = e, vr = e;
    a += n;
    b += n;
    while (a < b) {
      if (a & 1)
        vl = f(vl, dat[a++]);
      if (b & 1)
        vr = f(dat[--b], vr);
      a >>= 1;
      b >>= 1;
    }
    return f(vl, vr);
  }
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
  int n, size, log;
  vector<T> dat;
  vector<U> laz;
  T e;
  U id;
  FTT f;
  FTU g;
  FUU h;

  LazySegTree(int n_, T e_, U id_, FTT f_, FTU g_, FUU h_)
      : n(n_), e(e_), id(id_), f(f_), g(g_), h(h_) {
    size = 1;
    log = 0;
    while (size < n_) {
      size <<= 1;
      log++;
    }
    dat.assign(2 * size, e);
    laz.assign(size, id);
  }

  void update_node(int k) { dat[k] = f(dat[k << 1], dat[k << 1 | 1]); }

  void all_apply(int k, U x) {
    dat[k] = g(dat[k], x);
    if (k < size)
      laz[k] = h(laz[k], x);
  }

  void push(int k) {
    if (laz[k] == id)
      return;
    all_apply(k << 1, laz[k]);
    all_apply(k << 1 | 1, laz[k]);
    laz[k] = id;
  }

  void update(int a, int b, U x) {
    if (a == b)
      return;
    a += size;
    b += size;
    for (int i = log; i >= 1; i--) {
      if (((a >> i) << i) != a)
        push(a >> i);
      if (((b >> i) << i) != b)
        push((b - 1) >> i);
    }
    int l2 = a, r2 = b;
    while (a < b) {
      if (a & 1)
        all_apply(a++, x);
      if (b & 1)
        all_apply(--b, x);
      a >>= 1;
      b >>= 1;
    }
    for (int i = 1; i <= log; i++) {
      if (((l2 >> i) << i) != l2)
        update_node(l2 >> i);
      if (((r2 >> i) << i) != r2)
        update_node((r2 - 1) >> i);
    }
  }

  T query(int a, int b) {
    if (a == b)
      return e;
    a += size;
    b += size;
    for (int i = log; i >= 1; i--) {
      if (((a >> i) << i) != a)
        push(a >> i);
      if (((b >> i) << i) != b)
        push((b - 1) >> i);
    }
    T vl = e, vr = e;
    while (a < b) {
      if (a & 1)
        vl = f(vl, dat[a++]);
      if (b & 1)
        vr = f(dat[--b], vr);
      a >>= 1;
      b >>= 1;
    }
    return f(vl, vr);
  }
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
