// @begin RollingHash
// ローリングハッシュ
struct RollingHash {
  static const uint64_t mod = (1ull << 61ull) - 1;
  static vector<uint64_t> power;
  vector<uint64_t> hash;
  const uint64_t base;

  static uint64_t multiply(uint64_t a, uint64_t b) {
    __uint128_t t = (__uint128_t)a * b;
    t = (t >> 61) + (t & mod);
    if (t >= mod)
      t -= mod;
    return (uint64_t)t;
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
