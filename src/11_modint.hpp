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
