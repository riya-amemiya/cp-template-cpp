# cp-template-cpp

C++23 競技プログラミングテンプレート (AtCoder 等向け)

提出時は次のコマンドで未使用宣言を削る。

```bash
python3 tools/strip.py > submission.cpp
python3 tools/strip.py solution.cpp -o submission.cpp
```

空の `main` ならテンプレート全体から約 98% 削減される。解答が参照している `using` / マクロ / 関数 / struct だけが残る。

---

## 目次

- [提出用ストリッパー](#提出用ストリッパー)
- [型エイリアス](#型エイリアス)
- [定数](#定数)
- [マクロ](#マクロ)
- [ユーティリティ関数](#ユーティリティ関数)
- [modint](#modint)
- [math 名前空間](#math-名前空間)
- [structure 名前空間](#structure-名前空間)
- [binary\_search 名前空間](#binary_search-名前空間)
- [座標圧縮 Compress](#座標圧縮-compress)
- [累積和](#累積和)
- [行列 Matrix](#行列-matrix)
- [ローリングハッシュ RollingHash](#ローリングハッシュ-rollinghash)
- [文字列アルゴリズム](#文字列アルゴリズム)
- [graph 名前空間](#graph-名前空間)
- [依存関係マップ](#依存関係マップ)

---

## 提出用ストリッパー

`tools/strip.py` は C++ のトップレベル宣言（namespace 内を含む）を走査し、`main` から到達しない識別子を削除する。

| 入力 | 動作 |
| --- | --- |
| 引数なし | プロジェクトの `main.cpp` を削減 |
| `#include` または `int main` があるファイル | そのファイルを削減 |
| 断片 (本体だけ) | `main.cpp` の `fastio();` の直後に挿入してから削減 |

```bash
python3 tools/strip.py -o submission.cpp
```

---

## 型エイリアス

| エイリアス | 型 |
| --- | --- |
| `ll` | `long long` |
| `ld` | `long double` |
| `ull` | `unsigned long long` |
| `pii` | `pair<int, int>` |
| `pll` | `pair<ll, ll>` |
| `vi` | `vector<int>` |
| `vl` | `vector<ll>` |
| `vvi` | `vector<vector<int>>` |
| `vvl` | `vector<vector<ll>>` |
| `vb` | `vector<bool>` |
| `vvb` | `vector<vector<bool>>` |
| `vs` | `vector<string>` |
| `vpii` | `vector<pii>` |
| `vpll` | `vector<pll>` |
| `vld` | `vector<ld>` |
| `vull` | `vector<ull>` |

---

## 定数

| 定数 | 値 | 用途 |
| --- | --- | --- |
| `INF` | `1e9` | int の無限大として未到達の初期値に使う |
| `LINF` | `1e18` | ll の無限大として未到達の初期値に使う |
| `MOD` | `1e9 + 7` | 答えを mod で求める問題の定番素数 |
| `MOD998` | `998244353` | NTT フレンドリーな素数。問題文に指定がある場合に使う |
| `PI` | `M_PI` | 幾何や三角関数を使う問題 |
| `EPS` | `1e-9` | 実数比較の誤差吸収 (`abs(a-b) < EPS` で等値判定) |

### グリッド方向配列

| 配列 | 内容 | 使う場面 |
| --- | --- | --- |
| `dx4`, `dy4` | 上下左右 4 方向 | 迷路、BFS、島の数え上げなど、移動が 4 方向の問題 |
| `dx8`, `dy8` | 斜め込み 8 方向 | チェスのキングの移動や、斜め移動が許可される問題 |

---

## マクロ

### ループ

| マクロ | 展開 | 使う場面 |
| --- | --- | --- |
| `rep(i, n)` | `for (int i = 0; i < n; ++i)` | 0-indexed の基本ループ |
| `rep1(i, n)` | `for (int i = 1; i <= n; ++i)` | 1-indexed でループしたい時 |
| `rep2(i, a, b)` | `for (int i = a; i < b; ++i)` | 区間 [a, b) をループしたい時 |
| `rrep(i, n)` | `for (int i = n-1; i >= 0; --i)` | 後ろから走査、DP の逆順更新 |
| `rrep1(i, n)` | `for (int i = n; i >= 1; --i)` | 1-indexed で後ろから走査したい時 |
| `fore(i, a)` | `for (auto& i : a)` | コンテナの全要素を参照で走査 |

### コンテナ操作

| マクロ | 展開 |
| --- | --- |
| `all(v)` | `v.begin(), v.end()` |
| `rall(v)` | `v.rbegin(), v.rend()` |
| `sz(x)` | `((int)(x).size())` |
| `pb` | `push_back` |
| `eb` | `emplace_back` |
| `mp` | `make_pair` |
| `fi` | `first` |
| `se` | `second` |
| `Sort(v)` | `sort(all(v))` |
| `Reverse(v)` | `reverse(all(v))` |
| `Unique(v)` | `v.erase(unique(all(v)), v.end())` |

### デバッグ

コンパイル時に `-DLOCAL` を付けると有効になり、本番提出時は自動で無効化される。
`pair`, `vector`, `set`, `map`, `multiset`, `deque` をそのまま `cerr` に流せる。

```cpp
debug(x, y, z);          // [x, y, z]: 1, 2, 3
debug(v);                 // [v]: [1, 2, 3]
debug(mp(1, "abc"));     // [mp(1, "abc")]: (1, abc)
```

---

## ユーティリティ関数

### chmin / chmax

DP の遷移でほぼ毎回使う。「更新が発生したか」を返すので条件分岐にも使える。

```cpp
chmin(dp[i], dp[j] + cost);  // dp[i] = min(dp[i], dp[j] + cost)
chmax(ans, val);
```

> **使う場面**: DP の最小/最大値更新、グラフの距離更新

### fastio

入力が大量にある問題では必ず冒頭で呼ぶ。`cin` を 1MiB バッファのカスタム streambuf に付け替える。`endl` をマクロで `'\n'` に置き換えているので合わせて使う。
`cout` には `fixed` と `setprecision(20)` も設定するので、浮動小数点数は小数点以下 20 桁で出力される。

```cpp
fastio();
```

> **注意**: `fastio()` 後は `scanf`/`printf` と混在させないこと

### 入力ヘルパー

```cpp
auto x = input<ll>();               // 1 値読み込み
auto v = input_vec<ll>(n);          // n 要素の 1 次元配列
auto g = input_vec2<ll>(n, m);      // n×m の 2 次元配列
```

次のマクロは変数を宣言してそのまま `cin` で読み込む。

| マクロ | 宣言 |
| --- | --- |
| `INT(a, b)` | `int a, b;` |
| `LL(a, b)` | `ll a, b;` |
| `STR(s)` | `string s;` |
| `CHR(c)` | `char c;` |
| `DBL(x)` | `double x;` |
| `VEC(type, name, n)` | `vector<type> name(n);` |
| `VEC2(type, name, h, w)` | `vector<vector<type>> name(h, vector<type>(w));` |

### 出力ヘルパー

```cpp
print_vec(v);           // スペース区切りで出力 + 改行
print_vec(v, "\n");     // 1 行 1 要素で出力したい時
```

`Yes(cond)` は `cond` が true なら `Yes`、false なら `No` を改行付きで出力する。
`No()` は `No` を出力する。
`YES(cond)` と `NO()` は同じ動作で、`YES` と `NO` を出力する。

### 2 次元ベクトル初期化

```cpp
auto dp = vv<ll>(n, m, LINF);   // n×m の DP テーブルを LINF で初期化
auto g  = vv<int>(n, n, INF);   // グラフの距離行列を INF で初期化
```

### argsort / RLE / グリッド回転

```cpp
auto idx = argsort(a);          // a[idx[0]] <= a[idx[1]] <= ... (安定)
auto enc = rle(a);              // {(値, 長さ), ...}
auto encs = rle(s);             // 文字列にも使える
auto b = rotate90(grid);        // 時計回り 90°
```

---

## modint

「答えを `1e9+7` で割った余りを求めよ」という問題で使う。
通常の整数型と同じ感覚で四則演算が書けるため、mod 計算のバグを防げる。

| 型 | MOD | 使う場面 |
| --- | --- | --- |
| `mint` | `1e9 + 7` | 問題文に `mod 10^9+7` と書かれている場合 |
| `mint998` | `998244353` | 問題文に `mod 998244353` と書かれている場合 |

四則演算 (`+`, `-`, `*`, `/`)、累乗 (`.pow(n)`)、逆元 (`.inv()`)、`cin`/`cout` に対応。

```cpp
mint a = 3, b = 5;
mint c = a * b + a.pow(10);   // 自動で mod が掛かる
cout << c << "\n";
cin >> a;                      // そのまま入力も受け取れる
```

> **使う場面**: 経路数や場合の数の DP、組み合わせ数の計算、行列累乗

---

## math 名前空間

### pow_mod

MOD が固定でない場合や ll のまま扱いたい場合など、`modint` が使えない場面で使う。
途中の乗算は `__int128` で行う。
`math::mul_mod(a, b, m)` は同じ方法で `a * b mod m` を返す。

```cpp
ll r = math::pow_mod(a, n, m);  // a^n mod m、O(log n)
```

> **使う場面**: MOD が問題ごとに異なる場合、フェルマーの小定理による逆元計算

### is_prime

1 つの数が素数か判定するだけでよい場合に使う。大量に判定するなら `sieve` の方が速い。
判定は 64bit 決定的 Miller-Rabin で、本体は `math::miller_rabin(n)` にある。

```cpp
bool ok = math::is_prime(n);  // O(log n)
```

> **使う場面**: 素数に関する条件分岐、素数のみカウントする問題

### sieve / prime_list

ある上限以下の素数を列挙したい時に使う。

```cpp
auto is_p = math::sieve(n);       // is_p[i] == true なら i は素数
auto ps   = math::prime_list(n);  // n 以下の素数一覧
```

> **使う場面**: 複数の数について素数判定が必要な場合、素数の個数を数える問題
> **計算量**: O(n) (どちらも `sieve_spf` の線形篩を使う)

### factorize

```cpp
auto f = math::factorize(12);
// f = {(2, 2), (3, 1)}  → 12 = 2^2 × 3^1
```

> **使う場面**: 素因数の個数や種類を使う問題、約数の個数を求める問題（指数に +1 して掛け合わせる）
> **計算量**: 1e6 までの因数は試し割りで取り、残りは Pollard's Rho で分解する（n ≤ 1e18）

`math::pollard(n)` は、合成数 n の 1 と n 以外の約数を 1 つ返す補助関数。

### divisors

```cpp
auto d = math::divisors(12);  // d = {1, 2, 3, 4, 6, 12}
```

> **使う場面**: 約数全列挙、「n の約数についてすべて〜」という問題
> **計算量**: O(√n)

### extgcd

`ax + by = gcd(a, b)` を満たす整数 `x`, `y` を求める。

```cpp
ll x, y;
ll g = math::extgcd(a, b, x, y);  // g = gcd(a, b)
```

> **使う場面**: MOD が素数でない場合の逆元、中国剰余定理 (CRT)、線形ディオファントス方程式

### combination

nCk や nPk を大量に求める場合に前計算しておく。

```cpp
math::combination C(200000);  // 最大 n まで前計算 (O(n))
ll c = C(n, k);               // nCk mod MOD、O(1)
ll p = C.perm(n, k);          // nPk mod MOD、O(1)
```

> **使う場面**: 場合の数の DP、確率や期待値の問題、二項係数を何度も使う問題
> **注意**: `MOD` がデフォルト。問題によって `combination C(n, MOD998)` と指定可能

### crt

`x ≡ r1 (mod m1)` と `x ≡ r2 (mod m2)` を同時に満たす x を求める。
返り値は `{r, m}` で、`x ≡ r (mod m)` を表す。m は `lcm(m1, m2)` で、解がなければ `{0, -1}` を返す。

```cpp
auto [r, m] = math::crt(r1, m1, r2, m2);
```

### floor_sum

`sum_{i=0}^{n-1} floor((a*i + b) / m)` を求める。
`n >= 0` と `m >= 1` は assert で確かめる。負の `a` と `b` は正規化しないので、0 以上の値を渡す。

```cpp
ll s = math::floor_sum(n, m, a, b);
```

### euler_totient / euler_totient_table

`euler_totient(n)` はオイラーのトーシェント関数 φ(n) を O(√n) で返す。
`euler_totient_table(n)` は 0 から n までの φ を篩でまとめて求め、O(n log log n) で返す。

```cpp
ll phi = math::euler_totient(n);
auto table = math::euler_totient_table(n);
```

### ceil_div / floor_div

負数でも数学的な天井と床になる除算。`a / b` のゼロ方向切り捨てとは違う。

```cpp
ll x = math::ceil_div(10, 3);   // 4
ll y = math::floor_div(-10, 3); // -4
```

### sieve_spf

線形篩で最小素因数テーブルを O(n) で作る。多数の整数を素因数分解するときに使う。

```cpp
auto spf = math::sieve_spf(n);
int p = spf[x];  // x の最小素因数
```

---

## structure 名前空間

### BIT (Binary Indexed Tree / Fenwick Tree)

1 点加算と区間和を O(log n) で処理するデータ構造。SegTree より定数が小さく速い。

```cpp
structure::BIT<ll> bit(n);
bit.add(i, x);       // i 番目 (0-indexed) に x を加算
bit.sum(l, r);       // [l, r) の総和
bit.get(i);          // i 番目の値を取得
bit.set(i, x);       // i 番目の値を x に更新
bit.lower_bound(x);  // prefix sum >= x となる最小の i。無ければ n
```

> **使う場面**:
>
> - 転倒数の計算（自分より左にある自分より大きい要素の数）
> - 動的に更新しながら区間和を求める
> - Compress と組み合わせて座標圧縮後の BIT
>
> **計算量**: 更新 O(log n)、クエリ O(log n)

### UnionFind

グループの統合・判定を高速に行うデータ構造。

```cpp
structure::UnionFind uf(n);
uf.unite(x, y);  // x と y を同じグループに統合。統合済みなら false
uf.same(x, y);   // 同じグループか判定
uf.find(x);      // x の根（代表元）を返す
uf.size(x);      // x が属するグループのサイズ
```

> **使う場面**:
>
> - グラフの連結成分判定
> - クエリを後ろから処理して辺を追加していく（オフライン処理）
> - Kruskal 法（MST）の補助
> - 「グループが何個あるか」「同じグループか」の問い合わせが多い問題
>
> **計算量**: ほぼ O(1) (アッカーマン逆関数)

### WeightedUnionFind

「ノード間の重みの差」を管理できる UnionFind。

```cpp
structure::WeightedUnionFind<ll> wuf(n);
wuf.unite(x, y, cost);  // weight(y) - weight(x) = cost となるよう統合
wuf.diff(x, y);         // weight(y) - weight(x) を返す（同一成分のみ有効）
wuf.same(x, y);         // 同じ連結成分か
wuf.weight(x);          // 根を基準にした x の重み
wuf.size(x);            // x が属する連結成分のサイズ
```

> **使う場面**:
>
> - 相対的な差が与えられる問題（「A は B より 3 大きい」等）
> - 差分制約問題のうち単純なもの
> - 同グループ内での値の大小・差を管理する必要がある問題

### SegTree (セグメント木)

任意のモノイド演算について区間クエリ・点更新を行う。

```cpp
// 例1: 区間最小値 (RMQ)
structure::SegTree<ll> seg(n, LINF, [](ll a, ll b){ return min(a, b); });
// 例2: 区間和
structure::SegTree<ll> seg(n, 0LL, [](ll a, ll b){ return a + b; });
// 例3: 区間最大値
structure::SegTree<ll> seg(n, -LINF, [](ll a, ll b){ return max(a, b); });
structure::SegTree<ll> seg(v, 0LL, [](ll a, ll b){ return a + b; }); // 配列から構築

seg.update(i, val);  // i 番目を val に更新 (0-indexed)
seg.query(l, r);     // [l, r) の演算結果
seg.get(i);          // i 番目の値
```

> **使う場面**:
>
> - 点更新 + 区間クエリが必要な問題
> - BIT で扱えない演算（最大、最小、GCD など）
> - 「左から i 番目の要素を更新しながら区間の最大値を求める」
>
> **計算量**: 更新 O(log n)、クエリ O(log n)

### LazySegTree (遅延伝播セグメント木)

区間更新と区間クエリを両方扱える。SegTree の上位互換だが設定が複雑。

```cpp
// 例: 区間加算 + 区間最小値クエリ
// f: min 演算、g: 加算を適用、h: 作用素の合成（加算同士は足し算）
structure::LazySegTree<ll, ll> seg(
    n, LINF, 0LL,
    [](ll a, ll b){ return min(a, b); },  // f
    [](ll a, ll u){ return a + u; },      // g
    [](ll a, ll b){ return a + b; }       // h
);
seg.update(l, r, x);  // [l, r) の全要素に x を加算
seg.query(l, r);      // [l, r) の最小値
```

> **使う場面**:
>
> - 「区間に一様加算して区間最小/最大/和を求める」
> - 「区間を一様に書き換えて区間和を求める」
> - SegTree で TLE する場合（区間更新を O(n) でやっている場合）
>
> **計算量**: 更新 O(log n)、クエリ O(log n)

### SparseTable

更新のない配列について、区間 [l, r) の演算結果を O(1) で求める。
区間を重ねて計算するので、min、max、gcd のように同じ要素を 2 回使っても結果が変わらない演算に限る。
`l < r` の区間を渡す。

```cpp
structure::SparseTable<ll> st(v, [](ll a, ll b){ return min(a, b); });
st.query(l, r);
```

> **計算量**: 構築 O(n log n)、クエリ O(1)

---

## binary_search 名前空間

「条件を満たす最大/最小の値を求めよ」という問題で使う。
条件が単調（ある値以下は全部 OK、以上は全部 NG など）であれば適用可能。

```cpp
// 「f(x) == true となる最大の整数 x」を求める (ok > ng なら最小の x)
// ok: true になることが分かっている値、ng: false になることが分かっている値
ll ans = binary_search::binary_search_left(ok, ng, [&](ll mid){
    return check(mid);  // mid が条件を満たすなら true
});

// 「f(x) == false となる最小の整数 x」を求める (ng > ok なら最大の x)
// ng: true になることが分かっている値、ok: false になることが分かっている値
ll ans = binary_search::binary_search_right(ng, ok, [&](ll mid){
    return check(mid);
});

// 実数二分探索（デフォルト 100 回反復）。f(left) == false、f(right) == true として true 側の境界を返す
double ans = binary_search::binary_search_real(0.0, 1e9, [&](double mid){
    return check(mid);
});
```

> **使う場面**:
>
> - 「最大化せよ / 最小化せよ」という問題で答えを二分探索（パラメータ探索）
> - 「コスト X 以内で条件を達成できるか？」という判定問題に落とす
> - `lower_bound` / `upper_bound` を自前条件で書きたい場合
> - 実数解を求める問題（最小コスト・最適化）

---

## 座標圧縮 Compress

値の種類数は少ないが値そのものが大きい場合に、0-indexed の小さい整数に変換する。
BIT や SegTree のインデックスとして使うのが典型。

```cpp
Compress<ll> comp;
comp.add(v);      // vector の全要素を登録
comp.add(x);      // 1 要素を追加
comp.build();     // ソートと重複除去（add 後に必ず呼ぶ）

int idx = comp.get(x);   // x の圧縮後インデックス (0-indexed)
int sz  = comp.size();   // 圧縮後の種類数
ll val  = comp[i];       // 元の i 番目の値（逆引き）
```

`Compress<ll> comp(v);` と書くと、構築時に `v` の全要素を登録する。この場合も `build()` は呼ぶ。

> **使う場面**:
>
> - 座標値が最大 `1e18` だが種類数が `N` 以下の場合
> - 「値の大小関係は必要だが具体的な値は不要」という問題
> - BIT と座標圧縮を組み合わせた転倒数や区間カウント

---

## 累積和

### CumulativeSum (1 次元)

配列の区間和を O(1) で求める。更新は不可（静的）。

```cpp
CumulativeSum<ll> cs(n);
rep(i, n) cs.add(i, a[i]);  // または cs.add(i, x) で各要素を加算
cs.build();                  // 必ず build() してから使う
ll s = cs.sum(l, r);         // a[l] + a[l+1] + ... + a[r-1]
```

> **使う場面**:
>
> - 「区間 [l, r) の和を複数回求める」
> - しゃくとり法や二分探索と組み合わせて条件を満たす区間を探す
> - 差分配列の代わりに使う

### CumulativeSum2D (2 次元)

矩形領域の総和を O(1) で求める。

```cpp
CumulativeSum2D<ll> cs2(h, w);
rep(i, h) rep(j, w) cs2.add(i, j, grid[i][j]);
cs2.build();
ll s = cs2.sum(i1, j1, i2, j2);  // 行 [i1,i2)、列 [j1,j2) の矩形和
```

> **使う場面**:
>
> - グリッド上の矩形領域の合計を複数回求める問題
> - 「h×w のグリッドで条件を満たす部分矩形を数える」

---

## 行列 Matrix

行列の積と累乗を計算する。`T` に `mint` を使うと、mod を取りながら計算できる。

```cpp
Matrix<mint> A(n, n);
A[i][j] = x;
auto B = A.pow(k);
auto C = A * B;
auto I = Matrix<mint>::E(n);
```

`A * B` は行列積、`A.pow(k)` は k 乗、`Matrix<T>::E(n)` は n 次の単位行列を返す。
`Matrix<T>(vector<vector<T>>)` で 2 次元配列からも作れる。
積は左の列数と右の行数が一致しないと、`pow` は正方行列でないと assert で止まる。

> **計算量**: n 次正方行列の積 O(n^3)、k 乗 O(n^3 log k)

---

## ローリングハッシュ RollingHash

Mersenne 素数 `2^61 - 1` を法とする衝突率の低いハッシュ。
base はデフォルトで 10007 で、`RollingHash rh(s, base)` のように第 2 引数で変えられる。

```cpp
RollingHash rh(s);
uint64_t h  = rh.get(l, r);              // s[l..r) のハッシュ値
bool eq     = rh.match(l1, r1, l2, r2); // 2 部分文字列が等しいか O(1)
// 2 つのハッシュを連結（s1 + s2 のハッシュを求める）
uint64_t combined = RollingHash::connect(h1, h2, len2);
```

> **使う場面**:
>
> - 「文字列 S に文字列 T が含まれるか」（KMP の代替）
> - 「2 つの部分文字列が等しいか」を O(1) で判定
> - 回文判定（正順と逆順のハッシュを比較）
> - 「同じ部分文字列が何種類あるか」（二分探索と組み合わせ）
>
> **注意**: 累乗のテーブル `power` は全インスタンスで共有する static メンバで、構築のたびに、その文字列の長さと base で作り直される。base が違うインスタンスを同時に使うと、先に構築したほうの `get` と `connect` は正しい値を返さない。同じ base でも、後から短い文字列で構築すると、先に構築したほうで長い区間を `get` したときにテーブルの範囲外を読む。

---

## 文字列アルゴリズム

```cpp
auto z = string_algo::z_algorithm(s);
auto pi = string_algo::kmp_table(pat);
auto pos = string_algo::kmp_search(text, pat);  // 出現開始位置
auto sa = string_algo::suffix_array(s);
auto lcp = string_algo::lcp_array(s, sa);
auto rad = string_algo::manacher(s);  // '#' 挟み込み後の半径配列 (長さ 2n+1)
```

---

## graph 名前空間

### Dijkstra

非負重み付きグラフの単一始点最短経路。

```cpp
// g[v] = {(隣接頂点 u, 辺の重み cost), ...}
vector<vector<pair<int,ll>>> g(n);
g[u].eb(v, cost);
auto dist = graph::dijkstra(g, s);  // dist[v] = s→v の最短距離（未到達は LINF）
```

> **使う場面**:
>
> - 重み付きグラフで「最短コストで移動できるか」
> - グリッドでマス移動にコストがある問題
> - 「K 回以内で到達できる最小コスト」(状態を (頂点, 残り回数) に拡張)
>
> **計算量**: O((V + E) log V)
> **注意**: 負の辺がある場合は使えない（ベルマンフォードを使う）

### BFS

重みなしグラフ・グリッドの最短経路。

```cpp
// g[v] = {隣接頂点, ...}
auto dist = graph::bfs(g, s);  // dist[v] = s→v の最短距離（辺数）、未到達は -1
auto dist01 = graph::bfs01(g01, s);  // 辺重み 0/1。g01[v] = {(u, w), ...}、未到達は LINF
```

> **使う場面**:
>
> - 迷路の最短手数
> - 0-1 BFS（辺の重みが 0 か 1 の場合は deque で拡張）
> - 連結成分の分類
>
> **計算量**: O(V + E)

### Warshall-Floyd

全頂点間の最短経路を一括計算する。

```cpp
// dist[i][j] = i→j の距離（辺なし = LINF、自己ループ = 0）
auto dist = vv<ll>(n, n, LINF);
rep(i, n) dist[i][i] = 0;
// 辺の追加
dist[u][v] = cost;
graph::warshall_floyd(dist);
// 負閉路の検出: dist[i][i] < 0 の頂点が存在する場合
```

> **使う場面**:
>
> - 全頂点間の最短距離が必要な問題
> - 「任意の 2 頂点間を経由するコスト」
> - 閉路検出
>
> **計算量**: O(V^3)
> **注意**: V が 500 程度以下でないと TLE になる

### topological_sort

DAG の頂点を依存関係順に並べる（Kahn's algorithm）。

```cpp
// g[v] = {v から出る辺の先の頂点}
auto order = graph::topological_sort(g);
if (order.empty()) {
    // DAG でない（閉路が存在する）
}
```

> **使う場面**:
>
> - 「タスクの実行順序を求める」
> - DAG 上の DP（最長経路・経路数）
> - 「A の前に B を処理する必要がある」という依存関係の解決
> - 閉路の有無を判定する（空ならサイクルあり）
>
> **計算量**: O(V + E)

### kruskal (最小全域木)

辺コストの合計が最小になる全域木を求める。

```cpp
// edges[i] = {コスト, {頂点 u, 頂点 v}}
vector<pair<ll, pii>> edges;
edges.eb(cost, mp(u, v));
auto [total, used] = graph::kruskal(n, edges);
// total: MST の総コスト
// used:  採用した辺のリスト
```

> **使う場面**:
>
> - 「N 地点を最小コストで全て連結する」
> - 「最大コストの辺を最小にしてグラフを連結する（ミニマックスパス）」
> - 「余分な辺を取り除いて木にする」
>
> **計算量**: O(E log E)

### LCA (最小共通祖先)

根付き木上で 2 頂点の最も深い共通祖先をダブリングで求める。

```cpp
// g: 無向隣接リスト（木）
graph::LCA lca(g, 0);         // root = 0 で前計算
int anc = lca.lca(u, v);     // u と v の LCA ノード番号
int d   = lca.dist(u, v);    // u-v 間のパス長（辺数）
int dep = lca.depth[v];      // v の深さ（根からの距離）
```

> **使う場面**:
>
> - 「木上の 2 頂点間のパスに関するクエリ」
> - 「u-v 間のパスの長さ」= `depth[u] + depth[v] - 2*depth[lca(u,v)]`
> - BIT/SegTree と組み合わせたパスクエリ（Heavy-Light Decomposition の前段）
>
> **前計算**: O(n log n)、**クエリ**: O(log n)

### bellman_ford

負の辺を含むグラフの単一始点最短経路。
未到達の頂点は LINF、負閉路を経由して到達できる頂点は -LINF になる。

```cpp
vector<tuple<int, int, ll>> edges;
edges.eb(u, v, cost);
auto dist = graph::bellman_ford(n, edges, s);
```

> **計算量**: O(VE)

### scc

強連結成分分解。`comp[v]` は頂点 v が属する成分の番号で、番号は成分を縮約したグラフのトポロジカル順に振られる。

```cpp
auto comp = graph::scc(g);
```

> **計算量**: O(V + E)

### MaxFlow

Dinic 法で最大流を求める。

```cpp
graph::MaxFlow mf(n);
mf.add_edge(u, v, cap);
ll flow = mf.max_flow(s, t);
```

### EulerTour

根付き木の頂点に、DFS の行きがけ順で番号を振る。
頂点 v の部分木は区間 `[in[v], out[v])` に対応するので、BIT や SegTree と組み合わせて部分木クエリを処理できる。
`is_ancestor(u, v)` は、u が v の祖先なら true を返す。u と v が同じ頂点のときも true になる。

```cpp
graph::EulerTour et(g, 0);
int l = et.in[v], r = et.out[v];
bool anc = et.is_ancestor(u, v);
```

### tree_diameter

木の直径を辺数で求め、長さと両端の頂点を返す。

```cpp
auto [diam, uv] = graph::tree_diameter(g);  // 長さと両端の頂点
```

### TwoSat

```cpp
graph::TwoSat ts(n);
ts.add_clause(i, true, j, false);  // x_i ∨ ¬x_j
if (ts.satisfiable()) {
    bool xi = ts.ans[i];
}
```

### MinCostFlow

s から t へ最大 maxf だけ流し、`{流量, 費用}` を返す。
ポテンシャルを 0 から始めるので、辺のコストは 0 以上にする。

```cpp
graph::MinCostFlow mcf(n);
mcf.add_edge(u, v, cap, cost);
auto [flow, cost] = mcf.min_cost_flow(s, t, maxf);
```

---

## 依存関係マップ

```mermaid
graph TD
    BIT["structure::BIT"]
    UF["structure::UnionFind"]
    WUF["structure::WeightedUnionFind"]
    SEG["structure::SegTree"]
    LAZY["structure::LazySegTree"]
    SPARSE["structure::SparseTable"]
    POWMOD["math::pow_mod"]
    COMB["math::combination"]
    ISPRIME["math::is_prime"]
    FACT["math::factorize"]
    SPF["math::sieve_spf"]
    SIEVE["math::sieve"]
    PLIST["math::prime_list"]
    EXTGCD["math::extgcd"]
    CRT["math::crt"]
    KRUSKAL["graph::kruskal"]
    LCA["graph::LCA"]
    MODINT["modint / mint / mint998"]
    COMPRESS["Compress"]
    CS["CumulativeSum"]
    CS2["CumulativeSum2D"]
    MATRIX["Matrix"]
    RH["RollingHash"]
    DIJKSTRA["graph::dijkstra"]
    BFS["graph::bfs"]
    BFS01["graph::bfs01"]
    WF["graph::warshall_floyd"]
    TOPO["graph::topological_sort"]
    BF["graph::bellman_ford"]
    SCC["graph::scc"]
    MAXFLOW["graph::MaxFlow"]
    EULER["graph::EulerTour"]
    TWOSAT["graph::TwoSat"]
    MCF["graph::MinCostFlow"]
    DIAM["graph::tree_diameter"]

    KRUSKAL --> UF
    COMB --> POWMOD
    ISPRIME --> POWMOD
    FACT --> ISPRIME
    SIEVE --> SPF
    PLIST --> SPF
    CRT --> EXTGCD
    TWOSAT --> SCC
    DIAM --> BFS
```
