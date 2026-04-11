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
