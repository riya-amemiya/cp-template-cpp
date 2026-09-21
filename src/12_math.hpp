// @begin math
// @exports pow_mod is_prime miller_rabin sieve_spf sieve prime_list factorize divisors extgcd
// combination crt floor_sum euler_totient euler_totient_table floor_div ceil_div mul_mod pollard 数学関連の関数群
namespace math {
ll mul_mod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

// 高速な累乗計算 (繰り返し二乗法, __int128 でオーバーフロー回避)
template <typename T> T pow_mod(T a, T n, T m) {
  T ret = 1 % m;
  a %= m;
  if (a < 0)
    a += m;
  while (n > 0) {
    if (n & 1)
      ret = (T)((__int128)ret * a % m);
    a = (T)((__int128)a * a % m);
    n >>= 1;
  }
  return ret;
}

bool miller_rabin(ll n) {
  if (n < 2)
    return false;
  if (n % 2 == 0)
    return n == 2;
  ll d = n - 1;
  int s = 0;
  while ((d & 1) == 0) {
    d >>= 1;
    s++;
  }
  auto witness = [&](ll a) {
    if (a % n == 0)
      return false;
    ll x = pow_mod<ll>(a % n, d, n);
    if (x == 1 || x == n - 1)
      return false;
    for (int r = 1; r < s; r++) {
      x = mul_mod(x, x, n);
      if (x == n - 1)
        return false;
    }
    return true;
  };
  for (ll a : {2LL, 325LL, 9375LL, 28178LL, 450775LL, 9780504LL, 1795265022LL}) {
    if (a % n == 0)
      return true;
    if (witness(a))
      return false;
  }
  return true;
}

// 素数判定 (64bit 決定的 Miller-Rabin)
bool is_prime(ll n) { return miller_rabin(n); }

// 線形篩: 最小素因数テーブル O(n)
vector<int> sieve_spf(int n) {
  vector<int> spf(n + 1), primes;
  for (int i = 2; i <= n; i++) {
    if (spf[i] == 0) {
      spf[i] = i;
      primes.push_back(i);
    }
    for (int p : primes) {
      if (p > spf[i] || (ll)p * i > n)
        break;
      spf[p * i] = p;
    }
  }
  return spf;
}

// エラトステネスの篩 (線形篩)
vector<char> sieve(int n) {
  auto spf = sieve_spf(n);
  vector<char> is_p(n + 1, 0);
  for (int i = 2; i <= n; i++)
    is_p[i] = spf[i] == i;
  return is_p;
}

// エラトステネスの篩で素数リストを返す
vector<int> prime_list(int n) {
  auto spf = sieve_spf(n);
  vector<int> primes;
  for (int i = 2; i <= n; i++)
    if (spf[i] == i)
      primes.push_back(i);
  return primes;
}

ll pollard(ll n) {
  if (n % 2 == 0)
    return 2;
  if (n % 3 == 0)
    return 3;
  ll c = 1;
  while (true) {
    auto f = [&](ll x) { return (mul_mod(x, x, n) + c) % n; };
    ll x = 2, y = 2, d = 1;
    while (d == 1) {
      x = f(x);
      y = f(f(y));
      ll diff = x > y ? x - y : y - x;
      d = std::gcd(diff, n);
    }
    if (d != n)
      return d;
    c++;
  }
}

// 素因数分解 (小さい因数は試し割り、残りは Pollard's Rho)
vector<pair<ll, int>> factorize(ll n) {
  vector<pair<ll, int>> res;
  if (n <= 1)
    return res;
  auto bump = [&](ll p) {
    int cnt = 0;
    while (n % p == 0) {
      n /= p;
      cnt++;
    }
    if (cnt)
      res.emplace_back(p, cnt);
  };
  bump(2);
  bump(3);
  for (ll p = 5; p * p <= n && p <= 1000000; p += 6) {
    bump(p);
    bump(p + 2);
  }
  if (n == 1)
    return res;
  vector<ll> extra;
  auto rec = [&](auto &&self, ll x) -> void {
    if (x == 1)
      return;
    if (is_prime(x)) {
      extra.push_back(x);
      return;
    }
    ll d = pollard(x);
    self(self, d);
    self(self, x / d);
  };
  rec(rec, n);
  sort(all(extra));
  for (ll p : extra) {
    if (!res.empty() && res.back().first == p)
      res.back().second++;
    else
      res.emplace_back(p, 1);
  }
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

// 負数対応の floor/ceil 除算 (ゼロ方向ではなく数学的な床・天井)
ll floor_div(ll a, ll b) {
  ll q = a / b, r = a % b;
  if (r != 0 && ((a < 0) != (b < 0)))
    q--;
  return q;
}
ll ceil_div(ll a, ll b) {
  ll q = a / b, r = a % b;
  if (r != 0 && ((a < 0) == (b < 0)))
    q++;
  return q;
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
